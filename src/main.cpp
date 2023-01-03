#include <Arduino.h>

#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <RCSwitch.h>

#include <WiFiUdp.h>
#include <time.h>

#include "esp_log.h"
#include "logging.h"

#include <ArduinoJson.h>

#include <config.h>

bool isAuthentificated(AsyncWebServerRequest *request);
void transmit(String message, AsyncWebServerRequest *request);

String addTimeToLog(String log);
String getFormattedDateTime();

AsyncWebServer server(80);
RCSwitch transmiter = RCSwitch();

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

config cfg;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting up...");

  readConfig(&cfg);

  // Setup Logging to file

  Serial.println("Connecting to WiFi...");

  WiFi.begin(cfg.wifi.ssid.c_str(), cfg.wifi.password.c_str());

  for (int i = 0; i < 10; i++) {
    if (WiFi.status() == WL_CONNECTED) {
      break;
    }
    delay(500);
    Serial.print(".");
  }

  // check if connected
  // if not, start AP mode
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, starting AP mode...");
    WiFi.softAP("ESP32-433API");
    IPAddress IP = WiFi.softAPIP();
    // Log link to config page
    Serial.println("Please connect to the AP and go to http://" + IP.toString() + "/ to configure the device.");
  } else {
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Setup NTP
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    delay(1000);
    Serial.println("Time: " + getFormattedDateTime());
  }

  // Setup 433 MHz
  transmiter.enableTransmit(cfg.transmitter.pin);
  transmiter.setProtocol(cfg.transmitter.protocol);
  transmiter.setPulseLength(cfg.transmitter.pulse_length);

  Serial.println("433 MHz config as finished.");

  // Setup Webserver
  server.begin();

  // Setup routes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("A request was made to GET /");
    if (WiFi.status() != WL_CONNECTED){
      Serial.print("Loading config page...");
      request->send(SPIFFS, "/config.html", "text/html");
    } else {
      Serial.println("Send the wifi status message");
      request->send(200, "text/plain", "WiFi Status: Connected");
    }
    Serial.println("…done [GET /]");
  });

  server.on("/styles", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/styles.css", "text/css");
  });

  server.on("/logs", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!isAuthentificated(request)) {
      request->send(401, "text/plain", "Not Authentificated");
      return;
    }

    if (request->hasParam("clear")) {
      Serial.println("Clearing the log file");
      SPIFFS.remove("/system.log");
      request->send(200, "text/plain", "The log file was cleared");
    } else {
      request->send(SPIFFS, "/system.log", "text/plain");
    }
  }); 

  server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("A request was made to GET /config");

    if (request->hasParam("ssid") && request->hasParam("password")) {
      cfg.wifi.ssid = request->getParam("ssid")->value();
      cfg.wifi.password = request->getParam("password")->value();

      // save configuration
      saveConfig(&cfg);

      request->send(200, "text/plain", "WiFi configuration saved. The ESP will reboot now…");

      ESP.restart();
    } else if (request->hasParam("show")) {
      serializeConfig();

      info_log(addTimeToLog("Configuration was requested"));
      request->send(200, "text/plain", "The configuration was logged to the serial console");
    } else {
      request->send(406, "text/plain", "Configuration not acceptable");
    }
  });

  server.on("/transmit", HTTP_GET, [](AsyncWebServerRequest *request){
    // Authontification
    if (!isAuthentificated(request)) {
      request->send(401, "text/plain", "Not Authentificated");
      return;
    }

    // Message Parameter
    if (request->hasParam("msg")) {
      String message = request->getParam("msg")->value();
      transmit(message, request);
      request->send(200, "text/plain", "transmitted: " + message);
    } else {
      request->send(400, "text/plain", "No message sent");
    }
  });
}

bool isAuthentificated(AsyncWebServerRequest *request) {
  if (request->hasParam("key")) {
    String key = request->getParam("key")->value();

    if (key == cfg.api.key) {
      return true;
    } else {
      Serial.print("The API key is not correct. The correct key is: ");
      Serial.println(cfg.api.key);
    }
  }
  return false;
}

void transmit(String message, AsyncWebServerRequest *request) {
  if (request -> hasParam("type")) {
    String transmit_type = request->getParam("type")->value();
    if (transmit_type == "hex") {
      Serial.print("hex, msg: ");
      Serial.println(message);
      // transmit hex message
      transmiter.sendTriState(message.c_str());
    } else {
      // transmit binary message
      Serial.print("type: ");
      Serial.println(transmit_type);
      transmiter.send(message.c_str());
    }
  }
  info_log(addTimeToLog("Transmitted: " + message));
}

String addTimeToLog(String log) {
  return getFormattedDateTime() + " || " + log;
}

String getFormattedDateTime() {
  time_t now = time(nullptr);
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);
  char timeString[20];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(timeString);
}

void loop() { }