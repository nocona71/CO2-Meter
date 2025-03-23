#include "MockBMP280.h"

// Constructor
MockBMP280::MockBMP280() : beginCalled(false), temperatureValue(22.0f), pressureValue(101325.0f) {}

// Simulate sensor initialization
bool MockBMP280::begin(uint8_t addr) {
    beginCalled = true;
    return true; // Simulate successful initialization
}

// Simulate reading temperature
float MockBMP280::readTemperature() {
    return temperatureValue;
}

// Simulate reading pressure
float MockBMP280::readPressure() {
    return pressureValue;
}