#include "defines.h"
#include <opencv2/aruco.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <Eigen/Dense>

#include "Kalman.h"
#include "eulerRotation.h"
#include "cvwrapper.h"
#include "writevec.h"

#ifdef SERIAL

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int fd;
#endif

using namespace cv;

uint16_t crc16(unsigned char *data, size_t len) {
    uint16_t crc = 0;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int k = 0; k < 8; k++) {
            crc = crc & 1 ? (crc >> 1) ^ 0xa001 : crc >> 1;
        }
    }
    if (crc < 256)
        crc |= 1 << 8;
    if (!(crc & 255))
        crc |= 1 << 0;
    return crc;
}

int main(int argc, char **argv) {
    String markerFilePath;

    for (int i = 1; i < argc; i++) {
        if (i != argc - 1) {
            if (strcmp(argv[i], "--markerfile") == 0 || strcmp(argv[i], "-m") == 0) {
                markerFilePath = argv[i + 1];
                i++;
            }
        }
    }

    if (markerFilePath.empty()) {
        std::cerr << "Must specify marker file path!" << std::endl;
        exit(1);
    }

    cvwrapper c = cvwrapper(0, CAP_ANY);

#ifdef SERIAL
    // Open port
    fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        printf("Device cannot be opened.\n");
        exit(-1);
    }

    struct termios options;
    tcgetattr(fd, &options);                        // Get the current options of the port
    options.c_cflag = B9600 | CLOCAL | CREAD | CS8; // Configure the device : 8 bits, no parity, no control
    options.c_iflag = IGNPAR;
    options.c_cc[VTIME] = 0;
    options.c_cc[VMIN] = 0;
    tcflush(fd, TCIFLUSH);
    if (tcsetattr(fd, TCSANOW, &options) < 0) {
        printf("Error setting serial port attributes!\n");
        exit(-1);
    }
#endif

    std::vector<Affine3d> markers;
    FileStorage fs_read("markers.txt", 0);
    writevec::readVectorAffine3d(fs_read, "markers", markers);
    fs_read.release();

    int n = 6; // Number of states
    int m = 2; // Number of measurements

    double dt = 1.0 / 30; // Time step

    Eigen::MatrixXd A(n, n); // System dynamics matrix
    Eigen::MatrixXd C(m, n); // Output matrix
    Eigen::MatrixXd Q(n, n); // Process noise covariance
    Eigen::MatrixXd R(m, m); // Measurement noise covariance
    Eigen::MatrixXd P(n, n); // Estimate error covariance

    A << 1, dt, 1 / 2 * pow(dt, 2), 0, 0, 0,
            0, 1, dt, 0, 0, 0,
            0, 0, 1, 0, 0, 0,
            0, 0, 0, 1, dt, 1 / 2 * pow(dt, 2),
            0, 0, 0, 0, 1, dt,
            0, 0, 0, 0, 0, 1;
    C << 1, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0;

    Q << 1, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0,
            0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 0, 1;
    Q = Q * 0.05;
    R << 0.05, 0, 0, 0.05;
    P << 300, 0, 0, 0, 0, 0,
            0, 300, 0, 0, 0, 0,
            0, 0, 300, 0, 0, 0,
            0, 0, 0, 300, 0, 0,
            0, 0, 0, 0, 300, 0,
            0, 0, 0, 0, 0, 300;

    Kalman kalman(dt, A, C, Q, R, P);

    Eigen::VectorXd x0(n);
    double t = 0;
    x0 << 0, 0, 0, 0, 0, 0;
    kalman.init(t, x0);

    for (;;) {
        Mat frame;
        c.readFrame(frame); // Get the current frame

        if (c.numberOfMarkers()) {
            // Get rotational vecs and translational vecs of markers
            cvwrapper::rtvecs pos = c.getLocation(); // relative pos and rot of marker in relation to camera

            Vec3d sumCameraRotationalVector;
            for (int i = 0; i < c.numberOfMarkers(); i++) {
                Mat objectRotationalMatrix;
                Rodrigues(pos.rvecs[i], objectRotationalMatrix);

                pos.tvecs[i][0] *= -1;

                // transform for marker relative camera
                Affine3d tMarkerCamera(objectRotationalMatrix, pos.tvecs[i]);

                // transform for camera relative world
                Affine3d tCameraWorld = markers[c.markerIds[i]] * tMarkerCamera.inv();

                // translation and rotation vectors relative world
                Vec3d translation = tCameraWorld.translation();
                Vec3d rotation = eulerRotation::rotationMatrixToEulerAngles((Mat3d) tCameraWorld.rotation());

                Eigen::VectorXd m1(m);
                m1 << translation[0], translation[2];
                kalman.update(m1);

                sumCameraRotationalVector = rotation;
            }

#ifdef SERIAL
            unsigned char char_array[15]; // 4 bytes * 3 floats + null terminator, 4+4+4+1 = 13
            float x = sumCameraTranslationalVector[0];
            float y = sumCameraTranslationalVector[1];
            float rz = sumCameraRotationalVector[2];

            memcpy(char_array, &x, 4);
            memcpy(char_array + 4, &y, 4);
            memcpy(char_array + 8, &rz, 4);
            uint16_t crc = crc16(char_array, 12);
            //std::cout << std::hex << crc << std::endl;
            memcpy(char_array + 0xC, &crc, 8);
            char_array[14] = '\0'; // null terminator

            write(fd, char_array, sizeof char_array);
#endif

#ifdef DEBUG
            double camX = kalman.state()[0];
            double camY = kalman.state()[3];
            double camRZ = sumCameraRotationalVector[2];

            Scalar col = Scalar(255, 53, 184); // Red, green, blue color
            c.drawBoundingBoxes(frame, col); // Draw bounding boxes around all markers
            c.drawIds(frame, col); // Draw id label on marker
            c.drawBox(frame, col, 0, 0, 400, 400);
            c.drawCircle(frame, col, camX * 400 / 2,
                         camY * 400 / 2, 5);
            std::ostringstream s;
            s << "X: " << camX
              << " Y: " << camY
              << " R: " << camRZ;
            c.drawText(frame, s.str(), camX * 400 / 2,
                       camY * 400 / 2);
#endif
        }
#ifdef DEBUG
        c.show(frame); // Show the frame with drawings
#endif
        if (waitKey(2) >= 0) // Close on any key
            break;
    }

    c.release(); // Stop capturing video

    return 0;
}
