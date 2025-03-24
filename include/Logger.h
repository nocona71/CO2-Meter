#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

// Define log levels
enum LogLevel {
    LOG_NONE = 0,    // No logging
    LOG_ERROR = 1,   // Only errors
    LOG_WARNING = 2, // Errors and warnings
    LOG_INFO = 3,    // Normal information
    LOG_DEBUG = 4    // Verbose debug information
};

class Logger {
private:
    static LogLevel currentLogLevel;

public:
    // Define enum-like constants to use in code (avoiding macro conflicts)
    static const LogLevel NONE;
    static const LogLevel ERROR;
    static const LogLevel WARNING;
    static const LogLevel INFO;
    static const LogLevel DBG;  // Using DBG instead of DEBUG to avoid macro conflicts

    // Method to set the log level
    static void setLogLevel(LogLevel level);

    // Get current log level
    static LogLevel getLogLevel();

    // Log a message if it meets the current log level
    static void log(LogLevel level, const char* message);

    // Convenience methods for different log levels
    static void error(const char* message);
    static void warning(const char* message);
    static void info(const char* message);
    static void debug(const char* message);
};

#endif // LOGGER_H