#include "FS.h"
#include "SPIFFS.h"

#include <ArduinoJson.h>

#include <config_structs.h>

#define DOC_SIZE 2048


void readConfig(config *cfg) {
    // begin SPIFFS filesystem
    Serial.println("begin SPIFFS filesystem…");
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
    } else {
        Serial.println("…done");
    }
    
    File configFile = SPIFFS.open("/config.json");
    if(!configFile || configFile.isDirectory()){
        Serial.println("[ERROR] failed to open file for reading");
    }

    size_t size = configFile.size();
    Serial.println("data size: " + String(size));
    if (size > DOC_SIZE) {
        Serial.println("Config file size is too large");
        configFile.close();
    }

    std::unique_ptr<char[]> buf(new char[size]);

    configFile.readBytes(buf.get(), size);
    DynamicJsonDocument doc(DOC_SIZE);
    DeserializationError error = deserializeJson(doc, buf.get());
    if (error) {
        Serial.println("Failed to parse config file");
        configFile.close();
    }

    const char* ssid = doc["wifi"]["ssid"];
    const char* password = doc["wifi"]["password"];
    const char* hostname = doc["wifi"]["hostname"];
    int port = doc["wifi"]["port"];
    int serial_baudrate = doc["microcontroller"]["serial_baudrate"];
    int pin = doc["transmitter"]["pin"];
    int protocol = doc["transmitter"]["protocol"];
    int pulse_length = doc["transmitter"]["pulse_length"];
    const char* key = doc["api"]["key"];

    cfg->wifi.ssid = ssid;
    cfg->wifi.password = password;
    cfg->wifi.hostname = hostname;

    cfg->microcontroller.serial_baudrate = serial_baudrate;

    cfg->transmitter.pin = pin;
    cfg->transmitter.protocol = protocol;
    cfg->transmitter.pulse_length = pulse_length;

    cfg->api.key = key;
    cfg->api.port = port;

    configFile.close();
}

// this function will print the configuration in the config.json file to the serial monitor
bool serializeConfig() {
    File configFile = SPIFFS.open("/config.json");
    if(!configFile || configFile.isDirectory()){
        Serial.println("[ERROR] failed to open file for reading");
    }

    size_t size = configFile.size();
    Serial.println("data size: " + String(size));
    if (size > DOC_SIZE) {
        Serial.println("Config file size is too large");
        configFile.close();
    }

    std::unique_ptr<char[]> buf(new char[size]);

    configFile.readBytes(buf.get(), size);
    DynamicJsonDocument doc(DOC_SIZE);
    DeserializationError error = deserializeJson(doc, buf.get());
    if (error) {
        Serial.println("Failed to parse config file");
        configFile.close();
    }

    serializeJson(doc, Serial);
    Serial.println();
    configFile.close();
    return true;
}

bool saveConfig(config *cfg) {
    File configFile = SPIFFS.open("/config.json", "w");
    if(!configFile){
        Serial.println("[ERROR] failed to open file for writing");
    }

    // log ssid and password
    Serial.println("Saving config to file...");
    Serial.println("SSID: " + cfg->wifi.ssid);
    Serial.println("Password: " + cfg->wifi.password);

    DynamicJsonDocument doc(DOC_SIZE);
    doc["wifi"]["ssid"] = cfg->wifi.ssid;
    doc["wifi"]["password"] = cfg->wifi.password;
    doc["wifi"]["hostname"] = cfg->wifi.hostname;

    doc["microcontroller"]["serial_baudrate"] = cfg->microcontroller.serial_baudrate;

    doc["transmitter"]["pin"] = cfg->transmitter.pin;
    doc["transmitter"]["protocol"] = cfg->transmitter.protocol;
    doc["transmitter"]["pulse_length"] = cfg->transmitter.pulse_length;

    doc["api"]["key"] = cfg->api.key;
    doc["api"]["port"] = cfg->api.port;

    Serial.print("SSID: ");
    Serial.println(cfg->wifi.ssid);
    Serial.print("Password: ");
    Serial.println(cfg->wifi.password);

    // log JSON to serial
    serializeJson(doc, Serial);
    serializeJson(doc, configFile);

    return true;
} 