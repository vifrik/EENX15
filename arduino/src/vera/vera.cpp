#include <math.h>
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>

#include "magnetic.h"
#include "motor.h"
#include "position.h"
#include "purePursuitController.h"


Servo servo;
int straightTime = 968; // diff 168
int maxRightTime = 800;
int maxLeftTime = 1136;

Motor motor = Motor(2, 3, 4);

Position position;

std::vector<Coord> path;
purePursuitController ppc = purePursuitController(path, 0.2);

/// Get microseconds for servo motor
/// \param delta steering angle
/// \return number of microseconds
int deltaToMs(float delta) {
    delta = constrain(delta, -25.2552468, 25.2552468);
    return map(delta, -25.27559341, 25.2552468, maxLeftTime, maxRightTime);
}

void setup() {
    for (int i = 0; i < 1000; i++) {
        Coord coord = Coord(i / 100.0, sin((i / 100.0) * 500.0 * M_PI / 180.0));
        path.push_back(coord);
    }

    Serial.begin(115200);
    Wire.begin();

    Magnetic::calibrate();

    servo.attach(11);
    servo.writeMicroseconds(straightTime);

    delay(3000);

    motor.setForwards();
    motor.setSpeed(125);
}

void loop() {
    float phi = Magnetic::readAngle();
    Coord positionTrailer = position.updatePosition(phi);
    Coord positionTarget = ppc.getTarget(positionTrailer); // ppc logic here
    float phiDesired = position.getPhiDesired(positionTarget);
    float delta = position.phiDesiredToDelta(phiDesired);
    servo.writeMicroseconds(deltaToMs(delta * 180 / PI));

    if (ppc.atEnd()) {
        motor.stop();
    }

    delay(15);
}