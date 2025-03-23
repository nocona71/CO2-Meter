#include "DisplayManager.h"

DisplayManager::DisplayManager() 
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
      currentLogLevel(LOG_INFO) {
}

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
}