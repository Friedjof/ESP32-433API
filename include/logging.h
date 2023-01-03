#include "SPIFFS.h"


void logToSPIFFS(String message) {
    File logFile = SPIFFS.open("/system.log", FILE_APPEND);
    if(!logFile){
        Serial.println("Failed to open log file for writing");
    }
    logFile.println(message);
    logFile.close();
}

void info_log(String message) {
    message = "[INFO] " + message;
    Serial.println(message);
    logToSPIFFS(message);
}

void error_log(String message) {
    message = "[ERROR] " + message;
    Serial.println(message);
    logToSPIFFS(message);
}

void debug_log(String message) {
    message = "[DEBUG] " + message;
    Serial.println(message);
    logToSPIFFS(message);
}

void warning_log(String message) {
    message = "[WARNING] " + message;
    Serial.println(message);
    logToSPIFFS(message);
}

void fatal_log(String message) {
    message = "[FATAL] " + message;
    Serial.println(message);
    logToSPIFFS(message);
}