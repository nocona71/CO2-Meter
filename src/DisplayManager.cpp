#include "DisplayManager.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "Logger.h"

DisplayManager::DisplayManager() 
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
      currentLogLevel(LOG_INFO) {
}
^^
void DisplayManager::setLogLevel(LogLevel level) {
    currentLogLevel = level;
}

LogLevel DisplayManager::getLogLevel() const {
    return currentLogLevel;
}

void DisplayManager::log(LogLevel level, const char* message) {
    if (level <= currentLogLevel) {
        // You could display on screen, output to Serial, etc.
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

void DisplayManager::logError(const char* message) { 
    log(LOG_ERROR, message); 
}

void DisplayManager::logWarning(const char* message) { 
    log(LOG_WARNING, message); 
}

void DisplayManager::logInfo(const char* message) { 
    log(LOG_INFO, message); 
}

void DisplayManager::logDebug(const char* message) { 
    log(LOG_DEBUG, message); 
}

bool DisplayManager::initialize() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Logger::error("Display initialization failed");
        return false;
    }
    display.clearDisplay();
    Logger::info("Display initialized successfully");
    return true;
}

void DisplayManager::showHeadline(const char* text) {
    display.clearDisplay();
    int16_t x1, y1;
    uint16_t textWidth, textHeight;
    display.getTextBounds(text, 0, 0, &x1, &y1, &textWidth, &textHeight);
    int16_t x = (SCREEN_WIDTH - textWidth) / 2;
    display.setCursor(x, 0);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.println(text);
    display.display();
}

void DisplayManager::showCalibrationMessage(const char* message1, const char* message2) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.println(message1);
    display.println(message2);
    display.display();
}

void DisplayManager::showReadings(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure) {
    Logger::debug("Updating display with sensor readings...");

    display.fillRect(0, 16, SCREEN_WIDTH, SCREEN_HEIGHT - 16, SSD1306_BLACK);

    auto printAligned = [&](const char* label, float value, const char* unit, int row) {
        char valueBuffer[8];
        snprintf(valueBuffer, sizeof(valueBuffer), "%.2f", value);
        char fullValue[16];
        snprintf(fullValue, sizeof(fullValue), "%s %s", valueBuffer, unit);
        int16_t x1, y1;
        uint16_t fullValueWidth, textHeight;
        display.getTextBounds(fullValue, 0, 0, &x1, &y1, &fullValueWidth, &textHeight);
        display.setCursor(0, row);
        display.print(label);
        display.setCursor(SCREEN_WIDTH - fullValueWidth, row);
        display.print(fullValue);
    };

    int baseRow = 16;
    int rowSpacing = 10;

    printAligned("CO2:", co2, "ppm", baseRow);
    printAligned("T (SCD30):", temperatureSCD, "C", baseRow + rowSpacing);
    printAligned("T (BMP280):", temperatureBMP, "C", baseRow + 2 * rowSpacing);
    printAligned("Humidity:", humidity, "%", baseRow + 3 * rowSpacing);
    printAligned("Pressure:", pressure, "hPa", baseRow + 4 * rowSpacing);

    display.display();
    Logger::debug("Display updated.");
}

void DisplayManager::showWarning(const char* message) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("WARNING!");
    display.println(message);
    display.display();

    // Replace Serial.println with Logger
    String logMessage = String("Warning displayed: ") + message;
    Logger::warning(logMessage.c_str()); // Convert String to const char*
}

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
        display.setTextSize(2); // Use the largest font size
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

void DisplayManager::centerHeadline(const char* text) {
    // Clear only the headline area
    display.fillRect(0, 0, SCREEN_WIDTH, 16, SSD1306_BLACK);

    int16_t x1, y1;
    uint16_t textWidth, textHeight;

    display.setTextSize(2); // Use a larger font size for the headline
    display.getTextBounds(text, 0, 0, &x1, &y1, &textWidth, &textHeight);
    int16_t x = (SCREEN_WIDTH - textWidth) / 2;
    display.setCursor(x, 0);
    display.setTextColor(SSD1306_WHITE);
    display.println(text);
    display.display();
}

void DisplayManager::showNormalScreen(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure) {
    display.clearDisplay(); // Clear the entire display

    // Display the "CO2 Meter" heading in large font
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    centerHeadline("CO2 Meter");

    // Display the normal readings
    display.setTextSize(1); // Use smaller font for readings
    auto printAligned = [&](const char* label, float value, const char* unit, int row) {
        char valueBuffer[8];
        snprintf(valueBuffer, sizeof(valueBuffer), "%.2f", value);
        char fullValue[16];
        snprintf(fullValue, sizeof(fullValue), "%s %s", valueBuffer, unit);
        int16_t x1, y1;
        uint16_t fullValueWidth, textHeight;
        display.getTextBounds(fullValue, 0, 0, &x1, &y1, &fullValueWidth, &textHeight);
        display.setCursor(0, row);
        display.print(label);
        display.setCursor(SCREEN_WIDTH - fullValueWidth, row);
        display.print(fullValue);
    };

    int baseRow = 16; // Start below the heading
    int rowSpacing = 10;

    printAligned("CO2:", co2, "ppm", baseRow);
    printAligned("T (SCD30):", temperatureSCD, "C", baseRow + rowSpacing);
    printAligned("T (BMP280):", temperatureBMP, "C", baseRow + 2 * rowSpacing);
    printAligned("Humidity:", humidity, "%", baseRow + 3 * rowSpacing);
    printAligned("Pressure:", pressure, "hPa", baseRow + 4 * rowSpacing);

    display.display(); // Update the display with the new content
}

void DisplayManager::displayReadings(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure) {
    Logger::debug("Updating display with sensor readings...");

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

    int baseRow = 16; // Start below the headline
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
    display.clearDisplay();
    display.display();

    Logger::info("Display check complete.");
}