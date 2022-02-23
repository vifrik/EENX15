#include <opencv2/aruco.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "cvwrapper.h"
#include "tcpclient.h"
#include "math.h"

using namespace cv;

int main(int argc, char** argv) {
    cvwrapper c = cvwrapper(0, CAP_ANY);
    tcpclient tcp = tcpclient();
    tcp.connectTo("127.0.0.1", 12000);


    std::vector<Affine3d> markers;
    Vec3d r(0, 0, 0);
    Vec3d t(1.0,0,0);
    Affine3d marker1(oMath::eulerAnglesToRotationMatrix(r), t);

    markers.push_back(marker1);

    for (;;) {
        c.readFrame(); // Get the current frame
        c.detect(); // detect markers in frame

        if (c.numberOfMarkers()) {
            Scalar col = Scalar(0,255,0); // Red, green, blue color
            c.drawBoundingBoxes(col); // Draw bounding boxes around all markers
            c.drawTexts(col); // Draw id label on marker

            // Get rotational vecs and translational vecs of markers
            cvwrapper::rtvecs pos = c.getLocation(); // relative pos and rot of marker in relation to camera
            // TODO send data to arduino here

            Mat1d sumCameraRotationalVector(3, 1), sumCameraTranslationalVector(3, 1);
            for (int i = 0; i < c.numberOfMarkers(); i++) {
                Mat objectRotationalMatrix;
                Rodrigues(pos.rvecs[i], objectRotationalMatrix);

                // Transform of marker relative camera
                Affine3d tMarkerCamera(objectRotationalMatrix, pos.tvecs[i]);

                // Transform of camera relative world
                Affine3d tCameraWorld = markers[c.markerIds[i]]*tMarkerCamera;

                std::ostringstream os;
                os << tCameraWorld.translation() << "\n"
                   << oMath::rotationMatrixToEulerAngles((Mat3d) tCameraWorld.rotation());

                std::string send = os.str();
                c.drawText(send, 50, 50);
                char char_array[send.length() + 1];
                strcpy(char_array, send.c_str());
                tcp.sendMsg(char_array, sizeof char_array);
            }
        }

        c.show(); // Show the frame with drawings

        if (waitKey(2) >= 0) // Close on any key
            break;
    }

    c.release(); // Stop capturing video

    return 0;
}