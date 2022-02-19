#include <opencv2/aruco.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "cvwrapper.h"

using namespace cv;

int main(int argc, char** argv) {
    cvwrapper c = cvwrapper(0, CAP_ANY);

    for (;;) {
        c.readFrame(); // Get the current frame
        c.detect(); // detect markers in frame

        Scalar col = Scalar(0,255,0); // Red, green, blue color
        c.drawBoundingBoxes(col); // Draw bounding boxes around all markers
        c.drawTexts(col); // Draw id label on marker

        // Get rotational vecs and translational vecs of markers
        cvwrapper::rtvecs pos = c.getLocation(); // relative pos and rot of marker in relation to camera

        c.show(); // Show the frame with drawings

        if (waitKey(2) >= 0) // Close on any key
            break;
    }

    c.release(); // Stop capturing video

    // TODO close socket here

    return 0;
}