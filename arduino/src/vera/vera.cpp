#include <cmath>
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#include <Vector.h>

#include "shared/position.h"
#include "shared/purePursuitController.h"
#include "shared/magnetic.h"
#include "shared/motor.h"
#include "shared/coord.h"

Servo servo;
int straightTime = 968; // diff 168
int maxRightTime = 800;
int maxLeftTime = 1136;

Magnetic magnetic;

Motor motor = Motor(2, 3, 4);

Position position;

Vector<Coord> path;
purePursuitController ppc = purePursuitController(path, 0.2);

/// Get microseconds for servo motor
/// \param delta steering angle
/// \return number of microseconds
int deltaToMs(float delta) {
    delta = constrain(delta, -0.44078, 0.44078); // ~25 deg in rad
    return map(delta, -0.44078, 0.44078, maxLeftTime, maxRightTime);
}

void setup() {
    for (int i = 0; i < 1000; i++) {
        Coord coord = Coord(i / 100.0, sin((i / 100.0) * 500.0 * PI / 180.0));
        path.push_back(coord);
    }

    Serial.begin(115200);
    Wire.begin();

    magnetic.calibrate();

    servo.attach(11);
    servo.writeMicroseconds(straightTime);

    delay(3000);

    motor.setForwards();
    motor.setSpeed(125);
}

void loop() {
    float phi = magnetic.readAngle();

    Coord positionTrailer = position.getPositionTrailer(phi);
    Coord positionTarget = ppc.getTarget(positionTrailer);
    float delta = position.steeringAngle(positionTrailer, positionTarget);

    servo.writeMicroseconds(deltaToMs(delta));

    if (ppc.atEnd()) {
        motor.stop();
    }

    delay(15);
}