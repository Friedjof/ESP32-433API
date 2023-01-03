#include "SPIFFS.h"
#include <NTPClient.h>


void logToSPIFFS(String message) {
    File logFile = SPIFFS.open("/system.log", FILE_APPEND);
    if(!logFile){
        Serial.println("Failed to open log file for writing");
    }
    logFile.println(message);
    logFile.close();
}

void info_log(String message) {
    Serial.println("[INFO] " + message);
    logToSPIFFS(message);
}

void error_log(String message) {
    Serial.println("[ERROR] " + message);
    logToSPIFFS(message);
}

void debug_log(String message) {
    Serial.println("[DEBUG] " + message);
    logToSPIFFS(message);
}

void warning_log(String message) {
    Serial.println("[WARNING] " + message);
    logToSPIFFS(message);
}

void fatal_log(String message) {
    Serial.println("[FATAL] " + message);
    logToSPIFFS(message);
}