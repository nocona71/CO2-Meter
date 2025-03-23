#ifndef MOCK_BMP280_H
#define MOCK_BMP280_H

class MockBMP280 {
public:
    bool beginCalled;
    float temperatureValue;
    float pressureValue;

    MockBMP280(); // Constructor

    bool begin(uint8_t addr = 0x76);
    float readTemperature();
    float readPressure();
};

#endif // MOCK_BMP280_H