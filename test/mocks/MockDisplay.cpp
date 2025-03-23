#include "MockDisplay.h"

// Constructor
MockDisplay::MockDisplay()
    : beginCalled(false), clearCalled(false), displayCalled(false), lastMessage("") {}

// Simulate display initialization
bool MockDisplay::begin(uint8_t switchvcc, uint8_t i2caddr) {
    if (switchvcc == SSD1306_SWITCHCAPVCC && i2caddr == 0x3C) {
        beginCalled = true;
        return true; // Simulate successful initialization
    }
    return false; // Simulate failure for invalid parameters
}

// Simulate clearing the display
void MockDisplay::clearDisplay() {
    clearCalled = true;
}

// Simulate updating the display
void MockDisplay::display() {
    displayCalled = true;
}

// Simulate setting the cursor position
void MockDisplay::setCursor(int x, int y) {
    // Simulate setting the cursor position
}

// Simulate setting the text size
void MockDisplay::setTextSize(int size) {
    // Simulate setting the text size
}

// Simulate setting the text color
void MockDisplay::setTextColor(int color) {
    // Simulate setting the text color
}

// Simulate printing a message
void MockDisplay::println(const char* text) {
    lastMessage = String(text);
}