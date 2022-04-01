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
#define MOTOR_SPEED 100

#define LOOKAHEAD_DISTANCE 0.5
#define SERIAL_BAUDRATE 9600
#define ZERO_ANGLE 0
#define WAKE_DELAY 3000
#define FRAME_DELAY 1

struct PositionData
{
    float x;
    float y;
    float rz;
    uint16_t crc;
} positionData;

uint16_t crc16(unsigned char* data, size_t len) {
    uint16_t crc = 0;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int k = 0; k < 8; k++) {
            crc = crc & 1 ? (crc >> 1) ^ 0xa001 : crc >> 1;
        }
    }
    if(crc < 256) crc |= 1 << 8;
    if (!(crc & 255)) crc |= 1 << 0;
    return crc;
}

ServoManager servoManager = ServoManager(SERVO_PIN);
Motor motor = Motor(MOTOR_FIRST_DIR_PIN, MOTOR_SECOND_DIR_PIN, MOTOR_SPEED_PIN);
Position position;
Vector<Coord> path;
Coord coords[1000];
purePursuitController ppc;


void setup() {
    path.setStorage(coords);
    for (int i = 850; i > 0; i--) {
        Coord coord = Coord(i / 500.0, 1);
        path.push_back(coord);
    }

    Serial.begin(SERIAL_BAUDRATE); // host communication
    SerialUSB.begin(SERIAL_BAUDRATE); // jetson nano communication
    Wire.begin();
    servoManager.writeAngle(ZERO_ANGLE);

    delay(WAKE_DELAY);

    motor.setForwards();
    motor.setSpeed(MOTOR_SPEED);
}

void loop() {
    unsigned char bytesBuffer[15]; // 4 bytes * 3 floats + null terminator, 4+4+4+1 = 13
    int size = SerialUSB.readBytesUntil(byte(0), bytesBuffer, 15);
    if(size != 14) return;

    memcpy(&positionData, bytesBuffer, 14);  
    uint16_t c = crc16(bytesBuffer, 12);
    if(c != positionData.crc) return;

    //Serial.println("######## BEGIN ########");

    Coord positionTrailer(positionData.x, positionData.y);
    //Coord positionTrailer = Coord(cos(positionData.rz + PI) * CAMERA_OFFSET + positionData.x,
        //sin(positionData.rz + PI) * CAMERA_OFFSET + positionData.y); // update with camera pose
    Coord positionTarget = ppc.getTarget(path, LOOKAHEAD_DISTANCE, positionTrailer);

    //Serial.println(positionData.rz);

    
    /*Serial.print("Position data:: x:");
    Serial.print(positionTrailer.x);
    Serial.print(" y: ");
    Serial.println(positionTrailer.y);

    Serial.print("Target data:: x:");
    Serial.print(positionTarget.x);
    Serial.print(" y: ");
    Serial.println(positionTarget.y);*/
/*
    Serial.print("Target data:: x:");
    Serial.print(positionTarget.x);
    Serial.print(" y: ");
    Serial.println(positionTarget.y);
    */
    float delta = position.steeringAngle(positionTrailer, positionTarget, positionData.rz);
    servoManager.writeAngle(delta);

    //Serial.print("Angle data:: delta:");
    //Serial.println(delta);

    //servoManager.writeAngle(0);
    //servoManager.writeAngle(positionData.rz * 180 / 3.14);

    if (ppc.atEnd()) {
        motor.stop();
    }

    //Serial.println("########  END  ########");

    delay(FRAME_DELAY);
}