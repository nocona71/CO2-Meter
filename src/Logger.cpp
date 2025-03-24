#include "Logger.h"
#include <Arduino.h>
#include "config.h"

/**
 * @file Logger.cpp
 * @brief Implements logging functionality with different log levels for debugging and monitoring.
 */

/**
 * @brief Initializes the static member variable for the current log level.
 */
LogLevel Logger::currentLogLevel = static_cast<LogLevel>(LOG_LEVEL);

/**
 * @brief Initializes static constants for log levels.
 */
const LogLevel Logger::NONE = LOG_NONE;
const LogLevel Logger::ERROR = LOG_ERROR;
const LogLevel Logger::WARNING = LOG_WARNING;
const LogLevel Logger::INFO = LOG_INFO;
const LogLevel Logger::DBG = LOG_DEBUG;

/**
 * @brief Sets the current log level.
 * 
 * @param level The log level to set (e.g., `LOG_ERROR`, `LOG_INFO`).
 */
void Logger::setLogLevel(LogLevel level) {
    currentLogLevel = level;
}

/**
 * @brief Gets the current log level.
 * 
 * @return The current log level.
 */
LogLevel Logger::getLogLevel() {
    return currentLogLevel;
}

/**
 * @brief Logs a message if it meets the current log level.
 * 
 * @param level The log level of the message.
 * @param message The message to log.
 */
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

/**
 * @brief Logs an error message.
 * 
 * @param message The error message to log.
 */
void Logger::error(const char* message) { 
    log(LOG_ERROR, message); 
}

/**
 * @brief Logs a warning message.
 * 
 * @param message The warning message to log.
 */
void Logger::warning(const char* message) { 
    log(LOG_WARNING, message); 
}

/**
 * @brief Logs an informational message.
 * 
 * @param message The informational message to log.
 */
void Logger::info(const char* message) { 
    log(LOG_INFO, message); 
}

/**
 * @brief Logs a debug message.
 * 
 * @param message The debug message to log.
 */
void Logger::debug(const char* message) { 
    log(LOG_DEBUG, message); 
}