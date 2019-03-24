#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

const auto MAX_COL = 65535;
const auto BLACK = cv::Vec3w(0,0,0);
const auto WHITE = cv::Vec3w(MAX_COL,MAX_COL,MAX_COL);
const auto BLUE = cv::Vec3w(MAX_COL,0,0);
const auto GREEN = cv::Vec3w(0,MAX_COL,0);
const auto RED = cv::Vec3w(0,0,MAX_COL);

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

void colorQuadpixel(cv::Mat &pic, double X, double Y, const cv::Vec3w &colour) {
    pic.at<cv::Vec3w>(static_cast<int>(floor(Y)), static_cast<int>(floor(X))) = colour;
    pic.at<cv::Vec3w>(static_cast<int>(floor(Y)), static_cast<int>(ceil(X))) = colour;
    pic.at<cv::Vec3w>(static_cast<int>(ceil(Y)), static_cast<int>(floor(X))) = colour;
    pic.at<cv::Vec3w>(static_cast<int>(ceil(Y)), static_cast<int>(ceil(X))) = colour;
}

void drawSpiral(cv::Mat &pic, const cv::Point2d& center, double maxRadius) {
    double radius = maxRadius;
    double radiusDecrement = 0.01;
    double angle = M_PI_4;
    double angleIncrement = 0.005;

    while (radius > 0.5) {
        double X = radius * cos(angle) + center.x;
        double Y = radius * sin(angle) + center.y;
        double ratio = radius/maxRadius;
        colorQuadpixel(pic, X, Y, cv::Vec3w(ratio*MAX_COL, (1-ratio)*MAX_COL, 4*(1-ratio)*ratio*MAX_COL));
        radius -= radiusDecrement;
        angle += angleIncrement;
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
    drawCircleImp(picture, 200, 120, 100, false);
    drawCircleImp(picture, 800, 120, 100, true);
    drawSpiral(picture, {780, 780}, 200);

    cv::imwrite("task4A1.png", picture);

    return 0;
}
