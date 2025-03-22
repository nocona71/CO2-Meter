#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>
#include <SparkFun_SCD30_Arduino_Library.h>

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

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for Serial Monitor to open
  Serial.println("Initializing sensors...");

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

  Serial.println("Initialization complete");
}

void loop() {
  // Read data from BMP280
  float temperatureBMP = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F; // Convert to hPa

  // Read data from SCD30
  if (scd30.dataAvailable()) {
    float co2 = scd30.getCO2();
    float temperatureSCD = scd30.getTemperature();
    float humidity = scd30.getHumidity();

    // Clear the display
    display.clearDisplay();

    // Center the title
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor((SCREEN_WIDTH - 72) / 2, 0); // Center "CO2 Meter"
    display.println("CO2 Meter");

    // Helper function to align values
    auto printAligned = [&](const char *label, float value, const char *unit, int row) {
      char valueBuffer[8];
      snprintf(valueBuffer, sizeof(valueBuffer), "%.2f", value); // Format value with 2 decimals

      // Calculate the width of the label
      int16_t x1, y1;
      uint16_t labelWidth, textHeight;
      display.getTextBounds(label, 0, 0, &x1, &y1, &labelWidth, &textHeight);

      // Calculate the width of the value and unit
      char fullValue[16];
      snprintf(fullValue, sizeof(fullValue), "%s %s", valueBuffer, unit);
      uint16_t valueWidth;
      display.getTextBounds(fullValue, 0, 0, &x1, &y1, &valueWidth, &textHeight);

      // Print label on the left
      display.setCursor(0, row);
      display.print(label);

      // Print value and unit aligned to the right of the label
      display.setCursor(SCREEN_WIDTH - valueWidth, row);
      display.print(fullValue);
    };

    // Display CO2
    printAligned("CO2:", co2, "ppm", 16);

    // Display Temperature (SCD30)
    printAligned("T (SCD30):", temperatureSCD, "C", 26);

    // Display Temperature (BMP280)
    printAligned("T (BMP280):", temperatureBMP, "C", 36);

    // Display Humidity
    printAligned("Humidity:", humidity, "%", 46);

    // Display Pressure
    printAligned("Pressure:", pressure, "hPa", 56);

    // Update the display
    display.display();

    // Print to Serial Monitor
    Serial.printf("CO2: %.2f ppm\n", co2);
    Serial.printf("T (SCD30): %.2f C\n", temperatureSCD);
    Serial.printf("T (BMP280): %.2f C\n", temperatureBMP);
    Serial.printf("Humidity: %.2f %%\n", humidity);
    Serial.printf("Pressure: %.2f hPa\n\n", pressure);
  }

  delay(2000); // Wait 2 seconds before the next reading
}