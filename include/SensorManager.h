#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "Logger.h"
#include <SparkFun_SCD30_Arduino_Library.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include "config.h" // Include config.h for centralized constants

/**
 * @file SensorManager.h
 * @brief Manages the initialization, calibration, and data retrieval from sensors.
 */

/**
 * @class SensorManager
 * @brief A utility class for managing the SCD30 and BMP280 sensors.
 * 
 * The `SensorManager` class handles sensor initialization, calibration, and data retrieval
 * for CO2, temperature, humidity, and pressure readings.
 */
class SensorManager {
private:
    SCD30 scd30; ///< SCD30 CO2 sensor object
    Adafruit_BMP280 bmp280; ///< BMP280 pressure sensor object

    // Last valid readings
    float lastValidCO2 = DEFAULT_CO2; ///< Last valid CO2 reading in ppm
    float lastValidTempSCD = DEFAULT_TEMP_SCD; ///< Last valid temperature reading from SCD30 in °C
    float lastValidHumidity = DEFAULT_HUMIDITY; ///< Last valid humidity reading in %

public:
    /**
     * @brief Initializes the SCD30 and BMP280 sensors.
     * 
     * @return `true` if both sensors are successfully initialized, `false` otherwise.
     */
    bool initializeSensors();

    /**
     * @brief Retrieves the current CO2 reading from the SCD30 sensor.
     * 
     * @return The CO2 concentration in ppm.
     */
    float getCO2();

    /**
     * @brief Retrieves the current temperature reading from the SCD30 sensor.
     * 
     * @return The temperature in °C.
     */
    float getTemperatureSCD();

    /**
     * @brief Retrieves the current humidity reading from the SCD30 sensor.
     * 
     * @return The humidity in %.
     */
    float getHumidity();

    /**
     * @brief Retrieves the current temperature reading from the BMP280 sensor.
     * 
     * @return The temperature in °C.
     */
    float getTemperatureBMP();

    /**
     * @brief Retrieves the current pressure reading from the BMP280 sensor.
     * 
     * @return The pressure in hPa.
     */
    float getPressure();

    /**
     * @brief Resets the calibration flag stored in EEPROM.
     */
    void resetCalibrationFlag();

    /**
     * @brief Checks if new data is available from the sensors.
     * 
     * @return `true` if new data is available, `false` otherwise.
     */
    bool isDataAvailable();

    /**
     * @brief Calibrates the SCD30 sensor using the forced recalibration factor.
     */
    void calibrateSCD30();

    /**
     * @brief Checks if the SCD30 sensor needs calibration and performs calibration if necessary.
     */
    void checkAndCalibrateSCD30();
};

#endif // SENSOR_MANAGER_H