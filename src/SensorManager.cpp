#include "SensorManager.h"
#include "config.h" // Include config.h for centralized constants
#include <EEPROM.h>
#include "Logger.h"
#include <Wire.h>

/**
 * @file SensorManager.cpp
 * @brief Implements the functionality for managing the SCD30 and BMP280 sensors.
 */

/**
 * @brief Initializes the SCD30 and BMP280 sensors.
 * 
 * @return `true` if both sensors are successfully initialized, `false` otherwise.
 */
bool SensorManager::initializeSensors() {
    Logger::info("Initializing SCD30 sensor...");
    if (!scd30.begin()) {
        Logger::error("Failed to initialize SCD30 sensor.");
        return false;
    }
    Logger::info("SCD30 sensor initialized successfully.");

    Logger::info("Initializing BMP280 sensor...");
    if (!bmp280.begin(0x76)) { // Explicitly specify the I2C address
        Logger::error("Failed to initialize BMP280 sensor.");
        return false;
    }
    Logger::info("BMP280 sensor initialized successfully.");

    return true;
}

/**
 * @brief Checks if the SCD30 sensor needs calibration and performs calibration if necessary.
 * 
 * This method checks the EEPROM for a calibration flag. If calibration has not been performed,
 * it calibrates the SCD30 sensor and updates the EEPROM.
 */
void SensorManager::checkAndCalibrateSCD30() {
    // Check if calibration has already been performed using EEPROM
    if (EEPROM.read(EEPROM_CALIBRATION_FLAG_ADDRESS) == CALIBRATION_DONE) {
        Logger::info(MSG_ALREADY_CALIBRATED);
    } else {
        Logger::info(MSG_CALIBRATION_NEEDED);
        calibrateSCD30();

        // Mark calibration as done in EEPROM
        EEPROM.write(EEPROM_CALIBRATION_FLAG_ADDRESS, CALIBRATION_DONE);
        EEPROM.commit();
    }
}

/**
 * @brief Calibrates the SCD30 sensor using the forced recalibration factor.
 */
void SensorManager::calibrateSCD30() {
    if (scd30.setForcedRecalibrationFactor(FRESH_AIR_CO2)) {
        Logger::info(MSG_CALIBRATION_READY);
    } else {
        Logger::error(MSG_CALIBRATION_FAILED);
        Logger::error(MSG_TRY_AGAIN);
    }
}

/**
 * @brief Retrieves the current CO2 reading from the SCD30 sensor.
 * 
 * @return The CO2 concentration in ppm.
 */
float SensorManager::getCO2() {
    return scd30.getCO2();
}

/**
 * @brief Retrieves the current temperature reading from the SCD30 sensor.
 * 
 * @return The temperature in °C.
 */
float SensorManager::getTemperatureSCD() {
    return scd30.getTemperature();
}

/**
 * @brief Retrieves the current humidity reading from the SCD30 sensor.
 * 
 * @return The humidity in %.
 */
float SensorManager::getHumidity() {
    return scd30.getHumidity();
}

/**
 * @brief Retrieves the current temperature reading from the BMP280 sensor.
 * 
 * @return The temperature in °C.
 */
float SensorManager::getTemperatureBMP() {
    float temperatureBMP = bmp280.readTemperature();
    char tempBuffer[50];
    snprintf(tempBuffer, sizeof(tempBuffer), "BMP280 Temperature: %.2f °C", temperatureBMP);
    Logger::debug(tempBuffer);
    return temperatureBMP;
}

/**
 * @brief Retrieves the current pressure reading from the BMP280 sensor.
 * 
 * @return The pressure in hPa.
 */
float SensorManager::getPressure() {
    float pressure = bmp280.readPressure() / 100.0F; // Convert Pa to hPa
    char pressureBuffer[50];
    snprintf(pressureBuffer, sizeof(pressureBuffer), "BMP280 Pressure: %.2f hPa", pressure);
    Logger::debug(pressureBuffer);
    return pressure;
}

/**
 * @brief Resets the calibration flag stored in EEPROM.
 * 
 * This method resets the calibration flag in EEPROM to indicate that calibration
 * needs to be performed again.
 */
void SensorManager::resetCalibrationFlag() {
    Logger::debug("Resetting calibration flag in EEPROM...");
    EEPROM.write(EEPROM_CALIBRATION_FLAG_ADDRESS, 0);
    bool commitSuccess = EEPROM.commit();

    if (commitSuccess) {
        Logger::debug("EEPROM reset successful");
    } else {
        Logger::error("EEPROM reset failed");
    }
}

/**
 * @brief Checks if new data is available from the SCD30 sensor.
 * 
 * @return `true` if new data is available, `false` otherwise.
 */
bool SensorManager::isDataAvailable() {
    bool available = scd30.dataAvailable();
    Logger::debug((String("Sensor data available: ") + (available ? "Yes" : "No")).c_str());
    return available;
}