#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

using namespace cv;

// Defining the dimensions of checkerboard
int CHECKERBOARD[2]{6, 9};

std::string
gstreamer_pipeline(int capture_width, int capture_height, int display_width, int display_height, int framerate,
                   int flip_method) {
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) +
           ", height=(int)" +
           std::to_string(capture_height) + ", framerate=(fraction)" + std::to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" +
           std::to_string(display_width) + ", height=(int)" +
           std::to_string(display_height) +
           ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

int main() {
    printf("JETSON\n");
    //står att den ska klara 60 fps i 1280x720, men den kör 30 fps då
    int capture_width = 1280;
    int capture_height = 720;
    int display_width = 1280;
    int display_height = 720;
    int framerate = 60; //https://forums.developer.nvidia.com/t/120-fps-mode-support-removed-for-imx219-sensor/174327/9
    int flip_method = 0;

    std::string pipeline = gstreamer_pipeline(capture_width,
                                              capture_height,
                                              display_width,
                                              display_height,
                                              framerate,
                                              flip_method);

    Mat frame;
    VideoCapture videoCapture;

    // Creating vector to store vectors of 3D points for each checkerboard image
    std::vector<std::vector<cv::Point3f>> objpoints;

    // Creating vector to store vectors of 2D points for each checkerboard image
    std::vector<std::vector<cv::Point2f>> imgpoints;

    // Defining the world coordinates for 3D points
    std::vector<cv::Point3f> objp;
    for (int i = 0; i < CHECKERBOARD[1]; i++) {
        for (int j = 0; j < CHECKERBOARD[0]; j++)
            objp.push_back(cv::Point3f(j, i, 0));
    }

    Mat gray;
    // vector to store the pixel coordinates of detected checker board corners
    std::vector<cv::Point2f> corner_pts;
    bool success;

    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    videoCapture.open(pipeline, cv::CAP_GSTREAMER);

    int found = 0;
    // Looping over all the images in the directory
    for (int i = 0; i < 1000; i++) {
        videoCapture.read(frame);
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        success = cv::findChessboardCorners(gray, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts,
                                            CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);

        cv::imwrite("./img/image_" + std::to_string(i) + ".jpg", frame);

        if (success) {
            std::cout << "i : " << i << std::endl;
            found++;

            TermCriteria criteria(3, 30, 0.001);
            cornerSubPix(gray, corner_pts, Size(11, 11), Size(-1, -1), criteria);
            drawChessboardCorners(frame, Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, success);

            objpoints.push_back(objp);
            imgpoints.push_back(corner_pts);
        }

        if (found > 20)
            break;

        getchar();
    }

    //destroyAllWindows();

    Mat cameraMatrix, distCoeffs, R, T;
    double rms = cv::calibrateCamera(objpoints, imgpoints, cv::Size(gray.rows, gray.cols), cameraMatrix, distCoeffs, R,
                                     T);

    std::cout << "cameraMatrix : " << cameraMatrix << std::endl;
    std::cout << "distCoeffs : " << distCoeffs << std::endl;
    std::cout << "RMS : " << rms << std::endl;

    return 0;
}