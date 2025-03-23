#include "Logger.h"

// Initialize the static member variable
LogLevel Logger::currentLogLevel = LOG_INFO;

// Initialize static constants
const LogLevel Logger::NONE = LOG_NONE;
const LogLevel Logger::ERROR = LOG_ERROR;
const LogLevel Logger::WARNING = LOG_WARNING;
const LogLevel Logger::INFO = LOG_INFO;
const LogLevel Logger::DBG = LOG_DEBUG;

void Logger::setLogLevel(LogLevel level) {
    currentLogLevel = level;
}

LogLevel Logger::getLogLevel() {
    return currentLogLevel;
}

void Logger::log(LogLevel level, const char* message) {
    if (level <= currentLogLevel) {
        Serial.print("[");
        switch (level) {
            case LOG_ERROR: Serial.print("ERROR"); break;
            case LOG_WARNING: Serial.print("WARNING"); break;
            case LOG_INFO: Serial.print("INFO"); break;
            case LOG_DEBUG: Serial.print("DEBUG"); break;
            default: Serial.print("UNKNOWN"); break;
        }
        Serial.print("] ");
        Serial.println(message);
    }
}

void Logger::error(const char* message) { 
    log(LOG_ERROR, message); 
}

void Logger::warning(const char* message) { 
    log(LOG_WARNING, message); 
}

void Logger::info(const char* message) { 
    log(LOG_INFO, message); 
}

void Logger::debug(const char* message) { 
    log(LOG_DEBUG, message);  // Make sure this is LOG_DEBUG (value 4)
}