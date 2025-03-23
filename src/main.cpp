#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>
#include <SparkFun_SCD30_Arduino_Library.h>
#include <EEPROM.h> // Include EEPROM library

// Define display width, height, and I2C address
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // Reset pin (not used with I2C)
#define SCREEN_ADDRESS 0x3C  // Detected I2C address

// Create an instance of the display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Create instances for BMP280 and SCD30
Adafruit_BMP280 bmp;
SCD30 scd30;

// CO2 thresholds
const int CO2_LEVEL_1 = 1000;  // Normal level threshold
const int CO2_LEVEL_2 = 2000;  // Moderate warning threshold

// Timing variables
unsigned long lastWarningTime = 0;

// Cached readings
float lastCO2 = 0;
float lastTemperatureSCD = 0;
float lastTemperatureBMP = 0;
float lastHumidity = 0;
float lastPressure = 0;

// Function prototypes
void displayCriticalWarning(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure);
void displayModerateWarning(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure);
void displayNormal(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure);
void displayReadings(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure);
void calibrateSCD30();
void checkFRCStatus();

bool warningVisible = true; // Flag to toggle warning visibility

#define EEPROM_CALIBRATION_FLAG_ADDRESS 0 // Address in EEPROM to store the calibration flag
#define CALIBRATION_DONE 1                // Value indicating calibration is done
#define CALIBRATION_NOT_DONE 0            // Value indicating calibration is not done

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for Serial Monitor to open
  Serial.println("Initializing sensors...");

  // Initialize EEPROM
  EEPROM.begin(512); // Initialize EEPROM with 512 bytes of storage

  // Initialize the display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);  // Don't proceed, loop forever
  }
  display.clearDisplay();

  // Initialize BMP280
  if (!bmp.begin(0x76)) { // Default I2C address for BMP280
    Serial.println("BMP280 initialization failed");
    for (;;);  // Don't proceed, loop forever
  }

  // Initialize SCD30
  if (!scd30.begin()) {
    Serial.println("SCD30 initialization failed");
    for (;;);  // Don't proceed, loop forever
  }

  // Check FRC status and calibrate if necessary
  checkFRCStatus();

  // Display initial readings (default values)
  displayNormal(lastCO2, lastTemperatureSCD, lastTemperatureBMP, lastHumidity, lastPressure);

  Serial.println("Initialization complete");
}

void loop() {
  // Read pressure from BMP280
  float pressure = bmp.readPressure() / 100.0F; // Convert to hPa

  // Pass pressure to SCD30 for compensation
  scd30.setAmbientPressure(pressure);

  // Check if new data is available from SCD30
  if (scd30.dataAvailable()) {
    lastCO2 = scd30.getCO2();
    lastTemperatureSCD = scd30.getTemperature();
    lastHumidity = scd30.getHumidity();
    lastTemperatureBMP = bmp.readTemperature();
    lastPressure = pressure;

    // Check CO2 levels and display warnings if necessary
    if (lastCO2 > CO2_LEVEL_2) {
      // Level 3: Critical Warning
      displayCriticalWarning(lastCO2, lastTemperatureSCD, lastTemperatureBMP, lastHumidity, lastPressure);
    } else if (lastCO2 > CO2_LEVEL_1) {
      // Level 2: Moderate Warning
      displayModerateWarning(lastCO2, lastTemperatureSCD, lastTemperatureBMP, lastHumidity, lastPressure);
    } else {
      // Normal operation
      displayNormal(lastCO2, lastTemperatureSCD, lastTemperatureBMP, lastHumidity, lastPressure);
    }

    // Print to Serial Monitor
    Serial.printf("CO2: %.2f ppm\n", lastCO2);
    Serial.printf("T (SCD30): %.2f C\n", lastTemperatureSCD);
    Serial.printf("T (BMP280): %.2f C\n", lastTemperatureBMP);
    Serial.printf("Humidity: %.2f %%\n", lastHumidity);
    Serial.printf("Pressure: %.2f hPa\n\n", lastPressure);
  } else {
    // Use cached readings if no new data is available
    displayNormal(lastCO2, lastTemperatureSCD, lastTemperatureBMP, lastHumidity, lastPressure);
  }

  delay(2000); // Wait 2 seconds before the next reading
}

void displayCriticalWarning(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure) {
  // Clear the top line for the warning message
  display.fillRect(0, 0, SCREEN_WIDTH, 16, SSD1306_BLACK); // Clear the top space
  display.setTextColor(SSD1306_WHITE); // White text on black background
  display.setTextSize(1);

  // Center the warning text horizontally and vertically
  int16_t x1, y1;
  uint16_t textWidth, textHeight;
  const char *warningMessage = "CRITICAL!";
  display.getTextBounds(warningMessage, 0, 0, &x1, &y1, &textWidth, &textHeight);
  int x = (SCREEN_WIDTH - textWidth) / 2;
  int y = (16 - textHeight) / 2; // Center vertically in the top 16 pixels
  display.setCursor(x, y);
  display.print(warningMessage);

  // Update the display
  display.display();

  // Display the readings in the body section
  displayReadings(co2, temperatureSCD, temperatureBMP, humidity, pressure);
}

void displayModerateWarning(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure) {
  // Clear the top line for the warning message
  display.fillRect(0, 0, SCREEN_WIDTH, 16, SSD1306_BLACK); // Clear the top space
  display.setTextColor(SSD1306_WHITE); // White text on black background
  display.setTextSize(1);

  const char *warningMessage = "MODERATE!";
  int16_t x1, y1;
  uint16_t textWidth, textHeight;
  display.getTextBounds(warningMessage, 0, 0, &x1, &y1, &textWidth, &textHeight);
  int x = (SCREEN_WIDTH - textWidth) / 2;
  int y = (16 - textHeight) / 2;
  display.setCursor(x, y);
  display.print(warningMessage);

  // Update the display
  display.display();

  // Display the readings in the body section
  displayReadings(co2, temperatureSCD, temperatureBMP, humidity, pressure);
}

void displayNormal(float co2, float temperatureSCD, float temperatureBMP, float humidity, float pressure) {
  Serial.println("### Entered displayNormal"); // Debugging statement
  Serial.printf("CO2: %.2f, T (SCD30): %.2f, T (BMP280): %.2f, Humidity: %.2f, Pressure: %.2f\n",
                co2, temperatureSCD, temperatureBMP, humidity, pressure);

  // Clear the top line for the headline
  display.fillRect(0, 0, SCREEN_WIDTH, 16, SSD1306_BLACK); // Clear the top space
  display.setTextColor(SSD1306_WHITE); // White text on black background
  display.setTextSize(1);

  // Display the centered headline
  const char *headline = "CO2 Monitor";
  int16_t x1, y1;
  uint16_t textWidth, textHeight;
  display.getTextBounds(headline, 0, 0, &x1, &y1, &textWidth, &textHeight);
  int x = (SCREEN_WIDTH - textWidth) / 2; // Center horizontally
  int y = (16 - textHeight) / 2; // Center vertically in the top 16 pixels
  display.setCursor(x, y);
  display.print(headline);

  // Update the display
  display.display();

  // Display the readings in the body section
  displayReadings(co2, temperatureSCD, temperatureBMP, humidity, pressure);
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

void calibrateSCD30() {
  // Clear the display and inform the user
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Calibrating SCD30...");
  display.println("Place sensor in fresh air.");
  display.println("Do not breathe near it.");
  display.display();

  Serial.println("Calibrating SCD30...");
  Serial.println("Place the sensor in fresh air and do not breathe near it.");

  // Show a progress bar for 10 seconds
  int calibrationDuration = 10000; // 10 seconds
  int progressBarWidth = SCREEN_WIDTH - 4; // Leave 2-pixel padding on each side
  int progressBarHeight = 10; // Height of the progress bar
  int progressBarX = 2; // X position (2-pixel padding)
  int progressBarY = SCREEN_HEIGHT - 12; // Y position (bottom of the screen)

  for (int i = 0; i <= calibrationDuration; i += 100) {
    // Draw the progress bar background
    display.fillRect(progressBarX, progressBarY, progressBarWidth, progressBarHeight, SSD1306_BLACK);
    display.drawRect(progressBarX, progressBarY, progressBarWidth, progressBarHeight, SSD1306_WHITE);

    // Fill the progress bar based on elapsed time
    int filledWidth = (i * progressBarWidth) / calibrationDuration;
    display.fillRect(progressBarX, progressBarY, filledWidth, progressBarHeight, SSD1306_WHITE);

    // Update the display
    display.display();

    // Wait 100 ms before updating the progress bar
    delay(100);
  }

  // Perform Forced Recalibration (FRC) to 400 ppm (fresh air)
  const uint16_t freshAirCO2 = 400; // CO2 concentration in fresh air
  if (scd30.setForcedRecalibrationFactor(freshAirCO2)) {
    // Calibration successful
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Calibration successful!");
    display.println("Sensor is ready.");
    display.display();

    Serial.println("Calibration successful! Sensor is ready.");
  } else {
    // Calibration failed
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Calibration failed!");
    display.println("Please try again.");
    display.display();

    Serial.println("Calibration failed! Please try again.");
  }

  // Wait for a moment to let the user read the message
  delay(5000);
}

void checkFRCStatus() {
  // Read the calibration flag from EEPROM
  uint8_t calibrationFlag = EEPROM.read(EEPROM_CALIBRATION_FLAG_ADDRESS);

  if (calibrationFlag == CALIBRATION_DONE) {
    // Calibration has already been performed
    Serial.println("Calibration already performed. Skipping...");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Calibration check...");
    display.println("Already calibrated.");
    display.display();
    delay(3000); // Wait for the user to read the message
  } else {
    // Calibration has not been performed
    Serial.println("Calibration not performed. Starting calibration...");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Calibration check...");
    display.println("Calibration needed.");
    display.display();
    delay(3000); // Wait for the user to read the message

    // Perform calibration
    calibrateSCD30();

    // Mark calibration as done in EEPROM
    EEPROM.write(EEPROM_CALIBRATION_FLAG_ADDRESS, CALIBRATION_DONE);
    EEPROM.commit(); // Commit changes to EEPROM
  }
}