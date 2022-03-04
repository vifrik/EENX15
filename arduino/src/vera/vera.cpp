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


Magnetic magnetic;
ServoManager servoManager = ServoManager(11);
Motor motor = Motor(2, 3, 4);
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