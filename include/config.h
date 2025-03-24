#ifndef CONFIG_H
#define CONFIG_H

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

// EEPROM settings
#define EEPROM_CALIBRATION_FLAG_ADDRESS 0
#define CALIBRATION_DONE 123

// Calibration settings
#define FRESH_AIR_CO2 400
#define CO2_MODERATE_THRESHOLD 1000.0 // ppm
#define CO2_CRITICAL_THRESHOLD 2000.0  // ppm

#endif // CONFIG_H