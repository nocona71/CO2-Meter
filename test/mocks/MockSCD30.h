#ifndef MOCK_SCD30_H
#define MOCK_SCD30_H

class MockSCD30 {
public:
    bool beginCalled;
    float co2Value;
    float temperatureValue;
    float humidityValue;
    bool calibrationCalled;
    bool calibrationSuccess;

    MockSCD30(); // Constructor

    bool begin();
    float getCO2();
    float getTemperature();
    float getHumidity();
    bool calibrate(uint16_t freshAirCO2);
};

#endif // MOCK_SCD30_H