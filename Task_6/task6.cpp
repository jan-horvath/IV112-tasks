#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

const auto MAX_COL = 65535;
const auto BLACK = cv::Vec3w(0,0,0);
const auto WHITE = cv::Vec3w(MAX_COL,MAX_COL,MAX_COL);
const auto BLUE = cv::Vec3w(MAX_COL,0,0);
const auto GREEN = cv::Vec3w(0,MAX_COL,0);
const auto RED = cv::Vec3w(0,0,MAX_COL);
const auto CYAN = cv::Vec3w(MAX_COL,MAX_COL,0);
const auto PURPLE = cv::Vec3w(MAX_COL,0,MAX_COL);
const auto YELLOW = cv::Vec3w(0,MAX_COL,MAX_COL);

const vector<cv::Vec3w>COLORS = {BLUE, GREEN, RED, CYAN, PURPLE, YELLOW};

void drawFixedPoints(Mat &pic, const vector<Point2d> &fixedPoints) {
    for (const auto& fp: fixedPoints) {
        for (int x = (int)fp.x - 5; x <= fp.x + 5; ++x) {
            for (int y = (int)fp.y - 5; y <= fp.y + 5; ++y) {
                pic.at<Vec3w>(y, x) = RED;
            }
        }
    }
}

void chaosGame(Mat &pic, const vector<double> &fixedPointWeights, const vector<Point2d> &fixedPoints, size_t iterations) {
    assert(fixedPoints.size() == fixedPointWeights.size());
    size_t count = fixedPoints.size();

    Point2d point{0,0};
    size_t past_FP_index = 0;
    size_t FP_index = 0;
    for (unsigned i = 0; i < iterations; ++i) {
        do {
            FP_index = rand() % count;
        } while (false);
        point = point + (fixedPoints[FP_index] - point) * (1-fixedPointWeights[FP_index]);
        if (i > iterations/100) {
            auto currentColor = pic.at<Vec3w>((int) lround(point.y), (int) lround(point.x));
            if (currentColor.val[0] != 0) {
                pic.at<Vec3w>((int) lround(point.y), (int) lround(point.x)) -= Vec3w(4369, 4369, 4369);
                //21845, 13107, 4369, 3855, 1285, 771, 257, 85, 51, 17, 15, 5, 3
            }
            pic.at<Vec3w>((int) lround(point.y), (int) lround(point.x)) = COLORS[FP_index];
        }
        past_FP_index = FP_index;
    }

    drawFixedPoints(pic, fixedPoints);
}

void chaosGameRegularPolygon(Mat &pic, const vector<double> &fixedPointWeights, double radius, size_t iterations) {
    vector<Point2d> fixedPoints;
    unsigned polygonNumber = fixedPointWeights.size();

    for (unsigned i = 0; i < fixedPointWeights.size(); ++i) {
        fixedPoints.push_back({cos(i * 2*M_PI/polygonNumber) * radius + (double)pic.cols/2, sin(i * 2*M_PI/polygonNumber) * radius + (double)pic.rows/2});
    }

    chaosGame(pic, fixedPointWeights, fixedPoints, iterations);
}

vector<double> getLogisticPattern(double r) {
    vector<double> pattern;
    double x = 0.5;

    for (unsigned i = 0; i < 500; ++i) {
        x = 4 * r * x * (1-x);
        if (i > 100) {pattern.push_back(x);}
    }
    return move(pattern);
}

void feigenbaum(Mat &pic, double xmin, double xmax, double rmin, double rmax) {
    double r_increment = (rmax - rmin) / pic.cols;
    double x_increment = (xmax - xmin) / pic.rows;

    for (unsigned col = 0; col < pic.cols; ++col) {
        auto pattern = getLogisticPattern(rmin + col*r_increment);
        for (double num : pattern) {
            if ((xmin < num) && (num < xmax)) {
                pic.at<Vec3w>((int)((num - xmin)/(xmax - xmin) * pic.rows), col) = BLACK;
            }
        }
    }
}

int main() {

    /*Mat pictureA1(1440, 1440, CV_16UC3, WHITE);
    chaosGameRegularPolygon(pictureA1, {1.0/2, 1.0/2, 1.0/2, 1.0/2, 1.0/2, 1.0/2}, 700, 3000000);
    cv::imwrite("task6A1.png", pictureA1);*/

    /*Mat pictureA2(1000, 1000, CV_16UC3, WHITE);
    chaosGame(pictureA2, {1.0/2, 1.0/2, 1.0/2, 1.0/2, 1.0/2, 1.0/2},
            {{750, 100}, {750, 900}, {500, 650}, {250, 900}, {250, 100}, {500, 350}}, 3000000);
    cv::imwrite("task6A2.png", pictureA2);

    Mat pictureA3(1000, 1000, CV_16UC3, WHITE);
    chaosGame(pictureA3, {1.0/2, 1.0/2, 1.0/2, 1.0/2},
              {{200, 200}, {800, 200}, {500, 900}, {500, 500}}, 3000000);
    cv::imwrite("task6A3.png", pictureA3);*/

    auto a = getLogisticPattern(0.7);

    Mat feigenbaum1(1440, 2560, CV_16UC3, WHITE);
    feigenbaum(feigenbaum1, 0.3, 0.7, 0.85, 0.99);
    imwrite("feigenbaum1.png", feigenbaum1);



    return 0;
}

