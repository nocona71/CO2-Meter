#include "I2CScanner.h"

/**
 * @brief Scans the I2C bus for connected devices and logs their addresses.
 * 
 * This method iterates through all possible I2C addresses (1 to 127) and checks
 * if a device responds at each address. If a device is found, its address is logged.
 * If no devices are found, a message is logged indicating this.
 * 
 * @details
 * - The method uses the `Wire` library to communicate with devices on the I2C bus.
 * - For each address, it sends a transmission and checks for a response.
 * - If an error code of `0` is returned, a device is present at that address.
 * - If an error code of `4` is returned, an unknown error occurred.
 * 
 * @note The method logs the results using the `Logger` class.
 */
void I2CScanner::scan() {
    Logger::info("Starting I2C Scanner...");
    byte error, address;
    int devicesFound = 0;

    Wire.begin();
    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            char buffer[40]; // Buffer to store the log message
            snprintf(buffer, sizeof(buffer), "I2C device found at address 0x%02X", address);
            Logger::info(buffer);
            devicesFound++;
        } else if (error == 4) {
            char buffer[40]; // Buffer to store the log message
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