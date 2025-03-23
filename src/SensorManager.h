#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Adafruit_BMP280.h>
#include <SparkFun_SCD30_Arduino_Library.h>
#include "config.h"
#include "Logger.h"
#include "DisplayManager.h"

class SensorManager {
private:
    Adafruit_BMP280 bmp;
    SCD30 scd30;
    
    // Last valid readings
    float lastValidCO2 = 400.0;         // Start with fresh air assumption
    float lastValidTempSCD = 20.0;      // Start with room temperature assumption
    float lastValidHumidity = 50.0;     // Start with average humidity assumption

public:
    // Initialize the sensors
    bool initializeSensors();
    
    // Check if calibration is needed and perform it
    void checkAndCalibrate(DisplayManager& display);
    
    // Perform sensor calibration
    void calibrate(DisplayManager& display);
    
    // Get sensor readings
    float getCO2();
    float getTemperatureSCD();
    float getHumidity();
    float getTemperatureBMP();
    float getPressure();
    
    // Reset calibration flag in EEPROM
    void resetCalibrationFlag();

    // Check if data is available
    bool isDataAvailable();
};

#endif // SENSOR_MANAGER_H