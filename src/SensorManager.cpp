#include "SensorManager.h"
#include "DisplayManager.h"
#include <EEPROM.h>

bool SensorManager::initializeSensors() {
    Logger::debug("Initializing BMP280 sensor...");
    if (!bmp.begin(0x76)) {
        Logger::error("Failed to initialize BMP280 sensor!");
        return false;
    }
    Logger::debug("BMP280 sensor initialized successfully");
    
    Logger::debug("Initializing SCD30 sensor...");
    if (!scd30.begin()) {
        Logger::error("Failed to initialize SCD30 sensor!");
        return false;
    }
    Logger::debug("SCD30 sensor initialized successfully");
    
    return true;
}

void SensorManager::checkAndCalibrate(DisplayManager& display) {
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
    
    if (scd30.setForcedRecalibrationFactor(FRESH_AIR_CO2)) {
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
    float value = scd30.getCO2();
    
    // Create a String for logging
    String message = "CO2 reading: " + String(value) + " ppm";
    
    // Check if reading is valid (non-zero)
    if (value <= 0 || value > 10000) { // Add upper bound check for implausible values
        Logger::debug((message + " (invalid, using last valid: " + String(lastValidCO2) + ")").c_str());
        return lastValidCO2;
    }
    
    // Store the valid reading
    lastValidCO2 = value;
    Logger::debug(message.c_str());
    return value;
}

float SensorManager::getTemperatureSCD() {
    float value = scd30.getTemperature();
    
    // Create a String for logging
    String message = "SCD30 temperature: " + String(value) + " °C";
    
    // Check if reading is valid (non-zero and reasonable temperature range)
    if (value <= -40 || value > 85) { // SCD30 temperature range limits
        Logger::debug((message + " (invalid, using last valid: " + String(lastValidTempSCD) + ")").c_str());
        return lastValidTempSCD;
    }
    
    // Store the valid reading
    lastValidTempSCD = value;
    Logger::debug(message.c_str());
    return value;
}

float SensorManager::getHumidity() {
    float value = scd30.getHumidity();
    
    // Create a String for logging
    String message = "Humidity: " + String(value) + " %";
    
    // Check if reading is valid (non-zero and within reasonable range)
    if (value <= 0 || value > 100) { // Humidity limits
        Logger::debug((message + " (invalid, using last valid: " + String(lastValidHumidity) + ")").c_str());
        return lastValidHumidity;
    }
    
    // Store the valid reading
    lastValidHumidity = value;
    Logger::debug(message.c_str());
    return value;
}

float SensorManager::getTemperatureBMP() {
    float value = bmp.readTemperature();
    String message = "BMP280 temperature: " + String(value) + " °C";
    Logger::debug(message.c_str());
    return value;
}

float SensorManager::getPressure() {
    float value = bmp.readPressure() / 100.0F;
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
    bool dataAvailable = scd30.dataAvailable();
    
    // Log the data availability status
    if (dataAvailable) {
        Logger::debug("SCD30 data is available");
    } else {
        Logger::debug("SCD30 data is not available yet");
    }
    
    return dataAvailable;
}