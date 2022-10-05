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

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // Log the IP and MAC address
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());

  // Setup 433 MHz
  transmiter.enableTransmit(cfg.transmitter.pin);
  transmiter.setProtocol(cfg.transmitter.protocol);
  transmiter.setPulseLength(cfg.transmitter.pulse_length);

  // Setup Webserver
  server.begin();

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