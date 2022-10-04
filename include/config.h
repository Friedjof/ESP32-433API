#include "FS.h"
#include <ArduinoJson.h>

#include <config_structs.h>


config readConfig() {
  config cfg;
  
  File configFile = SPIFFS.open("/config.json");
  if(!configFile || configFile.isDirectory()){
    Serial.println("[ERROR] failed to open file for reading");
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    configFile.close();
    return cfg;
  }

  std::unique_ptr<char[]> buf(new char[size]);

  configFile.readBytes(buf.get(), size);
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, buf.get());
  if (error) {
    Serial.println("Failed to parse config file");
    configFile.close();
    return cfg;
  }

    const char* ssid = doc["wifi"]["ssid"];
    const char* password = doc["wifi"]["password"];
    const char* hostname = doc["wifi"]["hostname"];
    int port = doc["wifi"]["port"];
    int serial_baudrate = doc["microcontroller"]["serial_baudrate"];
    int pin = doc["transmitter"]["pin"];
    int protocol = doc["transmitter"]["protocol"];
    int pulse_length = doc["transmitter"]["pulse_length"];
    const char* token = doc["api"]["token"];

    cfg.wifi.ssid = ssid;
    cfg.wifi.password = password;
    cfg.wifi.hostname = hostname;
    cfg.wifi.port = port;
    cfg.microcontroller.serial_baudrate = serial_baudrate;
    cfg.transmitter.pin = pin;
    cfg.transmitter.protocol = protocol;
    cfg.transmitter.pulse_length = pulse_length;
    cfg.api.token = token;

    return cfg;
}