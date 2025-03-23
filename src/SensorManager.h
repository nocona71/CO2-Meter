#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "SensorInterface.h"
#include "Logger.h"

class DisplayManager; // Forward declaration

class SensorManager {
private:
    CO2SensorInterface* co2Sensor;
    PressureSensorInterface* pressureSensor;
    
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
};

#endif // SENSOR_MANAGER_H