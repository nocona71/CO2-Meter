#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>
#include <SparkFun_SCD30_Arduino_Library.h>
#include <EEPROM.h> // Include EEPROM library
#include "config.h" // Include the configuration file

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

  // Display initialization message
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.println("Please wait.");
  display.display();

  // Initialize BMP280
  if (!bmp.begin(0x76)) { // Default I2C address for BMP280
    Serial.println("BMP280 initialization failed");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("BMP280 failed!");
    display.display();
    for (;;);  // Don't proceed, loop forever
  }

  // Initialize SCD30
  if (!scd30.begin()) {
    Serial.println("SCD30 initialization failed");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("SCD30 failed!");
    display.display();
    for (;;);  // Don't proceed, loop forever
  }

  // Check and calibrate SCD30 if necessary
  checkAndCalibrateSCD30();

  // Display initial readings (default values)
  displayReadings(lastCO2, lastTemperatureSCD, lastTemperatureBMP, lastHumidity, lastPressure);

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

    // Display the readings
    displayReadings(lastCO2, lastTemperatureSCD, lastTemperatureBMP, lastHumidity, lastPressure);

    // Print to Serial Monitor
    Serial.printf("CO2: %.2f ppm\n", lastCO2);
    Serial.printf("T (SCD30): %.2f C\n", lastTemperatureSCD);
    Serial.printf("T (BMP280): %.2f C\n", lastTemperatureBMP);
    Serial.printf("Humidity: %.2f %%\n", lastHumidity);
    Serial.printf("Pressure: %.2f hPa\n\n", lastPressure);
  }

  delay(2000); // Wait 2 seconds before the next reading
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