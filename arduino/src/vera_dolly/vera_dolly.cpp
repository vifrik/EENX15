#include <cmath>
#include <SPI.h>
#include <Wire.h>

#include <Vector.h>

#include "shared/positionDolly.h"
#include "shared/purePursuitController.h"
#include "shared/magnetic.h"
#include "shared/motor.h"
#include "shared/coord.h"
#include "shared/servoManager.h"

#define SERVO_PIN 11
#define MOTOR_FIRST_DIR_PIN 2
#define MOTOR_SECOND_DIR_PIN 3
#define MOTOR_SPEED_PIN 4
#define MOTOR_SPEED 125

#define LOOKAHEAD_DISTANCE 0.2
#define SERIAL_BAUDRATE 115200
#define ZERO_ANGLE 0
#define WAKE_DELAY 3000
#define FRAME_DELAY 15

struct PositionData
{
    float x;
    float y;
    float rz;
} positionData;

ServoManager servoManager = ServoManager(SERVO_PIN);
Motor motor = Motor(MOTOR_FIRST_DIR_PIN, MOTOR_SECOND_DIR_PIN, MOTOR_SPEED_PIN);
Position position;
Vector<Coord> path;
purePursuitController ppc = purePursuitController(path, LOOKAHEAD_DISTANCE);


void setup() {
    for (int i = 0; i < 1000; i++) {
        Coord coord = Coord(i / 100.0, sin((i / 100.0) * 500.0 * PI / 180.0));
        path.push_back(coord);
    }

    SerialUSB.begin(SERIAL_BAUDRATE);
    Wire.begin();
    servoManager.writeAngle(ZERO_ANGLE);

    delay(WAKE_DELAY);

    motor.setForwards();
    motor.setSpeed(MOTOR_SPEED);
}

void loop() {
    byte bytesBuffer[13]; // 4 bytes * 3 floats + null terminator, 4+4+4+1 = 13
    int size = SerialUSB.readBytesUntil(byte(0), bytesBuffer, 13);

    memcpy(&positionData, bytesBuffer, 12);
    
    Coord positionTrailer = Coord(positionData.x, positionData.y); // update with camera pose
    Coord positionTarget = ppc.getTarget(positionTrailer);
    float delta = position.steeringAngle(positionTrailer, positionTarget);

    servoManager.writeAngle(delta);

    if (ppc.atEnd()) {
        motor.stop();
    }

    delay(FRAME_DELAY);
}