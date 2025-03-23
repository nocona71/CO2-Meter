#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"
#include "Logger.h"

class DisplayManager {
private:
    Adafruit_SSD1306 display;
    LogLevel currentLogLevel;

public:
    DisplayManager();
    
    // Method to set the log level
    void setLogLevel(LogLevel level);
    
    // Get current log level
    LogLevel getLogLevel() const;
    
    // Log methods
    void log(LogLevel level, const char* message);
    void logError(const char* message);
    void logWarning(const char* message);
    void logInfo(const char* message);
    void logDebug(const char* message);
    
    // Display methods
    bool initialize();
    void showHeadline(const char* text);
    void showCalibrationMessage(const char* message1, const char* message2);
    void showReadings(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure);
};

#endif // DISPLAY_MANAGER_H