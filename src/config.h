#ifndef CONFIG_H
#define CONFIG_H

#include "Messages.h"

// Other configuration constants (non-string constants can stay here)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define EEPROM_CALIBRATION_FLAG_ADDRESS 0
#define CALIBRATION_DONE 123

#define FRESH_AIR_CO2 400

// Add any other non-string constants you need

#endif // CONFIG_H