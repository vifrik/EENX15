#include "defines.h"
#include <opencv2/aruco.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

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

#ifdef TCP
#include "tcpclient.h"
#endif


using namespace cv;

int main(int argc, char **argv) {
    String markerFilePath;
    uint32_t baudrate;

    for (int i = 1; i < argc; i++) {
        if (i != argc - 1) {
            if (strcmp(argv[i], "--markerfile") == 0 || strcmp(argv[i], "-m") == 0) {
                markerFilePath = argv[i + 1];
                i++;
            } else if (strcmp(argv[i], "--baudrate") == 0 || strcmp(argv[i], "-b") == 0) {
                try {
                    baudrate = std::stoi(argv[i + 1], nullptr);
                } catch (std::exception const &ex) {
                    std::cerr << "Wrong baudrate format!" << std::endl;
                    exit(1);
                }
                i++;
            }
        }
    }

    if (markerFilePath.empty()) {
        std::cerr << "Must specify marker file path!" << std::endl;
        exit(1);
    }

    cvwrapper c = cvwrapper(0, CAP_ANY);

#ifdef TCP
    tcpclient tcp = tcpclient();
    tcp.connectTo("127.0.0.1", 12000);
#endif

#ifdef SERIAL
    // Open port
    fd = open("/dev/tty.usbmodem14401", O_RDWR | O_NOCTTY | O_NDELAY);
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

    for (;;) {
        Mat frame;
        c.readFrame(frame); // Get the current frame

        if (c.numberOfMarkers()) {
            // Get rotational vecs and translational vecs of markers
            cvwrapper::rtvecs pos = c.getLocation(); // relative pos and rot of marker in relation to camera

            double weightTotal = 0;
            Vec3d sumCameraRotationalVector, sumCameraTranslationalVector;
            for (int i = 0; i < c.numberOfMarkers(); i++) {
                Mat objectRotationalMatrix;
                Rodrigues(pos.rvecs[i], objectRotationalMatrix);

                // fråga mig inte varför detta funkar... linalg-magi helt enkelt
                pos.tvecs[i][0] *= -1;

                // transform for marker relative camera
                Affine3d tMarkerCamera(objectRotationalMatrix, pos.tvecs[i]);

                // transform for camera relative world
                Affine3d tCameraWorld = markers[c.markerIds[i]] * tMarkerCamera;

                // translation and rotation vectors relative world
                Vec3d translation = tCameraWorld.translation();
                Vec3d rotation = eulerRotation::rotationMatrixToEulerAngles((Mat3d) tCameraWorld.rotation());

                // 1/distance^2
                double weight = 1 / pow(pos.tvecs[i][2], 2);
                sumCameraTranslationalVector += weight * translation;
                sumCameraRotationalVector += weight * rotation;
                weightTotal += weight;
            }

            // calculate weighted average
            sumCameraTranslationalVector /= weightTotal;
            sumCameraRotationalVector /= weightTotal;

            sumCameraRotationalVector(2) += -M_PI / 2; // Make zero rotation in the direction of the x-axis

            std::ostringstream os;
            os << std::fixed << std::setprecision(3) << sumCameraTranslationalVector << "\n"
               << sumCameraRotationalVector;
            std::string send = os.str();

#ifdef TCP
            char char_array[send.length() + 1];
            strcpy(char_array, send.c_str());
            tcp.sendMsg(char_array, sizeof char_array);
#endif

#ifdef SERIAL

            String out = std::to_string((float) sumCameraRotationalVector[2]);
            char char_array[out.length() + 1];
            strcpy(char_array, out.c_str());

            write(fd, char_array, sizeof char_array);
#endif

#ifdef DEBUG
            double camX = sumCameraTranslationalVector[0];
            double camY = sumCameraTranslationalVector[1];
            double camRZ = sumCameraRotationalVector[2];

            Scalar col = Scalar(255, 53, 184); // Red, green, blue color
            c.drawBoundingBoxes(frame, col); // Draw bounding boxes around all markers
            c.drawIds(frame, col); // Draw id label on marker
            //c.drawText(frame, send, 50, 50);
            c.drawBox(frame, col, 0, 0, 400, 400);
            c.drawCircle(frame, col, camX * 400 / 2,
                         camY * 400 / 2, 5);
            std::ostringstream s;
            s << "X: " << camX
              << " Y: " << camY
              << " R: " << camRZ;
            c.drawText(frame, s.str(), camX * 400 / 2,
                       camY * 400 / 2);

            double xs, ys, rsz;
            double angleMag = M_PI_4;
            int lenTrailer = 100;
            rsz = camRZ + M_PI + angleMag;
            xs = camX * 400 / 2 + cos(rsz) * lenTrailer;
            ys = camY * 400 / 2 + sin(rsz) * lenTrailer;


            Scalar colTrailer = Scalar(0, 255, 255);
            c.drawCircle(frame, colTrailer, xs, ys, 5);
            ellipse(frame, Point(camX * 400 / 2, camY * 400 / 2), Point(50, 50), 0,
                    (camRZ + M_PI) * 180 / M_PI, rsz * 180 / M_PI, colTrailer, 2);

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