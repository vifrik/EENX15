#include <cmath>
#include <SPI.h>
#include <Wire.h>

#include <Vector.h>

#include "shared/position.h"
#include "shared/purePursuitController.h"
#include "shared/magnetic.h"
#include "shared/motor.h"
#include "shared/coord.h"
#include "shared/servoManager.h"

#define SERVO_PIN 11
#define MOTOR_FIRST_DIR_PIN 2
#define MOTOR_SECOND_DIR_PIN 3
#define MOTOR_SPEED_PIN 4

Magnetic magnetic;
ServoManager servoManager = ServoManager(SERVO_PIN);
Motor motor = Motor(MOTOR_FIRST_DIR_PIN, MOTOR_SECOND_DIR_PIN, MOTOR_SPEED_PIN);
Position position;
Vector<Coord> path;
purePursuitController ppc = purePursuitController(path, 0.2);


void setup() {
    for (int i = 0; i < 1000; i++) {
        Coord coord = Coord(i / 100.0, sin((i / 100.0) * 500.0 * PI / 180.0));
        path.push_back(coord);
    }

    Serial.begin(115200);
    Wire.begin();
    magnetic.calibrate();
    servoManager.writeAngle(0);

    delay(3000);

    motor.setForwards();
    motor.setSpeed(125);
}

void loop() {
    float phi = magnetic.readAngle();

    Coord positionTrailer = position.getPositionTrailer(phi);
    Coord positionTarget = ppc.getTarget(positionTrailer);
    float delta = position.steeringAngle(positionTrailer, positionTarget);

    servoManager.writeAngle(delta);

    if (ppc.atEnd()) {
        motor.stop();
    }

    delay(15);
}