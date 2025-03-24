#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>
#include <SparkFun_SCD30_Arduino_Library.h>
#include <EEPROM.h> // Include EEPROM library
#include "config.h" // Include the configuration file
#include "DisplayManager.h"
#include "SensorManager.h"
#include "Logger.h"
#include "Messages.h" // Include Messages.h

// Create instances of DisplayManager and SensorManager
DisplayManager displayManager;
SensorManager sensorManager;

void setup() {
    Serial.begin(115200);
    while (!Serial);

    Logger::info("Initializing...");

#if RUN_I2C_SCANNER == TRUE
    sensorManager.runI2CScanner();
#endif

    // Initialize the display
    if (!displayManager.initialize()) {
        Logger::error("Display initialization failed!");
        for (;;); // Halt if display initialization fails
    }

#if RUN_DISPLAY_CHECK == TRUE
    displayManager.runDisplayCheck();
#endif

    displayManager.centerHeadline("Initializing...");
    delay(2000);

    if (!sensorManager.initializeSensors()) {
        Logger::error("Sensor initialization failed!");
        for (;;); // Halt if sensor initialization fails
    }

    displayManager.centerHeadline("CO2 Meter"); // Set the headline to "CO2 Meter"
    Logger::info("Initialization complete.");
}

void loop() {
    if (sensorManager.isDataAvailable()) {
        float co2 = sensorManager.getCO2();
        float temperatureSCD = sensorManager.getTemperatureSCD();
        float temperatureBMP = sensorManager.getTemperatureBMP();
        float humidity = sensorManager.getHumidity();
        float pressure = sensorManager.getPressure();

        displayManager.showNormalScreen(co2, temperatureSCD, temperatureBMP, humidity, pressure);
        Logger::info("Displaying normal readings.");
    }

    delay(2000); // Adjust the delay as needed
}