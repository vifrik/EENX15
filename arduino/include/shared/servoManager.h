//
// Created by vikto on 2022-03-04.
//

#ifndef ARDUINO_SERVOMANAGER_H
#define ARDUINO_SERVOMANAGER_H

#include <Servo.h>

#define STRAIGHT_TIME 968
#define MAX_RIGHT_TIME STRAIGHT_TIME + 168/2
#define MAX_LEFT_TIME STRAIGHT_TIME - 168/2
#define MAX_ANGLE 25

class ServoManager {
private:
    Servo servo;

    /// Get microseconds for servo motor
    /// \param delta steering angle
    /// \return number of microseconds
    int deltaToMs(float delta) {
        delta = constrain(delta, -MAX_ANGLE, MAX_ANGLE);
        return map(delta, -MAX_ANGLE, MAX_ANGLE, MAX_LEFT_TIME, MAX_RIGHT_TIME);
    }

public:
    ServoManager(int pin) {
        servo.attach(pin);
    }

    int old_write = 44;
    void writeAngle(float angle) {
        int ang = (int)(angle*180/PI) + 44;

        servo.write(ang);
        return;

        int new_ang = old_write + max(-15, min(15, ang - old_write));
        new_ang = max(44-20, min(new_ang, 44 + 20));

        servo.write(new_ang);
        old_write = new_ang;
    }
};

#endif //ARDUINO_SERVOMANAGER_H
