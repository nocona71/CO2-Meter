#ifndef CONFIG_H
#define CONFIG_H

// Centralized configuration constants
#define EEPROM_CALIBRATION_FLAG_ADDRESS 0x10 // Example address in EEPROM
#define CALIBRATION_DONE 1
#define FRESH_AIR_CO2 400 // CO2 concentration in fresh air (ppm)

// Display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// Don't redefine these if they come from libraries
#ifndef SSD1306_SWITCHCAPVCC
#define SSD1306_SWITCHCAPVCC 0x02
#endif

#ifndef SSD1306_WHITE
#define SSD1306_WHITE 1
#endif

// Calibration settings
#define CO2_MODERATE_THRESHOLD 1000.0 // ppm
#define CO2_CRITICAL_THRESHOLD 2000.0  // ppm


#endif // CONFIG_H