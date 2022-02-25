// SERIAL or TCP
#define DEBUG
#define TCP

#include <opencv2/aruco.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <string.h>

#ifdef SERIAL
#include <unistd.h>  // UNIX standard function definitions
#include <fcntl.h>   // file control
#endif

#include "math.h"
#include "cvwrapper.h"
#include "writevec.h"

#ifdef TCP
#include "tcpclient.h"
#endif


using namespace cv;

int main(int argc, char** argv) {
    cvwrapper c = cvwrapper(0, CAP_ANY);
#ifdef TCP
    tcpclient tcp = tcpclient();
    tcp.connectTo("127.0.0.1", 12000);
#endif

#ifdef SERIAL
    int fileDescriptor;

    fileDescriptor = open("/dev/ttyf1", O_RDWR | O_NOCTTY | O_NDELAY);
    if (fileDescriptor < 0) {
        std::cerr << "Unable to open port /dev/ttyf1";
    }
    fcntl(fileDescriptor, F_SETFL, 0);

    int bytesWritten = write(fileDescriptor, "ATZ\r", 4);
    if (bytesWritten < 0)
        std::cerr << "Write failed";
#endif

    std::vector<Affine3d> markers;
    FileStorage fs_read("markers.txt",0);
    writevec::readVectorAffine3d(fs_read, "markers", markers);
    fs_read.release();

    for (;;) {
        c.readFrame(); // Get the current frame
        c.detect(); // detect markers in frame

        if (c.numberOfMarkers()) {
            // Get rotational vecs and translational vecs of markers
            cvwrapper::rtvecs pos = c.getLocation(); // relative pos and rot of marker in relation to camera

            double distanceTotal = 0;
            for (int i = 0; i < c.numberOfMarkers(); i++)
                distanceTotal += pos.tvecs[i][2];

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
                Affine3d tCameraWorld = markers[c.markerIds[i]]*tMarkerCamera;

                // translation and rotation vectors relative world
                Vec3d translation = tCameraWorld.translation();
                Vec3d rotation = oMath::rotationMatrixToEulerAngles((Mat3d) tCameraWorld.rotation());

                // distance^2/sumDist
                double weight = pow(pos.tvecs[i][2], 2) / distanceTotal;
                sumCameraTranslationalVector += weight * translation;
                sumCameraRotationalVector += weight * rotation;
                weightTotal += weight;
            }

            // calculate weighted average
            sumCameraTranslationalVector /= weightTotal;
            sumCameraRotationalVector /= weightTotal;

            std::ostringstream os;
            os << std::fixed << std::setprecision(3) << sumCameraTranslationalVector << "\n" << sumCameraRotationalVector;
            std::string send = os.str();

            char char_array[send.length() + 1];
            strcpy(char_array, send.c_str());
#ifdef TCP
            tcp.sendMsg(char_array, sizeof char_array);
#endif

#ifdef DEBUG
            Scalar col = Scalar(255,53,184); // Red, green, blue color
            c.drawBoundingBoxes(col); // Draw bounding boxes around all markers
            c.drawTexts(col); // Draw id label on marker
            c.drawText(send, 50, 50);
            circle( c.frame,
                    Point(sumCameraTranslationalVector[0]*400+50, sumCameraTranslationalVector[1]*400+50),
                    5,
                    Scalar( 255, 0, 0 ),
                    FILLED,
                    LINE_8 );
#endif
        }
#ifdef DEBUG
        c.show(); // Show the frame with drawings
#endif
        if (waitKey(2) >= 0) // Close on any key
            break;
    }

    c.release(); // Stop capturing video

    return 0;
}