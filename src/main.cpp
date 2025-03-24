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
#include <Arduino.h>
#include "I2CScanner.h"

/**
 * @file main.cpp
 * @brief Main entry point for the CO2 Meter application.
 * 
 * This file initializes the system, including the display, sensors, and logger.
 * It continuously retrieves sensor data, logs it, and displays warnings or normal readings
 * based on CO2 levels.
 */

/**
 * @brief Instance of the DisplayManager class for managing the OLED display.
 */
DisplayManager displayManager;

/**
 * @brief Instance of the SensorManager class for managing the SCD30 and BMP280 sensors.
 */
SensorManager sensorManager;

/**
 * @brief Instance of the I2CScanner class for scanning the I2C bus.
 */
I2CScanner i2cScanner;

/**
 * @brief Initializes the system, including the display, sensors, and logger.
 * 
 * This function sets up the serial communication, initializes the display and sensors,
 * and performs a calibration check for the SCD30 sensor. If any initialization fails,
 * the system halts.
 */
void setup() {
    Serial.begin(115200);
    while (!Serial);

    Logger::error("This is test error message.");    // Should always print
    Logger::warning("This test a warning message."); // Should not print if LOG_LEVEL=LOG_ERROR
    Logger::info("This is test info message.");      // Should not print if LOG_LEVEL=LOG_ERROR
    Logger::debug("This is test debug message.");    // Should not print if LOG_LEVEL=LOG_ERROR

    Logger::info("Initializing...");

#if RUN_I2C_SCANNER == TRUE
    i2cScanner.scan();
#endif

    // Initialize the display
    if (!displayManager.initialize()) {
        Logger::error("Display initialization failed!");
        for (;;); // Halt if display initialization fails
    }

#if RUN_DISPLAY_CHECK == TRUE
    displayManager.runDisplayCheck();
#endif

    displayManager.splashScreen("Initializing...");
    delay(2000);

    // Initialize sensors
    if (!sensorManager.initializeSensors()) {
        displayManager.splashScreen("Sensor init failed!");
        Logger::info("Sensor init failed!");
        for (;;); // Halt if sensor initialization fails
    }

    // Check and calibrate the SCD30 sensor
    sensorManager.checkAndCalibrateSCD30();
    Logger::info("Initialization complete.");
}

/**
 * @brief Main loop for retrieving sensor data and updating the display.
 * 
 * This function continuously retrieves sensor data, logs the readings, and updates the display.
 * It displays warnings if CO2 levels exceed predefined thresholds or normal readings otherwise.
 */
void loop() {
    Logger::debug("Entering loop...");

    if (sensorManager.isDataAvailable()) {
        Logger::debug("Sensor data is available.");

        // Retrieve sensor readings
        float co2 = sensorManager.getCO2();
        float temperatureSCD = sensorManager.getTemperatureSCD();
        float humidity = sensorManager.getHumidity();
        float temperatureBMP = sensorManager.getTemperatureBMP();
        float pressure = sensorManager.getPressure();

        Logger::debug("Sensor readings retrieved.");
        Logger::info(("CO2: " + String(co2) + " ppm").c_str());
        Logger::info(("Temperature (SCD30): " + String(temperatureSCD) + " °C").c_str());
        Logger::info(("Temperature (BMP280): " + String(temperatureBMP) + " °C").c_str());
        Logger::info(("Humidity: " + String(humidity) + " %").c_str());
        Logger::info(("Pressure: " + String(pressure) + " hPa").c_str());

        // Display warnings or normal readings based on CO2 levels
        if (co2 > CO2_CRITICAL_THRESHOLD) {
            Logger::warning("CRITICAL: High CO2 levels!");
            displayManager.showBlinkingWarning(
                "CRITICAL:", "High CO2", "levels!", "",
                co2, temperatureSCD, temperatureBMP, humidity, pressure);
        } else if (co2 > CO2_MODERATE_THRESHOLD) {
            Logger::warning("MODERATE: Elevated CO2 levels!");
            displayManager.showBlinkingWarning(
                "MODERATE:", "Elevated CO2", "levels!", "",
                co2, temperatureSCD, temperatureBMP, humidity, pressure);
        } else {
            Logger::info("Displaying normal readings.");
            displayManager.showNormalScreen(co2, temperatureSCD, temperatureBMP, humidity, pressure);
        }
    } else {
        Logger::debug("Sensor data not available.");
    }

    delay(1000); // Short delay for smoother blinking
}