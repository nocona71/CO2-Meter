#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Adafruit_BMP280.h>
#include <SparkFun_SCD30_Arduino_Library.h>
#include <EEPROM.h>
#include "config.h"
#include "Logger.h" // Include Logger for logging messages

class SensorManager {
private:
    Adafruit_BMP280 bmp;
    SCD30 scd30;

public:
    bool initializeSensors() {
        if (!bmp.begin(0x76)) {
            return false;
        }
        if (!scd30.begin()) {
            return false;
        }
        return true;
    }

    void checkAndCalibrate(DisplayManager& display) {
        uint8_t calibrationFlag = EEPROM.read(EEPROM_CALIBRATION_FLAG_ADDRESS);

        if (calibrationFlag == CALIBRATION_DONE) {
            display.showHeadline(MSG_CO2_MONITOR);
        } else {
            display.showCalibrationMessage("Calibration check...", MSG_CALIBRATION_NEEDED);
            calibrate(display);
            EEPROM.write(EEPROM_CALIBRATION_FLAG_ADDRESS, CALIBRATION_DONE);
            EEPROM.commit();
        }
    }

    void calibrate(DisplayManager& display) {
        display.showCalibrationMessage(MSG_CALIBRATING, MSG_PLACE_SENSOR);
        if (scd30.setForcedRecalibrationFactor(FRESH_AIR_CO2)) {
            display.showCalibrationMessage(MSG_CALIBRATION_SUCCESS, MSG_CALIBRATION_READY);
            Logger::log(Logger::INFO, "Calibration successful!");
        } else {
            display.showCalibrationMessage(MSG_CALIBRATION_FAILED, MSG_TRY_AGAIN);
            Logger::log(Logger::ERROR, "Calibration failed!");
        }

        // Wait for a moment to let the user read the message
        delay(5000);

        // Reset the display to the normal headline
        display.showHeadline(MSG_CO2_MONITOR);
    }

    float getCO2() { return scd30.getCO2(); }
    float getTemperatureSCD() { return scd30.getTemperature(); }
    float getHumidity() { return scd30.getHumidity(); }
    float getTemperatureBMP() { return bmp.readTemperature(); }
    float getPressure() { return bmp.readPressure() / 100.0F; }
};

#endif // SENSOR_MANAGER_H