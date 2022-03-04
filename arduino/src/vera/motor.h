//
// Created by vikto on 2022-03-04.
//

#ifndef ARDUINO_MOTOR_H
#define ARDUINO_MOTOR_H

#include "Wire.h"

class Motor {
private:
    int firstDirPin;
    int secondDirPin;
    int speedPin;
public:
    Motor(int firstDirPin, int secondDirPin, int speedPin) : firstDirPin(firstDirPin), secondDirPin(secondDirPin),
                                                             speedPin(speedPin) {}

    void setForwards() const {
        digitalWrite(firstDirPin, LOW);
        digitalWrite(secondDirPin, HIGH);
    }

    void setBackwards() const {
        digitalWrite(firstDirPin, HIGH);
        digitalWrite(secondDirPin, LOW);
    }

    void setSpeed(unsigned char speed) const {
        analogWrite(speedPin, speed);
    }

    void stop() {
        digitalWrite(firstDirPin, LOW);
        digitalWrite(secondDirPin, LOW);
    }
};

#endif //ARDUINO_MOTOR_H
