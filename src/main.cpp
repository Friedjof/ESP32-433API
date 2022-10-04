#include <Arduino.h>

#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <RCSwitch.h>

#define API_KEY "xxx"

const char* ssid = "xxx";
const char* password = "xxx";

AsyncWebServer server(80);
RCSwitch transmiter = RCSwitch();

bool isAuthentificated(AsyncWebServerRequest *request);
void transmit(String message, AsyncWebServerRequest *request);

void setup() {
  Serial.begin(9600);

  // Setup Wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  Serial.print("The IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("The MAC address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("The SSID: ");
  Serial.println(WiFi.SSID());

  // Setup 433 MHz
  transmiter.enableTransmit(27);
  transmiter.setProtocol(1);
  transmiter.setPulseLength(500);

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
    if (key == API_KEY) {
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