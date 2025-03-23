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

// Create an instance of the display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Create instances for BMP280 and SCD30
Adafruit_BMP280 bmp;
SCD30 scd30;

// Cached readings
float lastCO2 = 0;
float lastTemperatureSCD = 0;
float lastTemperatureBMP = 0;
float lastHumidity = 0;
float lastPressure = 0;

// Function prototypes
void displayReadings(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure);
void calibrateSCD30();
void checkAndCalibrateSCD30();
void centerHeadline(const char* text);

DisplayManager displayManager;
SensorManager sensorManager;

void setup() {
    Serial.begin(115200);
    while (!Serial);
    
    // Initialize EEPROM
    EEPROM.begin(512);  // Size depends on your needs, 512 is usually plenty
    
    // Set log level to DEBUG
    Logger::setLogLevel(Logger::DBG);
    
    // Print the current log level
    Serial.print("Current log level: ");
    Serial.println(Logger::getLogLevel());
    
    // Try logging at different levels
    Logger::debug("DEBUG message");
    Logger::info("INFO message");
    Logger::warning("WARNING message");
    Logger::error("ERROR message");
    
    // ...rest of setup...

    Logger::info("Initializing...");

    if (!displayManager.initialize()) {
        // No need to log here, DisplayManager already logs errors
        for (;;);
    }

    displayManager.showHeadline("Initializing...");
    delay(2000);

    if (!sensorManager.initializeSensors()) {
        displayManager.showHeadline("Sensor init failed!");
        displayManager.logError("Sensor initialization failed!");
        for (;;);
    }

    sensorManager.checkAndCalibrate(displayManager);
    displayManager.logInfo("Initialization complete.");
}

void loop() {
    // Only get new readings if data is available
    if (sensorManager.isDataAvailable()) {
        float co2 = sensorManager.getCO2();
        float temperatureSCD = sensorManager.getTemperatureSCD();
        float humidity = sensorManager.getHumidity();
        
        // These readings are from a different sensor
        float temperatureBMP = sensorManager.getTemperatureBMP();
        float pressure = sensorManager.getPressure();
        
        displayManager.showReadings(co2, temperatureSCD, temperatureBMP, humidity, pressure);
        Logger::info("Readings updated.");
    }
    
    delay(2000); // Still delay to avoid too frequent checks
}

void checkAndCalibrateSCD30() {
  // Read the calibration flag from EEPROM
  uint8_t calibrationFlag = EEPROM.read(EEPROM_CALIBRATION_FLAG_ADDRESS);

  if (calibrationFlag == CALIBRATION_DONE) {
    // Calibration has already been performed
    Serial.println(MSG_ALREADY_CALIBRATED);
    display.clearDisplay();
    display.setCursor(0, 0);
    centerHeadline(MSG_CO2_MONITOR); // Display the centered headline
    display.display();
    delay(2000); // Briefly show the calibration status
  } else {
    // Calibration has not been performed
    Serial.println(MSG_CALIBRATION_NEEDED);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Calibration check...");
    display.println(MSG_CALIBRATION_NEEDED);
    display.display();
    delay(3000); // Wait for the user to read the message

    // Perform calibration
    calibrateSCD30();

    // Mark calibration as done in EEPROM
    EEPROM.write(EEPROM_CALIBRATION_FLAG_ADDRESS, CALIBRATION_DONE);
    EEPROM.commit(); // Commit changes to EEPROM
  }

  // After calibration check, display the centered headline
  display.clearDisplay();
  centerHeadline(MSG_CO2_MONITOR); // Display the centered headline
  display.display();
}

void centerHeadline(const char* text) {
  // Calculate the width of the text
  int16_t x1, y1;
  uint16_t textWidth, textHeight;
  display.getTextBounds(text, 0, 0, &x1, &y1, &textWidth, &textHeight);

  // Calculate the x-coordinate to center the text
  int16_t x = (SCREEN_WIDTH - textWidth) / 2;
  int16_t y = 0; // Top of the display

  // Set the cursor and print the text
  display.setCursor(x, y);
  display.println(text);
}

void calibrateSCD30() {
  // Clear the display and inform the user
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(MSG_CALIBRATING);
  display.println(MSG_PLACE_SENSOR);
  display.println(MSG_DO_NOT_BREATHE);
  display.display();

  Serial.println(MSG_CALIBRATING);
  Serial.println(MSG_PLACE_SENSOR);
  Serial.println(MSG_DO_NOT_BREATHE);

  // Perform Forced Recalibration (FRC) to 400 ppm (fresh air)
  if (scd30.setForcedRecalibrationFactor(FRESH_AIR_CO2)) {
    // Calibration successful
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(MSG_CALIBRATION_SUCCESS);
    display.println(MSG_CALIBRATION_READY);
    display.display();

    Serial.println(MSG_CALIBRATION_SUCCESS);
    Serial.println(MSG_CALIBRATION_READY);
  } else {
    // Calibration failed
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(MSG_CALIBRATION_FAILED);
    display.println(MSG_TRY_AGAIN);
    display.display();

    Serial.println(MSG_CALIBRATION_FAILED);
    Serial.println(MSG_TRY_AGAIN);
  }

  // Wait for a moment to let the user read the message
  delay(5000);
}

void displayReadings(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure) {
  // Clear the body section (below the headline or warning)
  display.fillRect(0, 16, SCREEN_WIDTH, SCREEN_HEIGHT - 16, SSD1306_BLACK);

  // Helper function to align values
  auto printAligned = [&](const char *label, float value, const char *unit, int row) {
    char valueBuffer[8];
    snprintf(valueBuffer, sizeof(valueBuffer), "%.2f", value); // Format value with 2 decimals

    // Calculate the width of the value and unit
    char fullValue[16];
    snprintf(fullValue, sizeof(fullValue), "%s %s", valueBuffer, unit);
    int16_t x1, y1;
    uint16_t fullValueWidth, textHeight;
    display.getTextBounds(fullValue, 0, 0, &x1, &y1, &fullValueWidth, &textHeight);

    // Print label on the left
    display.setCursor(0, row);
    display.print(label);

    // Print value and unit aligned to the right edge
    display.setCursor(SCREEN_WIDTH - fullValueWidth, row);
    display.print(fullValue);
  };

  // Adjust rows to fit measurements at the bottom
  int baseRow = 16; // Start below the headline or warning
  int rowSpacing = 10; // Adjust spacing to avoid overlap

  // Display CO2
  printAligned("CO2:", co2, "ppm", baseRow);

  // Display Temperature (SCD30)
  printAligned("T (SCD30):", temperatureSCD, "C", baseRow + rowSpacing);

  // Display Temperature (BMP280)
  printAligned("T (BMP280):", temperatureBMP, "C", baseRow + 2 * rowSpacing);

  // Display Humidity
  printAligned("Humidity:", humidity, "%", baseRow + 3 * rowSpacing);

  // Display Pressure
  printAligned("Pressure:", pressure, "hPa", baseRow + 4 * rowSpacing);

  // Update the display
  display.display();
  Serial.println("### Display updated"); // Debugging statement
}