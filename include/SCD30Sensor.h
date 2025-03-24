#ifndef SCD30_SENSOR_H
#define SCD30_SENSOR_H

#include "SensorInterface.h"
#include <SparkFun_SCD30_Arduino_Library.h>

class SCD30Sensor : public CO2SensorInterface {
private:
    SCD30 sensor;
    
public:
    bool begin() override {
        return sensor.begin();
    }
    
    bool dataAvailable() override {
        return sensor.dataAvailable();
    }
    
    float getCO2() override {
        return sensor.getCO2();
    }
    
    float getTemperature() override {
        return sensor.getTemperature();
    }
    
    float getHumidity() override {
        return sensor.getHumidity();
    }
    
    bool setForcedRecalibrationFactor(uint16_t concentration) override {
        return sensor.setForcedRecalibrationFactor(concentration);
    }
};

#endif // SCD30_SENSOR_H