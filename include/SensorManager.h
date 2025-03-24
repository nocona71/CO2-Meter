#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "SensorInterface.h"
#include "Logger.h"
#include <SparkFun_SCD30_Arduino_Library.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include "config.h" // Include config.h for centralized constants

class DisplayManager; // Forward declaration

class SensorManager {
private:
    CO2SensorInterface* co2Sensor;
    PressureSensorInterface* pressureSensor;
    SCD30 scd30; // Declare the SCD30 object
    Adafruit_BMP280 bmp280; // Declare bmp280 object
    
    // Last valid readings
    float lastValidCO2 = 400.0f;
    float lastValidTempSCD = 20.0f;
    float lastValidHumidity = 50.0f;

public:
    // Default constructor for testing
    SensorManager() : co2Sensor(nullptr), pressureSensor(nullptr) {}
    
    // Constructor with sensor interfaces
    SensorManager(CO2SensorInterface* co2, PressureSensorInterface* pressure) 
        : co2Sensor(co2), pressureSensor(pressure) {}
    
    bool initializeSensors();
    void checkAndCalibrate(DisplayManager& display);
    void calibrate(DisplayManager& display);
    float getCO2();
    float getTemperatureSCD();
    float getHumidity();
    float getTemperatureBMP();
    float getPressure();
    void resetCalibrationFlag();
    bool isDataAvailable();
    void calibrateSCD30();
    void checkAndCalibrateSCD30();

    // New method for running the I2C scanner
    void runI2CScanner();
};

#endif // SENSOR_MANAGER_H