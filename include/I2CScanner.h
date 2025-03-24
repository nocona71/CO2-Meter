#ifndef I2CSCANNER_H
#define I2CSCANNER_H

#include <Wire.h>
#include "Logger.h"

/**
 * @file I2CScanner.h
 * @brief Provides functionality to scan the I2C bus for connected devices.
 */

/**
 * @class I2CScanner
 * @brief A utility class for scanning the I2C bus and logging connected devices.
 */
class I2CScanner {
public:
    /**
     * @brief Scans the I2C bus for connected devices.
     * 
     * This method iterates through all possible I2C addresses (1 to 127) and checks
     * if a device responds at each address. If a device is found, its address is logged.
     * If no devices are found, a message is logged indicating this.
     */
    void scan();
};

#endif // I2CSCANNER_H