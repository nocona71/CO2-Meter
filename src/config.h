#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// Display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // Reset pin (not used with I2C)
#define SCREEN_ADDRESS 0x3C  // Detected I2C address

// EEPROM settings
#define EEPROM_CALIBRATION_FLAG_ADDRESS 0 // Address in EEPROM to store the calibration flag
#define CALIBRATION_DONE 1                // Value indicating calibration is done
#define CALIBRATION_NOT_DONE 0            // Value indicating calibration is not done

// CO2 thresholds
const int CO2_LEVEL_1 = 1000;  // Normal level threshold
const int CO2_LEVEL_2 = 2000;  // Moderate warning threshold

// Calibration settings
const uint16_t FRESH_AIR_CO2 = 400; // CO2 concentration in fresh air (ppm)
const int CALIBRATION_DURATION = 10000; // Calibration duration in milliseconds

// Progress bar settings
const int PROGRESS_BAR_PADDING = 2; // Padding for the progress bar
const int PROGRESS_BAR_HEIGHT = 10; // Height of the progress bar

// Display messages
const char* MSG_CALIBRATING = "Calibrating SCD30...";
const char* MSG_PLACE_SENSOR = "Place sensor in fresh air.";
const char* MSG_DO_NOT_BREATHE = "Do not breathe near it.";
const char* MSG_CALIBRATION_SUCCESS = "Calibration successful!";
const char* MSG_CALIBRATION_READY = "Sensor is ready.";
const char* MSG_CALIBRATION_FAILED = "Calibration failed!";
const char* MSG_TRY_AGAIN = "Please try again.";
const char* MSG_ALREADY_CALIBRATED = "Already calibrated.";
const char* MSG_CALIBRATION_NEEDED = "Calibration needed.";
const char* MSG_CRITICAL_WARNING = "CRITICAL!";
const char* MSG_MODERATE_WARNING = "MODERATE!";
const char* MSG_CO2_MONITOR = "CO2 Monitor";

#endif // CONFIG_H