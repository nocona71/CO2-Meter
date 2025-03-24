#include <Wire.h>
#include <Arduino.h>

void setup() {
    Serial.begin(115200); // Start serial communication at 115200 baud
    while (!Serial);      // Wait for the serial port to connect (for boards like Leonardo)

    Serial.println("\nI2C Scanner");
    Wire.begin();         // Initialize the I2C bus

    // Scan for I2C devices
    for (uint8_t address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        if (Wire.endTransmission() == 0) { // If the device responds
            Serial.print("I2C device found at address 0x");
            if (address < 16) Serial.print("0"); // Add leading zero for single-digit addresses
            Serial.println(address, HEX);
        }
    }

    Serial.println("Scan complete.");
}

void loop() {
    // Nothing to do in the loop
}