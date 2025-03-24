# Variables
BUILD_DIR = .pio/build
DOCS_DIR = docs/html
PLATFORMIO = platformio
DOXYGEN = doxygen

# Default target
all: build

# Build the sources
build:
	@echo "Building the project for esp12e..."
	$(PLATFORMIO) run -e esp12e

# Run tests
test:
	@echo "Running tests in unit_test environment..."
	$(PLATFORMIO) test -e unit_test

# Clean all build files
clean:
	@echo "Cleaning build files..."
	$(PLATFORMIO) run --target clean
	@rm -rf $(BUILD_DIR)

# Deploy to ESP32
deploy:
	@echo "Deploying to ESP32..."
	$(PLATFORMIO) run -e esp12e --target upload

# Generate Doxygen documentation
docs:
	@echo "Generating Doxygen documentation..."
	$(DOXYGEN) Doxyfile

# Help
help:
	@echo "Available targets:"
	@echo "  build   - Build the project"
	@echo "  test    - Run tests"
	@echo "  clean   - Clean all build files"
	@echo "  deploy  - Deploy to ESP32"
	@echo "  docs    - Generate Doxygen documentation"
	@echo "  help    - Show this help message"

.PHONY: all build test clean deploy docs help