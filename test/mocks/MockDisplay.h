#ifndef MOCK_DISPLAY_H
#define MOCK_DISPLAY_H

#include <Arduino.h>

#define SSD1306_SWITCHCAPVCC 0x2

class MockDisplay {
public:
    bool beginCalled;
    bool clearCalled;
    bool displayCalled;
    String lastMessage;

    MockDisplay(); // Constructor

    bool begin(uint8_t switchvcc, uint8_t i2caddr);
    void clearDisplay();
    void display();
    void setCursor(int x, int y);
    void setTextSize(int size);
    void setTextColor(int color);
    void println(const char* text);
};

#endif // MOCK_DISPLAY_H