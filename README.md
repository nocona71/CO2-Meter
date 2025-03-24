# CO2 Meter

A project for monitoring CO2 levels, temperature, humidity, and pressure using the SCD30 and BMP280 sensors. The data is displayed on an OLED screen and logged for debugging and monitoring purposes.

---

## **Installation**

1. Clone this repository to your local machine:
```bash
git clone https://github.com/your-username/CO2-Meter.git
cd CO2-Meter
```

2. Install [PlatformIO](https://platformio.org/) if you haven’t already:
   - Install the PlatformIO extension in Visual Studio Code.
   - Alternatively, install it via the command line:
```bash
pip install platformio
```

3. Install `make`:
   - **Windows:**
     - Install [Git for Windows](https://gitforwindows.org/) to get Git Bash, which includes the `make` command.
     - Alternatively, install `make` via [MinGW](http://www.mingw.org/):
       1. Download and install MinGW.
       2. During installation, select the `mingw32-make` package.
       3. Add the MinGW `bin` directory (e.g., `C:\MinGW\bin`) to your system's PATH environment variable.
   - **Linux/Mac:**
     - Use your package manager:
```bash
sudo apt install make        # For Debian/Ubuntu
brew install make            # For macOS
```

4. Open the project in Visual Studio Code:
   ```bash
   code CO2-Meter
   ```

---

## **Requirements**

- **Hardware:**
  - ESP8266-based board (e.g., ESP-12E or NodeMCU).
  - SCD30 CO2 sensor.
  - BMP280 pressure sensor.
  - SSD1306 OLED display.
  - I2C connections for the sensors and display.

- **Software:**
  - [PlatformIO](https://platformio.org/) for building and deploying the firmware.
  - `make` for running the build and deployment tasks.
  - Required libraries (automatically installed via `platformio.ini`):
    - Adafruit GFX Library
    - Adafruit SSD1306
    - SparkFun SCD30 Arduino Library
    - Adafruit BMP280 Library

---

## **Building**

1. Open the project directory in a terminal:
   - **Windows:** Use Git Bash or a terminal where `make` is available.
   - **Linux/Mac:** Use your default terminal.

2. Build the project using the `Makefile`:
```bash
make build
```

---

## **Deployment**

1. Connect your ESP8266 board to your computer via USB.
2. Upload the firmware to the board:
```bash
make upload
```

3. Open the serial monitor to verify the output:
```bash
make monitor
```

---

## **Configuration**

The project uses a centralized configuration file (`config.h`) to define constants and settings. You can modify the following parameters to suit your needs:

### **Key Configuration Options**
#### **EEPROM Settings:**
```cpp
#define EEPROM_CALIBRATION_FLAG_ADDRESS 0x10
#define CALIBRATION_DONE 1
```

#### **CO2 Thresholds:**
```cpp
#define CO2_MODERATE_THRESHOLD 1000.0 // ppm
#define CO2_CRITICAL_THRESHOLD 2000.0 // ppm
```

#### **Default Sensor Readings:**
```cpp
#define DEFAULT_CO2 400.0f
#define DEFAULT_TEMP_SCD 20.0f
#define DEFAULT_HUMIDITY 50.0f
```

#### **Display Settings:**
```cpp
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
```

### **Modifying Configuration**
1. Open the `config.h` file in the `include` directory.
2. Update the constants as needed.
3. Rebuild and upload the firmware to apply the changes:
```bash
make build
make upload
```

---

## **Features**

- **CO2 Monitoring:** Measures CO2 levels using the SCD30 sensor.
- **Environmental Data:** Reads temperature, humidity, and pressure from the SCD30 and BMP280 sensors.
- **OLED Display:** Displays sensor readings and warnings on an SSD1306 OLED screen.
- **Logging:** Logs sensor data and system messages using the `Logger` class.
- **Calibration:** Automatically calibrates the SCD30 sensor and stores the calibration flag in EEPROM.
- **I2C Scanner:** Scans the I2C bus for connected devices.

---

## **Using Additional Makefile Targets**

The `Makefile` includes additional targets for specific tasks:

### **Run the I2C Scanner:**
```bash
make i2c-scan
```

### **Run the Display Check:**
```bash
make display-check
```

### **Clean the Build Files:**
```bash
make clean
```

### **Show Help:**
```bash
make help
```

---

## **Windows-Specific Notes**

- Use **Git Bash** or a terminal where `make` is available to run the commands.
- Ensure that the `make` command is in your system's PATH. If you installed `make` via MinGW, add the `C:\MinGW\bin` directory to your PATH:
  1. Open the Start menu and search for "Environment Variables."
  2. Click "Edit the system environment variables."
  3. In the "System Properties" window, click "Environment Variables."
  4. Under "System variables," find the `Path` variable and click "Edit."
  5. Add the path to the MinGW `bin` directory (e.g., `C:\MinGW\bin`).
  6. Click "OK" to save and close all windows.

---

## **License**

This project is licensed under the [MIT License](LICENSE).

---

## **Contributing**

Contributions are welcome! Feel free to submit issues or pull requests to improve the project.

---

## **Contact**

For questions or support, please use the issue tracker.

