#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

class Logger {
public:
    enum Level { INFO, WARNING, ERROR };

    static void log(Level level, const char* message) {
        switch (level) {
            case INFO:
                Serial.print("[INFO] ");
                break;
            case WARNING:
                Serial.print("[WARNING] ");
                break;
            case ERROR:
                Serial.print("[ERROR] ");
                break;
        }
        Serial.println(message);
    }
};

#endif // LOGGER_H