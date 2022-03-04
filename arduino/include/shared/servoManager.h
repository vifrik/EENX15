//
// Created by vikto on 2022-03-04.
//

#ifndef ARDUINO_SERVOMANAGER_H
#define ARDUINO_SERVOMANAGER_H

#include <Servo.h>

#define STRAIGHT_TIME 968;
#define MAX_RIGHT_TIME 800
#define MAX_LEFT_TIME 1136
#define MAX_ANGLE 25*PI/180

class ServoManager {
private:
    Servo servo;

    /// Get microseconds for servo motor
    /// \param delta steering angle
    /// \return number of microseconds
    int deltaToMs(float delta) {
        delta = constrain(delta, -MAX_ANGLE, MAX_ANGLE); // ~25 deg in rad
        return map(delta, -MAX_ANGLE, MAX_ANGLE, MAX_LEFT_TIME, MAX_RIGHT_TIME);
    }

public:
    ServoManager(int pin) {
        servo.attach(11);
    }

    void writeAngle(float angle) {
        servo.writeMicroseconds(deltaToMs(angle));
    }
};

#endif //ARDUINO_SERVOMANAGER_H
