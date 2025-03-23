#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"

class DisplayManager {
private:
    Adafruit_SSD1306 display;

public:
    DisplayManager() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

    bool initialize() {
        if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
            return false;
        }
        display.clearDisplay();
        return true;
    }

    void showHeadline(const char* text) {
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

    void showCalibrationMessage(const char* message1, const char* message2) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.println(message1);
        display.println(message2);
        display.display();
    }

    void showReadings(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure) {
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
};

#endif // DISPLAY_MANAGER_H