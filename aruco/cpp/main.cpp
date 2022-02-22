#include <opencv2/aruco.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "cvwrapper.h"
#include "tcpclient.h"

using namespace cv;

int main(int argc, char** argv) {
    cvwrapper c = cvwrapper(0, CAP_ANY);
    tcpclient tcp = tcpclient();
    tcp.connectTo("127.0.0.1", 12000);

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

            std::ostringstream os;

            for (int i = 0; i < c.numberOfMarkers(); i++) {
                os << pos.tvecs[i] << "\n";
                os << pos.rvecs[i] << (i == c.numberOfMarkers() - 1 ? "" : "\n");
            }

            std::string toSend = os.str();
            char char_array[toSend.length() + 1];
            strcpy(char_array, toSend.c_str());

            tcp.sendMsg(char_array, sizeof char_array);
        }

        c.show(); // Show the frame with drawings

        if (waitKey(2) >= 0) // Close on any key
            break;
    }

    c.release(); // Stop capturing video

    return 0;
}