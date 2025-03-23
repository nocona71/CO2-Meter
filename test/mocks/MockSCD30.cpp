#include "MockSCD30.h"

// Constructor
MockSCD30::MockSCD30()
    : beginCalled(false), co2Value(450.0f), temperatureValue(22.5f), humidityValue(55.0f),
      calibrationCalled(false), calibrationSuccess(true) {}

// Simulate sensor initialization
bool MockSCD30::begin() {
    beginCalled = true;
    return true; // Simulate successful initialization
}

// Simulate getting CO2 value
float MockSCD30::getCO2() {
    return co2Value;
}

// Simulate getting temperature value
float MockSCD30::getTemperature() {
    return temperatureValue;
}

// Simulate getting humidity value
float MockSCD30::getHumidity() {
    return humidityValue;
}

// Simulate calibration
bool MockSCD30::calibrate(uint16_t freshAirCO2) {
    calibrationCalled = true;
    if (calibrationSuccess) {
        co2Value = freshAirCO2; // Simulate successful calibration
        return true;
    }
    return false; // Simulate calibration failure
}