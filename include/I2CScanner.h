#ifndef I2CSCANNER_H
#define I2CSCANNER_H

#include <Wire.h>
#include "Logger.h"

class I2CScanner {
public:
    void scan(); // Method to scan I2C devices
};

#endif // I2CSCANNER_H