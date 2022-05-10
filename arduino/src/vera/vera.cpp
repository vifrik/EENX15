#include <cmath>
#include <SPI.h>
#include <Wire.h>

#include <Vector.h>

#include "shared/position.h"
#include "shared/purePursuitController.h"
//#include "shared/magnetic.h"
#include "shared/motor.h"
#include "shared/coord.h"
#include "shared/servoManager.h"
#include "shared/potentiometer.h"

#define SERVO_PIN 11
#define MOTOR_FIRST_DIR_PIN 2
#define MOTOR_SECOND_DIR_PIN 3
#define MOTOR_SPEED_PIN 4
#define MOTOR_SPEED 110

#define LOOKAHEAD_DISTANCE 0.2
#define SERIAL_BAUDRATE 9600
#define ZERO_ANGLE 0
#define WAKE_DELAY 3000
#define FRAME_DELAY 15

Potentiometer potentiometer;
ServoManager servoManager = ServoManager(SERVO_PIN);
Motor motor = Motor(MOTOR_FIRST_DIR_PIN, MOTOR_SECOND_DIR_PIN, MOTOR_SPEED_PIN);
Position position;
Coord coords[1000];
Vector<Coord> path;
purePursuitController ppc = purePursuitController(); //path, LOOKAHEAD_DISTANCE);


void setup() {
    Serial.begin(SERIAL_BAUDRATE);
    Wire.begin();
    servoManager.writeAngle(ZERO_ANGLE);

    path.setStorage(coords);
    float radius = 0.9;
    float distance = 3.3;
    float steps = 300;
    for (int i = 0; i < steps; i++) {
        float step = (distance - radius) / steps;
        Coord coord = Coord(i * step, 0);
        path.push_back(coord);
    }

    Coord lastPos = path.at(path.size() - 1);
    for (int i = 0; i < steps; i++) {
        float step = PI / (2*steps);
        Coord coord = Coord(lastPos.x + radius * cos(-(PI / 2 - step * i)), lastPos.y + radius + radius * sin(-(PI / 2 - step * i)));
        path.push_back(coord);
    }

    Coord lastRotPos = path.at(path.size() - 1);
    for (int i = 0; i < steps; i++) {
        float step = (distance - radius) / steps;
        Coord coord = Coord(lastRotPos.x, lastRotPos.y + i * step);
        path.push_back(coord);
    }

    // for (int i = 0; i < path.size(); i++) {
    //     Serial.print(path.at(i).x);
    //     Serial.print(",");
    //     Serial.print(path.at(i).y);
    //     Serial.println(",0");
    // }

    // exit(1);

    delay(WAKE_DELAY);

    motor.setBackwards();
    motor.setSpeed(MOTOR_SPEED);
}

PositionData posTruck;
PositionData posTrailer;

#include <Servo.h>
Servo servo;
void loop() {
    float phi = potentiometer.read();

    if(!position.getPositionTrailer(phi, posTruck, posTrailer)) {
        //Serial.println("Fail get posttrailer");
        return;
    }

    Coord posTarget = ppc.getTarget(path, 0.5, Coord(posTrailer.x, posTrailer.y));
    float delta = position.steeringAngle(posTruck, posTrailer, posTarget);

    servoManager.writeAngle(delta);

    if (ppc.atEnd()) {
        motor.stop();
    }

#ifdef PRINT
    Serial.print("potentio: ");
    Serial.println(phi);
    Serial.println("------------------------");
#endif

    delay(FRAME_DELAY);
}