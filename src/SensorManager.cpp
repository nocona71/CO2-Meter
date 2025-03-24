#include "SensorManager.h"
#include "config.h" // Include config.h for centralized constants
#include <EEPROM.h>
#include "Messages.h"
#include "Logger.h"
#include <Wire.h>

// Initialize sensors
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

// Check and calibrate SCD30
void SensorManager::checkAndCalibrateSCD30() {
    uint8_t calibrationFlag = EEPROM.read(EEPROM_CALIBRATION_FLAG_ADDRESS);

    if (calibrationFlag == CALIBRATION_DONE) {
        Logger::info(MSG_ALREADY_CALIBRATED);
    } else {
        Logger::info(MSG_CALIBRATION_NEEDED);
        calibrateSCD30();
        EEPROM.write(EEPROM_CALIBRATION_FLAG_ADDRESS, CALIBRATION_DONE);
        EEPROM.commit();
    }
}

// Calibrate SCD30
void SensorManager::calibrateSCD30() {
    Logger::info(MSG_CALIBRATING);
    Logger::info(MSG_PLACE_SENSOR);
    Logger::info(MSG_DO_NOT_BREATHE);

    if (scd30.setForcedRecalibrationFactor(FRESH_AIR_CO2)) {
        Logger::info(MSG_CALIBRATION_SUCCESS);
        Logger::info(MSG_CALIBRATION_READY);
    } else {
        Logger::error(MSG_CALIBRATION_FAILED);
        Logger::error(MSG_TRY_AGAIN);
    }

    delay(5000);
}

// Get CO2 reading
float SensorManager::getCO2() {
    return scd30.getCO2();
}

// Get temperature from SCD30
float SensorManager::getTemperatureSCD() {
    return scd30.getTemperature();
}

// Get humidity from SCD30
float SensorManager::getHumidity() {
    return scd30.getHumidity();
}

// Get temperature from BMP280
float SensorManager::getTemperatureBMP() {
    float temperatureBMP = bmp280.readTemperature();
    char tempBuffer[50];
    snprintf(tempBuffer, sizeof(tempBuffer), "BMP280 Temperature: %.2f °C", temperatureBMP);
    Logger::debug(tempBuffer);
    return temperatureBMP;
}

// Get pressure from BMP280
float SensorManager::getPressure() {
    float pressure = bmp280.readPressure() / 100.0F; // Convert Pa to hPa
    char pressureBuffer[50];
    snprintf(pressureBuffer, sizeof(pressureBuffer), "BMP280 Pressure: %.2f hPa", pressure);
    Logger::debug(pressureBuffer);
    return pressure;
}

// Reset calibration flag in EEPROM
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

// Check if data is available from SCD30
bool SensorManager::isDataAvailable() {
    bool available = scd30.dataAvailable();
    Logger::debug((String("Sensor data available: ") + (available ? "Yes" : "No")).c_str());
    return available;
}