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
    virtual ~DisplayManager() {} // Add virtual destructor
    
    // Method to set the log level
    void setLogLevel(LogLevel level);
    
    // Get current log level
    LogLevel getLogLevel() const;
    
    // Log methods - make them virtual so they can be mocked
    virtual void log(LogLevel level, const char* message);
    virtual void logError(const char* message);
    virtual void logWarning(const char* message);
    virtual void logInfo(const char* message);
    virtual void logDebug(const char* message);
    
    // Display methods - make them virtual so they can be mocked
    virtual bool initialize();
    virtual void showHeadline(const char* text);
    virtual void showCalibrationMessage(const char* message1, const char* message2);
    virtual void showReadings(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure);
};

#endif // DISPLAY_MANAGER_H