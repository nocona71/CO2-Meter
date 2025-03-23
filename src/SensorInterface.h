#ifndef SENSOR_INTERFACE_H
#define SENSOR_INTERFACE_H

#include <Arduino.h> // Provides uint8_t, uint16_t

class CO2SensorInterface {
public:
    virtual ~CO2SensorInterface() {}
    virtual bool begin() = 0;
    virtual bool dataAvailable() = 0;
    virtual float getCO2() = 0;
    virtual float getTemperature() = 0;
    virtual float getHumidity() = 0;
    virtual bool setForcedRecalibrationFactor(uint16_t concentration) = 0;
};

class PressureSensorInterface {
public:
    virtual ~PressureSensorInterface() {}
    virtual bool begin(uint8_t addr) = 0;
    virtual float readTemperature() = 0;
    virtual float readPressure() = 0;
};

#endif // SENSOR_INTERFACE_H