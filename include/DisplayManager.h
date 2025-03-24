#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Adafruit_SSD1306.h>
#include "Logger.h"

#define SCREEN_WIDTH 128 ///< Width of the OLED display in pixels
#define SCREEN_HEIGHT 64 ///< Height of the OLED display in pixels
#define OLED_RESET -1    ///< OLED reset pin (-1 if not used)
#define SCREEN_ADDRESS 0x3C ///< I2C address of the OLED display
#define FONT_SIZE_SMALL 1   ///< Font size for small text
#define FONT_SIZE_LARGE 2   ///< Font size for large text
// Don't redefine these if they come from libraries
#ifndef SSD1306_SWITCHCAPVCC
#define SSD1306_SWITCHCAPVCC 0x02
#endif

#ifndef SSD1306_WHITE
#define SSD1306_WHITE 1
#endif


/**
 * @class DisplayManager
 * @brief Manages the OLED display for showing messages, warnings, and sensor readings.
 */
class DisplayManager {
public:
    /**
     * @brief Constructor for the DisplayManager class.
     */
    DisplayManager();

    /**
     * @brief Initializes the OLED display.
     * 
     * @return true if the display was successfully initialized, false otherwise.
     */
    bool initialize();

    /**
     * @brief Displays a calibration message on the screen.
     * 
     * @param message1 The first line of the calibration message.
     * @param message2 The second line of the calibration message.
     */
    void showCalibrationMessage(const char* message1, const char* message2);

    /**
     * @brief Displays a blinking warning message on the screen.
     * 
     * @param line1 The first line of the warning message.
     * @param line2 The second line of the warning message.
     * @param line3 The third line of the warning message.
     * @param line4 The fourth line of the warning message.
     * @param co2 The CO2 level to display.
     * @param temperatureSCD The temperature from the SCD30 sensor.
     * @param temperatureBMP The temperature from the BMP280 sensor.
     * @param humidity The humidity level to display.
     * @param pressure The pressure level to display.
     */
    void showBlinkingWarning(const char* line1, const char* line2, const char* line3, const char* line4, float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure);

    /**
     * @brief Displays the normal screen with sensor readings.
     * 
     * @param co2 The CO2 level to display.
     * @param temperatureSCD The temperature from the SCD30 sensor.
     * @param temperatureBMP The temperature from the BMP280 sensor.
     * @param humidity The humidity level to display.
     * @param pressure The pressure level to display.
     */
    void showNormalScreen(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure);

    /**
     * @brief Runs a display check to verify the OLED functionality.
     */
    void runDisplayCheck();

    /**
     * @brief Displays a splash screen with a centered message.
     * 
     * @param text The message to display on the splash screen.
     */
    void splashScreen(const char* text);

private:
    Adafruit_SSD1306 display; ///< The OLED display object.

    unsigned long lastBlinkTime = 0; ///< Timestamp of the last blink toggle.
    unsigned long blinkInterval = 1000; ///< Interval for blinking warnings (in milliseconds).
    bool isWarningActive = false; ///< Indicates whether the warning is currently active.

    /**
     * @brief Displays a headline centered at the top of the screen.
     * 
     * @param text The headline text to display.
     */
    void showHeadline(const char* text);

    /**
     * @brief Displays sensor readings on the screen.
     * 
     * @param co2 The CO2 level to display.
     * @param temperatureSCD The temperature from the SCD30 sensor.
     * @param temperatureBMP The temperature from the BMP280 sensor.
     * @param humidity The humidity level to display.
     * @param pressure The pressure level to display.
     */
    void displayReadings(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure);
};

#endif // DISPLAY_MANAGER_H