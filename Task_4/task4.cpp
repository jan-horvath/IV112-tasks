#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

const auto BLACK = cv::Vec3w(0,0,0);
const auto WHITE = cv::Vec3w(65535,65535,65535);
const auto BLUE = cv::Vec3w(65535,0,0);
const auto GREEN = cv::Vec3w(0,65535,0);
const auto RED = cv::Vec3w(0,0,65535);

void drawCircleImp(cv::Mat &pic, double x, double y, double r, bool fill) {
    const double DELTA = 0.015;

    for (size_t i = 0; i < pic.cols; ++i) {
        for (size_t j = 0; j < pic.rows; ++j) {
            if (!fill) {
                if (abs(sqrt((i - x) * (i - x) + (j - y) * (j - y)) / r - 1) < DELTA) {
                    pic.at<cv::Vec3w>(j, i) = BLACK;
                }
            } else {
                if (sqrt((i - x) * (i - x) + (j - y) * (j - y)) / r - 1 < DELTA) {
                    pic.at<cv::Vec3w>(j, i) = BLACK;
                }
            }
        }
    }
}

void equaliteralTriangle(cv::Mat &pic, double size, double cx, double cy) {


    for (size_t i = 0; i < pic.cols; ++i) {
        for (size_t j = 0; j < pic.rows; ++j) {

        }
    }
}



int main() {

    cv::Mat picture(1000, 1000, CV_16UC3, WHITE);
    drawCircleImp(picture, 200, 300, 100, false);
    drawCircleImp(picture, 800, 300, 100, true);

//    equaliteralTriangle(picture);

    cv::imwrite("task4A1.png", picture);

    return 0;
}
