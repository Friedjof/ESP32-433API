#include <Arduino.h>

#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <RCSwitch.h>

#include <ArduinoJson.h>

#include <config.h>

bool isAuthentificated(AsyncWebServerRequest *request);
void transmit(String message, AsyncWebServerRequest *request);

AsyncWebServer server(80);
RCSwitch transmiter = RCSwitch();

config cfg;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting up...");

  readConfig(&cfg);

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
    Serial.print("AP IP address: ");
    // Log link to config page
    Serial.println("Please connect to the AP and go to http://" + IP.toString() + "/ to configure the device.");
  } else {
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  // Setup 433 MHz
  transmiter.enableTransmit(cfg.transmitter.pin);
  transmiter.setProtocol(cfg.transmitter.protocol);
  transmiter.setPulseLength(cfg.transmitter.pulse_length);

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

  server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("A request was made to POST /config");

    if (request->hasParam("ssid") && request->hasParam("password")) {
      cfg.wifi.ssid = request->getParam("ssid")->value();
      cfg.wifi.password = request->getParam("password")->value();

      // log configuration
      Serial.println("SSID: " + cfg.wifi.ssid);
      Serial.println("Password: " + cfg.wifi.password);

      // save configuration
      saveConfig(&cfg);

      request->send(200, "text/plain", "WiFi configuration saved");
    } else if (request->hasParam("show")) {
      serializeConfig();
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
    }
  }
  return false;
}

void transmit(String message, AsyncWebServerRequest *request) {
  if (request -> hasParam("type")) {
    String transmit_type = request->getParam("type")->value();
    if (transmit_type == "hex") {
      // transmit hex message
      transmiter.sendTriState(message.c_str());
    } else {
      // transmit binary message
      transmiter.send(message.c_str());
    }
  }
}

void loop() { }