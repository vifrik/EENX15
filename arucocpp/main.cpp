#include <opencv2/aruco.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;

#define THICKNESS 2

int main(int argc, char** argv) {
    VideoCapture videoCapture;
    int deviceID = 0;             // 0 = default
    int apiID = cv::CAP_ANY;      // 0 = autodetect API
    videoCapture.open(deviceID, apiID);

    // Camera calibration, use executable calibrate to calibrate
    Mat cameraMatrix = (Mat1f(3, 3) << 971.35220497224020, 0.0, 646.72249200823774, 0.0,
            970.01993978791074, 357.74170661497186, 0.0, 0.0, 1.0);
    Mat distCoeffs = (Mat1f(5, 1) << -0.036721326921445439, 0.60501514643400323,
            0.0027156799005218536, 0.0040208404882518750,
            -1.8414740894267736);

    // TODO open socket and connect here

    if (!videoCapture.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    // Use 4x4 dictionary with 50 tags and default params
    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_50);
    Ptr<aruco::DetectorParameters> parameters = aruco::DetectorParameters::create();

    std::vector<std::vector<Point2f>> markerCorners, rejectedCandidates;
    std::vector<int> markerIds;

    Mat frame;
    for (;;) {
        videoCapture.read(frame);
        if (frame.empty()) {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        // Look for markers and save result in markerCorners/markerIds
        aruco::detectMarkers(frame, dictionary, markerCorners, markerIds, parameters,
                             rejectedCandidates);

        for (int id : markerIds) {
            std::vector<Point2f> corners = markerCorners[id]; // Get current corners of tag with id, id

            Scalar color = Scalar(255, 0, 0); // Color scalar

            // Draw bounding box around tag
            for (int i = 0; i < 4; i++) {
                line(frame, corners[i], corners[(i + 1) % 4], color, THICKNESS, LINE_8);
            }

            // Write id text at top left corner of tag
            putText(frame, std::to_string(id), Point(corners[0].x,corners[0].y - 15),
                    FONT_HERSHEY_DUPLEX,1, color, 2, false);

            std::vector<Vec3d> rvecs, tvecs; // Rotational and translational vectors, contains x,y,z and rot around all axes
            aruco::estimatePoseSingleMarkers(markerCorners, 0.15, cameraMatrix, distCoeffs, rvecs, tvecs);

            // TODO send message to processing here
        }

        imshow("Live preview", frame);
        // Close on any key
        if (waitKey(5) >= 0)
            break;
    }

    videoCapture.release(); // Stop capturing video
    destroyAllWindows();
    // TODO close socket here

    return 0;
}