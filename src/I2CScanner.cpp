#include "I2CScanner.h"

void I2CScanner::scan() {
    Logger::info("Starting I2C Scanner...");
    byte error, address;
    int devicesFound = 0;

    Wire.begin();
    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            char buffer[40]; // Increase buffer size to avoid truncation
            snprintf(buffer, sizeof(buffer), "I2C device found at address 0x%02X", address);
            Logger::info(buffer);
            devicesFound++;
        } else if (error == 4) {
            char buffer[40]; // Increase buffer size to avoid truncation
            snprintf(buffer, sizeof(buffer), "Unknown error at address 0x%02X", address);
            Logger::warning(buffer);
        }
    }

    if (devicesFound == 0) {
        Logger::info("No I2C devices found.");
    } else {
        Logger::info("I2C scan complete.");
    }
}