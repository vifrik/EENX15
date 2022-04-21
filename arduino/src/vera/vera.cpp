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
#define MOTOR_SPEED 125

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
    path.setStorage(coords);
    for (int i = 0; i < 1000; i++) {
        //Coord coord = Coord(i / 100.0, sin((i / 100.0) * 500.0 * PI / 180.0));
        Coord coord = Coord(1, i/100.0f);
        path.push_back(coord);
    }

    Serial.begin(SERIAL_BAUDRATE);
    Wire.begin();
    servoManager.writeAngle(ZERO_ANGLE);

    delay(WAKE_DELAY);

    motor.setBackwards();
    motor.setSpeed(MOTOR_SPEED);
}

PositionData posTruck;
PositionData posTrailer;
void loop() {
    float phi = potentiometer.read();

    if(!position.getPositionTrailer(phi, posTruck, posTrailer)) {
        Serial.println("Fail get posttrailer");
        return;
    }

    Coord posTarget = ppc.getTarget(path, 75, Coord(posTrailer.x, posTrailer.y));
    float delta = position.steeringAngle(posTruck, posTrailer, posTarget);

    servoManager.writeAngle(delta);

    if (ppc.atEnd()) {
        motor.stop();
    }

    Serial.print("potentio: ");
    Serial.println(phi);
    Serial.println("------------------------");

    //delay(FRAME_DELAY);
}