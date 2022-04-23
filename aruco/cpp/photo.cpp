//
// Created by Viktor on 2022-04-22.
//



#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

using namespace cv;

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

    Mat gray;
    // vector to store the pixel coordinates of detected checker board corners
    std::vector<cv::Point2f> corner_pts;
    bool success;

    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    videoCapture.open(pipeline, cv::CAP_GSTREAMER);

    videoCapture.read(frame);
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    cv::imwrite("./out.jpg", frame);
    return 0;
}