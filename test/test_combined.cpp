#include <unity.h>
#include "mocks/MockSCD30.h"
#include "mocks/MockBMP280.h"
#include "mocks/MockDisplay.h"

// Include mock implementations
#include "mocks/MockSCD30.cpp"
#include "mocks/MockBMP280.cpp"
#include "mocks/MockDisplay.cpp"

// Test sensor initialization
void test_scd30_initialization(void) {
    MockSCD30 scd30;
    TEST_ASSERT_TRUE(scd30.begin());
    TEST_ASSERT_TRUE(scd30.beginCalled);
}

void test_bmp280_initialization(void) {
    MockBMP280 bmp280;
    TEST_ASSERT_TRUE(bmp280.begin());
    TEST_ASSERT_TRUE(bmp280.beginCalled);
}

// Test display initialization
void test_display_initialization(void) {
    MockDisplay display;
    TEST_ASSERT_TRUE(display.begin(SSD1306_SWITCHCAPVCC, 0x3C));
    TEST_ASSERT_TRUE(display.beginCalled);
}

// Test CO2 sensor readings
void test_scd30_readings(void) {
    MockSCD30 scd30;
    scd30.begin();

    TEST_ASSERT_EQUAL_FLOAT(450.0f, scd30.getCO2());
    TEST_ASSERT_EQUAL_FLOAT(22.5f, scd30.getTemperature());
    TEST_ASSERT_EQUAL_FLOAT(55.0f, scd30.getHumidity());
}

// Test pressure sensor readings
void test_bmp280_readings(void) {
    MockBMP280 bmp280;
    bmp280.begin();

    TEST_ASSERT_EQUAL_FLOAT(22.0f, bmp280.readTemperature());
    TEST_ASSERT_EQUAL_FLOAT(101325.0f, bmp280.readPressure());
}

// Test displaying a message
void test_display_message(void) {
    MockDisplay display;
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(1);
    display.println("CO2: 450 ppm");
    display.display();

    TEST_ASSERT_TRUE(display.clearCalled);
    TEST_ASSERT_TRUE(display.displayCalled);
    TEST_ASSERT_EQUAL_STRING("CO2: 450 ppm", display.lastMessage.c_str());
}

// Test CO2 sensor calibration
void test_scd30_calibration_success(void) {
    MockSCD30 scd30;
    scd30.begin();

    // Simulate successful calibration
    scd30.calibrationSuccess = true;
    TEST_ASSERT_TRUE(scd30.calibrate(400));
    TEST_ASSERT_TRUE(scd30.calibrationCalled);
    TEST_ASSERT_EQUAL_FLOAT(400.0f, scd30.getCO2());
}

void test_scd30_calibration_failure(void) {
    MockSCD30 scd30;
    scd30.begin();

    // Simulate calibration failure
    scd30.calibrationSuccess = false;
    TEST_ASSERT_FALSE(scd30.calibrate(400));
    TEST_ASSERT_TRUE(scd30.calibrationCalled);
    TEST_ASSERT_NOT_EQUAL(400.0f, scd30.getCO2());
}

void setup() {
    delay(2000); // Give time for the serial monitor to connect
    UNITY_BEGIN();

    // Run sensor initialization tests
    RUN_TEST(test_scd30_initialization);
    RUN_TEST(test_bmp280_initialization);

    // Run sensor reading tests
    RUN_TEST(test_scd30_readings);
    RUN_TEST(test_bmp280_readings);

    // Run display tests
    RUN_TEST(test_display_initialization);
    RUN_TEST(test_display_message);

    // Run calibration tests
    RUN_TEST(test_scd30_calibration_success);
    RUN_TEST(test_scd30_calibration_failure);

    UNITY_END();
}

void loop() {
    // Nothing to do here
}