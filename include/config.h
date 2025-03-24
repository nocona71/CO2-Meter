#ifndef CONFIG_H
#define CONFIG_H

// Centralized configuration constants
#define EEPROM_CALIBRATION_FLAG_ADDRESS 0x10 // Example address in EEPROM
#define CALIBRATION_DONE 1
#define FRESH_AIR_CO2 400 // CO2 concentration in fresh air (ppm)

// Display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// Calibration settings
#define CO2_MODERATE_THRESHOLD 1000.0 // ppm
#define CO2_CRITICAL_THRESHOLD 2000.0 // ppm

// Default sensor readings
#define DEFAULT_CO2 400.0f ///< Default CO2 concentration in ppm
#define DEFAULT_TEMP_SCD 20.0f ///< Default temperature from SCD30 in °C
#define DEFAULT_HUMIDITY 50.0f ///< Default humidity in %

// Message strings
#define MSG_CALIBRATION_READY "Calibration ready."
#define MSG_CALIBRATION_FAILED "Calibration failed."
#define MSG_TRY_AGAIN "Please try again."
#define MSG_ALREADY_CALIBRATED "Sensor is already calibrated."
#define MSG_CALIBRATION_NEEDED "Calibration is needed."

#endif // CONFIG_H