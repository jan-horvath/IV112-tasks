#include <iostream>
#include <complex>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <thread>

using namespace std;
using namespace cv;

typedef complex<double> DComp;
const DComp sol1(1,0);
const DComp sol2(-0.5,sqrt(3)/2);
const DComp sol3(-0.5,-sqrt(3)/2);

const ushort MAX_COL = 65535;
const auto BLACK = cv::Vec3w(0,0,0);
const auto WHITE = cv::Vec3w(MAX_COL,MAX_COL,MAX_COL);

Vec3w Newton(DComp cnum, unsigned iter) {
    for (unsigned i = 0; i < iter; ++i) {
        cnum = cnum - (pow(cnum, 3) - DComp(1))/(DComp(3) * pow(cnum,2));
    }

    double distances[3] = {abs(cnum-sol1), abs(cnum-sol2), abs(cnum-sol3)};
    double maxDistance = *max_element(distances, distances + 3);
    return Vec3w(
            static_cast<ushort>(floor(distances[0]/maxDistance * MAX_COL)),
            static_cast<ushort>(floor(distances[1]/maxDistance * MAX_COL)),
            static_cast<ushort>(floor(distances[2]/maxDistance * MAX_COL)));

}

Vec3w Mandelbrot(DComp cnum, unsigned iter) {
    DComp z(0.0, 0.0);
    for (unsigned i = 0; i < iter; ++i) {
        z = z*z + cnum;
    }

    if (abs(z) < 2) {
        return BLACK;
    }
    return WHITE;
}

Vec3w Julia(DComp cnum, DComp fixedC, unsigned iter) {
    for (unsigned i = 0; i < iter; ++i) {
        cnum = cnum*cnum + fixedC;
    }

    if (abs(cnum) < 2) {
        return BLACK;
    }
    return WHITE;
}

template<char ID>
void Universal(Mat& pic, double minR, double maxR, double minI, double maxI,
        unsigned iter = 30, DComp fixedC = {0.0, 0.0}) {
    double R_increment = (maxR - minR)/pic.cols;
    double I_increment = (maxI - minI)/pic.rows;

    for (size_t y = 0; y < pic.rows; ++y) {
        for (size_t x = 0; x < pic.cols; ++x) {
            double X = minR + x*R_increment;
            double Y = minI + y*I_increment;
            if (ID == 'N') {
                pic.at<Vec3w>(y, x) = Newton(DComp{X, Y}, iter);
            }
            if (ID == 'M') {
                pic.at<Vec3w>(y, x) = Mandelbrot(DComp{X, Y}, iter);
            }
            if (ID == 'J') {
                pic.at<Vec3w>(y, x) = Julia(DComp{X, Y}, fixedC, iter);
            }
        }
    }
}

DComp get2DPoint(unsigned side, double minX, double maxX, double minY, double maxY, unsigned parameter) {
    assert(parameter < side*side);
    double X = (maxX-minX) * static_cast<double>(parameter/side)/(side-1) + minX;
    double Y = ((parameter/side) % 2 == 0) ?
               ((maxY - minY) * static_cast<double>(parameter % side)/(side-1) + minY)
               : ((maxY - minY) * static_cast<double>((side-1) - (parameter % side))/(side-1) + minY);
    return {X,Y};
}

int main() {
    /*Mat pictureA1(1000, 1000, CV_16UC3, WHITE);
    Universal<'N'>(pictureA1, -2, 2, -2, 2);
    imwrite("task7A1.png", pictureA1);

    Mat pictureB1(1000, 1000, CV_16UC3, WHITE);
    Universal<'M'>(pictureB1, -2, 0.5, -1.25, 1.25);
    imwrite("task7B1.png", pictureB1);

    Mat pictureC1(1000, 1000, CV_16UC3, WHITE);
    Universal<'J'>(pictureC1, -2, 2, -2, 2, {0.42, 0.13});
    imwrite("task7C1.png", pictureC1);*/

    /*Mat picture1(1000, 1000, CV_16UC3, WHITE);
    Mat picture2(1000, 1000, CV_16UC3, WHITE);
    Mat picture3(1000, 1000, CV_16UC3, WHITE);
    Mat picture4(1000, 1000, CV_16UC3, WHITE);

    for (unsigned i = 5; i < 7; ++i) {
        thread t1(Universal<'N'>, ref(picture1), -2, 2, -2, 2, 4*i, DComp(0,0));
        thread t2(Universal<'N'>, ref(picture2), -2, 2, -2, 2, 4*i+1, DComp(0,0));
        thread t3(Universal<'N'>, ref(picture3), -2, 2, -2, 2, 4*i+2, DComp(0,0));
        thread t4(Universal<'N'>, ref(picture4), -2, 2, -2, 2, 4*i+3, DComp(0,0));

        t1.join();
        t2.join();
        t3.join();
        t4.join();

        imwrite("Newton/" + to_string(4*i) + ".png", picture1);
        imwrite("Newton/" + to_string(4*i+1) + ".png", picture2);
        imwrite("Newton/" + to_string(4*i+2) + ".png", picture3);
        imwrite("Newton/" + to_string(4*i+3) + ".png", picture4);

        picture1.setTo(WHITE);
        picture2.setTo(WHITE);
        picture3.setTo(WHITE);
        picture4.setTo(WHITE);
    }*/

    Mat picture1(1000, 1000, CV_16UC3, WHITE);
    Mat picture2(1000, 1000, CV_16UC3, WHITE);
    Mat picture3(1000, 1000, CV_16UC3, WHITE);
    Mat picture4(1000, 1000, CV_16UC3, WHITE);

    unsigned pointCount1D = 15;
    double minX = 0.2;
    double maxX = 0.5;
    double minY = 0.2;
    double maxY = 0.5;

    for (unsigned i = 0; i < 55; ++i) {
        thread t1(Universal<'J'>, ref(picture1), -2, 2, -2, 2, 30,
                  get2DPoint(pointCount1D, minX, maxX, minY, maxY, 4*i+0));
        thread t2(Universal<'J'>, ref(picture2), -2, 2, -2, 2, 30,
                  get2DPoint(pointCount1D, minX, maxX, minY, maxY, 4*i+1));
        thread t3(Universal<'J'>, ref(picture3), -2, 2, -2, 2, 30,
                  get2DPoint(pointCount1D, minX, maxX, minY, maxY, 4*i+2));
        thread t4(Universal<'J'>, ref(picture4), -2, 2, -2, 2, 30,
                  get2DPoint(pointCount1D, minX, maxX, minY, maxY, 4*i+3));

        t1.join();
        t2.join();
        t3.join();
        t4.join();

        imwrite("Julia/" + to_string(4*i) + ".png", picture1);
        imwrite("Julia/" + to_string(4*i+1) + ".png", picture2);
        imwrite("Julia/" + to_string(4*i+2) + ".png", picture3);
        imwrite("Julia/" + to_string(4*i+3) + ".png", picture4);

        picture1.setTo(WHITE);
        picture2.setTo(WHITE);
        picture3.setTo(WHITE);
        picture4.setTo(WHITE);
    }

    cout << get2DPoint(5, 0.4, 0.6, 0.4, 0.6, 12);

}