#include "DisplayManager.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "Logger.h"

/**
 * @brief Constructs the DisplayManager object and initializes the display object.
 */
DisplayManager::DisplayManager()
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

/**
 * @brief Initializes the OLED display.
 * 
 * @return `true` if the display was successfully initialized, `false` otherwise.
 */
bool DisplayManager::initialize() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Logger::error("Display initialization failed");
        return false;
    }
    display.clearDisplay();
    Logger::info("Display initialized successfully");
    return true;
}

/**
 * @brief Displays a calibration message on the screen.
 * 
 * @param message1 The first line of the calibration message.
 * @param message2 The second line of the calibration message.
 */
void DisplayManager::showCalibrationMessage(const char* message1, const char* message2) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(FONT_SIZE_SMALL); // Use the constant for small font size
    display.setTextColor(SSD1306_WHITE);
    display.println(message1);
    display.println(message2);
    display.display();
}

/**
 * @brief Displays a blinking warning message or normal readings alternately.
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
void DisplayManager::showBlinkingWarning(const char* line1, const char* line2, const char* line3, const char* line4, float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure) {
    unsigned long currentTime = millis();

    // Check if it's time to toggle the warning display
    if (currentTime - lastBlinkTime >= blinkInterval) {
        lastBlinkTime = currentTime;
        isWarningActive = !isWarningActive; // Toggle the warning state
    }

    display.clearDisplay();

    if (isWarningActive) {
        // Display the warning with the largest font
        display.setTextSize(FONT_SIZE_LARGE); // Use the constant for large font size
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.println("WARNING!");

        // Display the 4 lines of the warning message
        display.setCursor(0, 16); // Move cursor below "WARNING!"
        display.println(line1);
        display.println(line2);
        display.println(line3);
        display.println(line4);
    } else {
        // Use the reusable method to display the normal screen
        showNormalScreen(co2, temperatureSCD, temperatureBMP, humidity, pressure);
    }

    display.display();
}

/**
 * @brief Displays a centered headline on the screen.
 * 
 * @param text The headline text to display.
 */
void DisplayManager::showHeadline(const char* text) {
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    int16_t x1, y1;
    uint16_t textWidth, textHeight;
    display.getTextBounds(text, 0, 0, &x1, &y1, &textWidth, &textHeight);
    int16_t x = (SCREEN_WIDTH - textWidth) / 2;
    display.setCursor(x, 0);
    display.println(text);
    display.display();
}

/**
 * @brief Displays the normal screen with sensor readings.
 * 
 * @param co2 The CO2 level to display.
 * @param temperatureSCD The temperature from the SCD30 sensor.
 * @param temperatureBMP The temperature from the BMP280 sensor.
 * @param humidity The humidity level to display.
 * @param pressure The pressure level to display.
 */
void DisplayManager::showNormalScreen(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure) {
    display.clearDisplay(); // Clear the entire display

    showHeadline("CO2 Meter");
    displayReadings(co2, temperatureSCD, temperatureBMP, humidity, pressure);
}

/**
 * @brief Displays sensor readings on the screen.
 * 
 * @param co2 The CO2 level to display.
 * @param temperatureSCD The temperature from the SCD30 sensor.
 * @param temperatureBMP The temperature from the BMP280 sensor.
 * @param humidity The humidity level to display.
 * @param pressure The pressure level to display.
 */
void DisplayManager::displayReadings(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure) {
    Logger::debug("Updating display with sensor readings...");
    display.setTextSize(FONT_SIZE_SMALL); // Small font size for readings

    // Use a lambda function to print aligned text without clearing the entire area
    auto printAligned = [&](const char* label, float value, const char* unit, int row) {
        char valueBuffer[8];
        snprintf(valueBuffer, sizeof(valueBuffer), "%.2f", value);
        char fullValue[16];
        snprintf(fullValue, sizeof(fullValue), "%s %s", valueBuffer, unit);

        // Calculate the width of the full value text
        int16_t x1, y1;
        uint16_t fullValueWidth, textHeight;
        display.getTextBounds(fullValue, 0, 0, &x1, &y1, &fullValueWidth, &textHeight);

        // Print the label on the left
        display.setCursor(0, row);
        display.setTextColor(SSD1306_WHITE, SSD1306_BLACK); // Overwrite previous text
        display.print(label);

        // Print the value on the right
        display.setCursor(SCREEN_WIDTH - fullValueWidth, row);
        display.print(fullValue);
    };

    int baseRow = 17; // Start below the headline
    int rowSpacing = 10;

    // Update only the readings area without clearing it
    printAligned("CO2:", co2, "ppm", baseRow);
    printAligned("T (SCD30):", temperatureSCD, "C", baseRow + rowSpacing);
    printAligned("T (BMP280):", temperatureBMP, "C", baseRow + 2 * rowSpacing);
    printAligned("Humidity:", humidity, "%", baseRow + 3 * rowSpacing);
    printAligned("Pressure:", pressure, "hPa", baseRow + 4 * rowSpacing);

    display.display(); // Update the display with the new content
    Logger::debug("Display updated.");
}

/**
 * @brief Runs a display check to verify the OLED functionality.
 */
void DisplayManager::runDisplayCheck() {
    Logger::info("Running display check...");

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Display Test");
    display.setCursor(0, 10);
    display.println("Line 2: Hello!");
    display.setCursor(0, 20);
    display.println("Line 3: Testing...");
    display.display();

    delay(5000); // Keep the test visible for 5 seconds
    Logger::info("Display check complete.");
}

/**
 * @brief Displays a splash screen with a centered message.
 * 
 * @param text The message to display on the splash screen.
 */
void DisplayManager::splashScreen(const char* text) {
    display.clearDisplay();
    display.setTextSize(FONT_SIZE_SMALL); // Use the constant for large font size
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);

    // Center the text on the screen
    int16_t x1, y1;
    uint16_t textWidth, textHeight;
    display.getTextBounds(text, 0, 0, &x1, &y1, &textWidth, &textHeight);
    int16_t x = (SCREEN_WIDTH - textWidth) / 2;
    int16_t y = (SCREEN_HEIGHT - textHeight) / 2;

    display.setCursor(x, y);
    display.println(text);
    display.display();
}