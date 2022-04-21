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

    float old = 0;
    void writeAngle(float angle) {
        servo.write(angle*180/PI + 45);
        return;

        float incAngle = max(-PI/180,min(PI/180,angle-old));
        float newAngle = max(-15*PI/180,min(15*PI/180,old+incAngle));
        old = newAngle;
        servo.write(newAngle*180/PI+45);
        Serial.println(newAngle*180/PI+45);
    }
};

#endif //ARDUINO_SERVOMANAGER_H
