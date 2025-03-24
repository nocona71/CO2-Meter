#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

/**
 * @file Logger.h
 * @brief Provides logging functionality with different log levels for debugging and monitoring.
 */

/**
 * @enum LogLevel
 * @brief Defines the different levels of logging.
 * 
 * - `LOG_NONE`: No logging.
 * - `LOG_ERROR`: Logs only errors.
 * - `LOG_WARNING`: Logs errors and warnings.
 * - `LOG_INFO`: Logs informational messages, warnings, and errors.
 * - `LOG_DEBUG`: Logs all messages, including verbose debug information.
 */
enum LogLevel {
    LOG_NONE = 0,    ///< No logging
    LOG_ERROR = 1,   ///< Only errors
    LOG_WARNING = 2, ///< Errors and warnings
    LOG_INFO = 3,    ///< Normal information
    LOG_DEBUG = 4    ///< Verbose debug information
};

/**
 * @class Logger
 * @brief A utility class for logging messages at different log levels.
 * 
 * The `Logger` class provides methods to log messages at various levels (error, warning, info, debug).
 * It allows setting a global log level to control the verbosity of the logs.
 */
class Logger {
private:
    static LogLevel currentLogLevel; ///< The current log level for filtering messages.

public:
    /**
     * @brief Enum-like constants for log levels.
     */
    static const LogLevel NONE;    ///< No logging.
    static const LogLevel ERROR;   ///< Error logging.
    static const LogLevel WARNING; ///< Warning logging.
    static const LogLevel INFO;    ///< Informational logging.
    static const LogLevel DBG;     ///< Debug logging (renamed to avoid macro conflicts).

    /**
     * @brief Sets the current log level.
     * 
     * @param level The log level to set (e.g., `LOG_ERROR`, `LOG_INFO`).
     */
    static void setLogLevel(LogLevel level);

    /**
     * @brief Gets the current log level.
     * 
     * @return The current log level.
     */
    static LogLevel getLogLevel();

    /**
     * @brief Logs a message if it meets the current log level.
     * 
     * @param level The log level of the message.
     * @param message The message to log.
     */
    static void log(LogLevel level, const char* message);

    /**
     * @brief Logs an error message.
     * 
     * @param message The error message to log.
     */
    static void error(const char* message);

    /**
     * @brief Logs a warning message.
     * 
     * @param message The warning message to log.
     */
    static void warning(const char* message);

    /**
     * @brief Logs an informational message.
     * 
     * @param message The informational message to log.
     */
    static void info(const char* message);

    /**
     * @brief Logs a debug message.
     * 
     * @param message The debug message to log.
     */
    static void debug(const char* message);
};

#endif // LOGGER_H