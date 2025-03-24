#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Adafruit_SSD1306.h>
#include "Logger.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

class DisplayManager {
public:
    DisplayManager();

    // Initialize the display
    bool initialize();

    // Display methods
    void showCalibrationMessage(const char* message1, const char* message2);
    void showBlinkingWarning(const char* line1, const char* line2, const char* line3, const char* line4, float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure);
    void showNormalScreen(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure);
    void runDisplayCheck();
    void splashScreen(const char* text);

private:
    Adafruit_SSD1306 display;

    // Variables for blinking warning
    unsigned long lastBlinkTime = 0;
    unsigned long blinkInterval = 1000; // Default interval: 1 second
    bool isWarningActive = false;

    // Private method
    void showHeadline(const char* text);
    void displayReadings(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure);
};

#endif // DISPLAY_MANAGER_H