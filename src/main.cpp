#include <Arduino.h>

#include <WiFi.h>
#include <SPIFFS.h>

#include <ArduinoJson.h>

#include <config.h>

config readConfig();

void setup() {
  Serial.begin(9600);

  // begin SPIFFS filesystem
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
  } else {
    Serial.println("Ok");
  }

  config cfg = readConfig();

  Serial.println("Config:");
  Serial.println(cfg.wifi.ssid);
  Serial.println(cfg.wifi.hostname);
  Serial.println(cfg.wifi.port);
}

void loop() { }