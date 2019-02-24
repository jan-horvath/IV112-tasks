#include <iostream>
#include "../framework/framework.hpp"
#include <opencv2/opencv.hpp>

#include <vector>
#include <cmath>

using namespace svg;

auto getDivisorCount(unsigned number) {
    unsigned currentDivCount = 0;
    for (unsigned divisor = 1; divisor < number; ++divisor) {
        if (number % divisor == 0) {
            currentDivCount++;
        }
    }
    return currentDivCount;
}

auto maxDivisors(unsigned input) {
    std::vector<unsigned> maxDivCountNums;
    unsigned maxDivCount = 0;
    unsigned currentDivCount = 0;

    for (unsigned number = 0; number < input; ++number) {
        currentDivCount = getDivisorCount(number);

        if (currentDivCount > maxDivCount) {
            maxDivCountNums.clear();
            maxDivCount = currentDivCount;
            maxDivCountNums.push_back(number);
        } else if (currentDivCount == maxDivCount) {
            maxDivCountNums.push_back(number);
        }
    }

    maxDivCountNums.push_back(maxDivCount);
    return maxDivCountNums;
}

auto isThreeSquaresSummable(unsigned number) {
    for (unsigned i = 1; i < sqrt(number); ++i) {
        for (unsigned j = 1; j < sqrt(number); ++j) {
            for (unsigned k = 1; k < sqrt(number); ++k) {
                if (number < i*i + j*j + k*k) break;
                if (number == i*i + j*j + k*k) return true;
            }
        }
    }

    return false;
}

auto threeSquaresSum(unsigned input) {
    std::vector<unsigned> output;

    for (unsigned number = 1; number < input; ++number) {
        if (!isThreeSquaresSummable(number))
            output.push_back(number);
    }
    return output;
}

auto getCollatzSteps(unsigned i) {
    unsigned stepsCount = 0;

    while (i != 1) {
        if (i % 2 == 0) {
            i = i / 2;
        } else {
            i = 3*i+1;
        }
        stepsCount++;
    }
    return stepsCount;
}

auto maxStepsCollatzSequence(unsigned input) {
    unsigned maxSteps = 0;
    unsigned currentSteps = 0;
    std::vector<unsigned> maxStepsNumbers;

    for (unsigned i = 1; i < input; ++i) {
        currentSteps = getCollatzSteps(i);

        if (currentSteps > maxSteps) {
            maxSteps = currentSteps;
            maxStepsNumbers.clear();
            maxStepsNumbers.push_back(i);
        } else if (currentSteps == maxSteps) {
            maxStepsNumbers.push_back(i);
        }
    }

    maxStepsNumbers.push_back(maxSteps);
    return maxStepsNumbers;
}

void drawLadder(Point A, Point B, Point C, Point D, SVGFile& svgFile, double frequency = 0) {
    svgFile.addLine(A, B, true);
    svgFile.addLine(C, D, true);

    double lambda = 0.0;
    while (lambda < 1.0) {
        svgFile.addLine(A + ((B - A) * lambda), C + ((D - C) * lambda), true);
        if (frequency != 0) {
            lambda += frequency;
        } else {
            //TODO add random increment
        }
    }
    svgFile.addLine(B, D, true);
}

void drawStar(SVGFile& svgFile, unsigned starPoints, double frequency = 0) {
    for (unsigned i = 0; i < starPoints; ++i) {
        Point A = {cos(2*M_PI/starPoints * i), sin(2*M_PI/starPoints * i)};
        Point B = {cos(2*M_PI/starPoints * (i+1)), sin(2*M_PI/starPoints * (i+1))};
        drawLadder({0.0, 0.0}, A, B, {0.0,0.0}, svgFile, frequency);
    }
}

void generateUlamsSpiral(SVGFile& svgFile, unsigned n, unsigned pixelRadius, double spacing = 0.02) {
    Point A = {0,0};
    Point B = {1,0};
    svgFile.addLine({A.X*spacing, A.Y*spacing}, {B.X*spacing, B.Y*spacing}, true);
    A = B;
    for (unsigned i = 2; i < n; ++i) {
        unsigned checkCount = 0;
        if ((A.X >= A.Y+1) && (A.Y > -A.X)) {B = {B.X, B.Y-1}; ++checkCount;}
        if ((A.Y <= -A.X) && (A.X > A.Y)) {B = {B.X-1, B.Y}; ++checkCount;}
        if ((A.X <= A.Y) && (A.Y < -A.X)) {B = {B.X, B.Y+1}; ++checkCount;}
        if ((A.Y >= -A.X) && (A.Y+1 > A.X)) {B = {B.X+1, B.Y}; ++checkCount;}

        assert(checkCount == 1);

        svgFile.addLine({A.X*spacing, A.Y*spacing}, {B.X*spacing, B.Y*spacing}, true);
        if (i % 19 == 0) {
            svgFile.addCircle({A.X * spacing, A.Y * spacing}, pixelRadius, true, true);
        }
        A = B;
    }
}


int main() {
    /*auto maxDivisorsVector = maxDivisors(10000);
    for (auto number : maxDivisorsVector) {
        std::cout << number << std::endl;
    }*/

    /*auto TSS = threeSquaresSum(1000);
    for (auto number : TSS) {
        std::cout << number << ", ";
    }
    std::cout << std::endl << TSS.size();*/

    /*auto maxCollSteps = maxStepsCollatzSequence(10000);
    for (auto num : maxCollSteps) {
        std::cout << num << std::endl;
    }*/

    /*cv::Mat picture(1000, 1000, CV_16UC3, cv::Scalar(52000, 52000, 52000));
    for (unsigned i = 0; i < picture.cols; ++i) {
        for (unsigned j = 0; j < picture.rows; ++j) {
            picture.at<cv::Vec3w>(j,i) = cv::Vec3w(j*65535/1000, 0, i*65535/1000);
        }
    }
    cv::imwrite("task1B.png", picture);*/

    /*
    SVGFile svgFile("task1B.svg", 1000, 1000);
    drawStar(svgFile, 20, 0.05);
    */

    SVGFile svgFile("task1C.svg", 1000, 1000);
    //svgFile.addCircle({0.5, 0.5}, 50, true, true);
    //svgFile.addLine({0, 0.1}, {0.1, 0.5}, true);
    generateUlamsSpiral(svgFile, 10000, 3);


    return 0;
}