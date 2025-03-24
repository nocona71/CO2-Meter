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
    bool isWarningActive = false; // Tracks whether the warning is active
    unsigned long lastBlinkTime = 0; // Tracks the last time the warning blinked
    const unsigned long blinkInterval = 500; // Interval for blinking (in milliseconds)

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
    void showWarning(const char* message);
    void showBlinkingWarning(const char* line1, const char* line2, const char* line3, const char* line4, float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure);
    void centerHeadline(const char* text);
    void showNormalScreen(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure);
    void displayReadings(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure);
    void runDisplayCheck(); // New method for display check
};

#endif // DISPLAY_MANAGER_H