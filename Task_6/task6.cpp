#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

const auto MAX_COL = 65535;
const auto BLACK = cv::Vec3w(0,0,0);
const auto WHITE = cv::Vec3w(MAX_COL,MAX_COL,MAX_COL);

void chaosGame(Mat &pic, const vector<double> &fixedPointWeights, double radius, const Point2d &startingPoint, size_t iterations) {
    cv::Point2d point = startingPoint;
    vector<Point2d> fixedPoints;
    unsigned polygonNumber = fixedPointWeights.size();

    for (unsigned i = 0; i < fixedPointWeights.size(); ++i) {
        fixedPoints.push_back({cos(i * 2*M_PI/polygonNumber) * radius + pic.cols/2, sin(i * 2*M_PI/polygonNumber) * radius + pic.rows/2});
    }

    unsigned past_FP_index = 0;
    unsigned FP_index = 0;
    for (unsigned i = 0; i < iterations; ++i) {
        while (past_FP_index == FP_index) {FP_index = rand() % polygonNumber;}
        point = point + (fixedPoints[FP_index] - point) * (1-fixedPointWeights[FP_index]);
        if (i > iterations/100) {
            pic.at<Vec3w>((int) lround(point.y), (int) lround(point.x)) = BLACK;
        }
        past_FP_index = FP_index;
    }
}

int main() {
    Mat picture(1000, 1000, CV_16UC3, WHITE);

    chaosGame(picture, {1.0/3, 1.0/3, 1.0/3, 1.0/3, 1.0/3, 1.0/3}, 400, {500, 500}, 3000000);
    cv::imwrite("task6A.png", picture);

    return 0;
}

