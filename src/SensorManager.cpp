#include "SensorManager.h"
#include "DisplayManager.h"
#include <EEPROM.h>
#include "Messages.h"

bool SensorManager::initializeSensors() {
    if (!scd30.begin()) {
        Logger::error("Failed to initialize SCD30 sensor!");
        return false;
    }
    Logger::info("SCD30 sensor initialized successfully.");
    return true;
}

void SensorManager::checkAndCalibrate(DisplayManager& display) {
    display.showHeadline(MSG_CO2_MONITOR);
    display.showCalibrationMessage("Calibration check...", MSG_CALIBRATION_NEEDED);
    // Debug EEPROM address
    String addrMsg = "Reading calibration flag from EEPROM address: " + String(EEPROM_CALIBRATION_FLAG_ADDRESS);
    Logger::debug(addrMsg.c_str());
    
    uint8_t calibrationFlag = EEPROM.read(EEPROM_CALIBRATION_FLAG_ADDRESS);
    
    // Log the current flag value and what we're comparing against
    String message = "Calibration flag value: " + String(calibrationFlag) + 
                     ", compared with CALIBRATION_DONE value: " + String(CALIBRATION_DONE);
    Logger::debug(message.c_str());

    if (calibrationFlag == CALIBRATION_DONE) {
        Logger::debug("Sensor already calibrated");
        display.showHeadline(MSG_CO2_MONITOR);
    } else {
        Logger::debug("Sensor needs calibration");
        display.showCalibrationMessage("Calibration check...", MSG_CALIBRATION_NEEDED);
        calibrate(display);
        
        // Write and commit to EEPROM with more debugging
        Logger::debug("Writing calibration flag to EEPROM...");
        EEPROM.write(EEPROM_CALIBRATION_FLAG_ADDRESS, CALIBRATION_DONE);
        
        Logger::debug("Committing EEPROM changes...");
        bool commitSuccess = EEPROM.commit();
        
        if (commitSuccess) {
            Logger::debug("EEPROM commit successful");
        } else {
            Logger::error("EEPROM commit failed");
        }
        
        // Verify the flag was written correctly
        uint8_t verifyFlag = EEPROM.read(EEPROM_CALIBRATION_FLAG_ADDRESS);
        String verifyMsg = "Verification: Calibration flag now: " + String(verifyFlag);
        Logger::debug(verifyMsg.c_str());
    }
}

void SensorManager::calibrate(DisplayManager& display) {
    Logger::debug("Starting sensor calibration");
    display.showCalibrationMessage(MSG_CALIBRATING, MSG_PLACE_SENSOR);
    
    // Create a String and convert to const char*
    String message = "Attempting forced recalibration with CO2 value: " + String(FRESH_AIR_CO2);
    Logger::debug(message.c_str());
    
    if (co2Sensor && co2Sensor->setForcedRecalibrationFactor(FRESH_AIR_CO2)) {
        display.showCalibrationMessage(MSG_CALIBRATION_SUCCESS, MSG_CALIBRATION_READY);
        Logger::info("Calibration successful!");
    } else {
        display.showCalibrationMessage(MSG_CALIBRATION_FAILED, MSG_TRY_AGAIN);
        Logger::error("Calibration failed!");
    }

    // Wait for a moment to let the user read the message
    delay(5000);

    // Reset the display to the normal headline
    display.showHeadline(MSG_CO2_MONITOR);
}

float SensorManager::getCO2() {
    return scd30.getCO2();
}

float SensorManager::getTemperatureSCD() {
    return scd30.getTemperature();
}

float SensorManager::getHumidity() {
    return scd30.getHumidity();
}

float SensorManager::getTemperatureBMP() {
    if (!pressureSensor) {
        return 20.0f; // Default room temperature
    }
    
    float value = pressureSensor->readTemperature();
    String message = "Pressure sensor temperature: " + String(value) + " °C";
    Logger::debug(message.c_str());
    return value;
}

float SensorManager::getPressure() {
    if (!pressureSensor) {
        return 1013.25f; // Standard atmospheric pressure at sea level
    }
    
    float value = pressureSensor->readPressure() / 100.0F; // Convert Pa to hPa
    String message = "Pressure: " + String(value) + " hPa";
    Logger::debug(message.c_str());
    return value;
}

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

bool SensorManager::isDataAvailable() {
    return scd30.dataAvailable();
}