#include <iostream>
#include <opencv2/opencv.hpp>
#include "../framework/framework.hpp"

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
        //} while (false);
        } while (FP_index == past_FP_index);
        point = point + (fixedPoints[FP_index] - point) * (1-fixedPointWeights[FP_index]);
        if (i > iterations/100) {
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

    for (unsigned i = 0; i < 1000; ++i) {
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
                auto &color = pic.at<Vec3w>((int)((num - xmin)/(xmax - xmin) * pic.rows), col);
                if (color.val[0] == MAX_COL) {
                    color = GREEN;
                    //pic.at<Vec3w>((int)((num - xmin)/(xmax - xmin) * pic.rows), col) = BLACK;
                } else if (color[1] != 0) {
                    color -= Vec3w(0, 13107, 0);
                }
            }
        }
    }
}

int main() {

    /*Mat pictureA1(1000, 1000, CV_16UC3, WHITE);
    chaosGame(pictureA1, {1.0/2, 1.0/2, 1.0/2, 1.0/2},
              {{200, 200}, {800, 200}, {500, 900}, {500, 500}}, 3000000);
    cv::imwrite("task6Test.png", pictureA1);*/

    /*Mat pictureA1(1440, 1440, CV_16UC3, WHITE);
    chaosGameRegularPolygon(pictureA1, {0.41, 0.41,0.41,0.41,0.41,0.41}, 700, 3000000);
    cv::imwrite("task6A1.png", pictureA1);

    Mat pictureA2(1000, 1000, CV_16UC3, WHITE);
    chaosGame(pictureA2, {1.0/3, 1.0/3, 1.0/3, 1.0/3, 1.0/3, 1.0/3},
            {{750, 100}, {750, 900}, {500, 750}, {250, 900}, {250, 100}, {500, 250}}, 3000000);
    cv::imwrite("task6A2.png", pictureA2);

    Mat pictureA3(1000, 1000, CV_16UC3, WHITE);
    chaosGame(pictureA3, {1.0/2, 1.0/2, 1.0/2, 1.0/2},
              {{200, 200}, {800, 200}, {500, 900}, {500, 500}}, 3000000);
    cv::imwrite("task6A3.png", pictureA3);*/


    /*Mat feigenbaum1(1440, 2560, CV_16UC3, WHITE);
    feigenbaum(feigenbaum1, 0.63, 0.65, 0.905, 0.915);
    imwrite("feigenbaum4.png", feigenbaum1);*/

    using namespace svg;
    /*LSystem lSystem1("task6C1.svg", 1440, 1440);
    lSystem1.addTranslationRule('F', {"F+F--F+F"});
    lSystem1.addDrawingRule<double>('F', &Turtle::forward, 5.0);
    lSystem1.addDrawingRule<double, bool>('-', &Turtle::left, 60, false);
    lSystem1.addDrawingRule<double, bool>('+', &Turtle::right, 60, false);

    lSystem1.m_turtle.drawing(false);
    lSystem1.m_turtle.forward(250);
    lSystem1.m_turtle.right(45);
    lSystem1.m_turtle.back(850);
    lSystem1.m_turtle.right(45);
    lSystem1.m_turtle.drawing(true);

    string str1 = lSystem1.translate("F--F--F", 5);
    lSystem1.drawString(str1);*/

    /*LSystem lSystem2("task6C2.svg", 1440, 1440);
    lSystem2.addTranslationRule('A', {"B-A-B"});
    lSystem2.addTranslationRule('B', {"A+B+A"});
    lSystem2.addDrawingRule<double>('A', &Turtle::forward, 10.0);
    lSystem2.addDrawingRule<double>('B', &Turtle::forward, 10.0);
    lSystem2.addDrawingRule<double, bool>('-', &Turtle::left, 60, false);
    lSystem2.addDrawingRule<double, bool>('+', &Turtle::right, 60, false);

    lSystem2.m_turtle.drawing(false);
    lSystem2.m_turtle.forward(150);
    lSystem2.m_turtle.right(45);
    lSystem2.m_turtle.back(900);
    lSystem2.m_turtle.right(45);
    lSystem2.m_turtle.drawing(true);

    string str2 = lSystem2.translate("A", 7);
    lSystem2.drawString(str2);*/

    /*LSystem lSystem3("task6C3.svg", 1440, 1440);
    lSystem3.addTranslationRule('A', {"-BF+AFA+FB-"});
    lSystem3.addTranslationRule('B', {"+AF-BFB-FA+"});
    lSystem3.addDrawingRule<double>('F', &Turtle::forward, 20.0);
    lSystem3.addDrawingRule<double, bool>('-', &Turtle::left, 90, false);
    lSystem3.addDrawingRule<double, bool>('+', &Turtle::right, 90, false);

    lSystem3.m_turtle.drawing(false);
    lSystem3.m_turtle.right(45);
    lSystem3.m_turtle.back(900);
    lSystem3.m_turtle.right(45);
    lSystem3.m_turtle.drawing(true);

    string str2 = lSystem3.translate("A", 6);
    lSystem3.drawString(str2);*/

    /*LSystem lSystem4("task6C4.svg", 1440, 1440);
    lSystem4.addTranslationRule('A', {"+BFB+"});
    lSystem4.addTranslationRule('B', {"-AFA-"});
    lSystem4.addDrawingRule<double>('F', &Turtle::forward, 20.0);
    lSystem4.addDrawingRule<double, bool>('-', &Turtle::left, 45, false);
    lSystem4.addDrawingRule<double, bool>('+', &Turtle::right, 45, false);

    lSystem4.m_turtle.drawing(false);
    lSystem4.m_turtle.right(90);
    lSystem4.m_turtle.drawing(true);

    string str4 = lSystem4.translate("A", 8);
    lSystem4.drawString(str4);*/

    /*LSystem lSystem5("task6C5.svg", 1440, 1440);
    lSystem5.addTranslationRule('F', {"FF+[+F-FF]-[-F+F+F]"});
    lSystem5.addDrawingRule<double>('F', &Turtle::forward, 15.0);
    lSystem5.addDrawingRule<double, bool>('-', &Turtle::left, 25, false);
    lSystem5.addDrawingRule<double, bool>('+', &Turtle::right, 25, false);
    lSystem5.addDrawingRule('[', &Turtle::pushPosition);
    lSystem5.addDrawingRule(']', &Turtle::popPosition);

    lSystem5.m_turtle.drawing(false);
    lSystem5.m_turtle.back(500);
    lSystem5.m_turtle.drawing(true);

    string x = lSystem5.translate("F", 4);
    lSystem5.drawString(x);*/



    /*LSystem lSystem5("task6C6.svg", 1440, 1440);
    lSystem5.addTranslationRule('F', {"X[+F][-F]"});
    lSystem5.addTranslationRule('X', {"XX"});
    lSystem5.addDrawingRule<double>('F', &Turtle::forward, 10.0);
    lSystem5.addDrawingRule<double>('X', &Turtle::forward, 10.0);
    lSystem5.addDrawingRule<double, bool>('-', &Turtle::left, 90, false);
    lSystem5.addDrawingRule<double, bool>('+', &Turtle::right, 90, false);
    lSystem5.addDrawingRule('[', &Turtle::pushPosition);
    lSystem5.addDrawingRule(']', &Turtle::popPosition);

    lSystem5.m_turtle.drawing(false);
    lSystem5.m_turtle.back(500);
    lSystem5.m_turtle.drawing(true);

    string x = lSystem5.translate("F", 7);
    lSystem5.drawString(x);*/

    /*LSystem lSystem6("task6C7.svg", 1440, 1440);
    lSystem6.addTranslationRule('F', {"X[F][+F]", "F[-F][XF]", "H"});
    lSystem6.addTranslationRule('X', {"XX"});
    lSystem6.addTranslationRule('H',
            {"+GGGGG[G]+[G]+[G]+[G]+[G]+[G]+[G]+[G]+[G]+[G]+[G]+[G]+",
             "-GGGGG[G]+[G]+[G]+[G]+[G]+[G]+[G]+[G]+[G]+[G]+[G]+[G]+"});
    lSystem6.addDrawingRule<double>('F', &Turtle::forward, 15.0);
    lSystem6.addDrawingRule<double>('X', &Turtle::forward, 15.0);
    lSystem6.addDrawingRule<double>('G', &Turtle::forward, 10.0);
    lSystem6.addDrawingRule<double, bool>('-', &Turtle::left, 30, false);
    lSystem6.addDrawingRule<double, bool>('+', &Turtle::right, 30, false);
    lSystem6.addDrawingRule('[', &Turtle::pushPosition);
    lSystem6.addDrawingRule(']', &Turtle::popPosition);

    lSystem6.m_turtle.drawing(false);
    lSystem6.m_turtle.back(500);
    lSystem6.m_turtle.drawing(true);

    string x = lSystem6.translate("F", 7);
    lSystem6.drawString(x);*/


    return 0;
}

