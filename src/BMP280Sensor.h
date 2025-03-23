#ifndef BMP280_SENSOR_H
#define BMP280_SENSOR_H

#include "SensorInterface.h"
#include <Adafruit_BMP280.h>

class BMP280Sensor : public PressureSensorInterface {
private:
    Adafruit_BMP280 sensor;
    
public:
    bool begin(uint8_t addr) override {
        return sensor.begin(addr);
    }
    
    float readTemperature() override {
        return sensor.readTemperature();
    }
    
    float readPressure() override {
        return sensor.readPressure();
    }
};

#endif // BMP280_SENSOR_H