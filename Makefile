# Variables
BUILD_DIR = .pio/build
DOCS_DIR = docs/html
PLATFORMIO = platformio
DOXYGEN = doxygen

# Default target
all: build

# Build the main project
build:
	@echo "Building the main project..."
	platformio run -e esp12e

# Build and deploy the I2C scanner
scanner:
	@echo "Building and deploying the I2C scanner..."
	platformio run -e esp12e --project-conf tools/i2c-scanner/platformio.ini --target upload

# Deploy the main project
deploy:
	@echo "Deploying the main project..."
	platformio run -e esp12e --target upload

# Clean the build files
clean:
	@echo "Cleaning build files..."
	platformio run --target clean

# Monitor the serial output
monitor:
	@echo "Opening serial monitor..."
	platformio device monitor

# Help
help:
	@echo "Available targets:"
	@echo "  build    - Build the main project"
	@echo "  deploy   - Deploy the main project"
	@echo "  scanner  - Build and deploy the I2C scanner"
	@echo "  clean    - Clean the build files"
	@echo "  monitor  - Open the serial monitor"
	@echo "  help     - Show this help message"

.PHONY: all build deploy scanner clean monitor help