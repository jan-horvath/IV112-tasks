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

struct ImplicitLine {
    double A;
    double B;
    double C;

    void computeC(double X, double Y) {
        C = -(A*X + B*Y);
    }

    double getValue(double X, double Y) {
        return A*X + B*Y + C;
    }
};

double distance(double x1, double y1, double x2, double y2) {
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

void equaliteralTriangle(Mat &pic, double minX, double maxY, double sideLength) {
    ImplicitLine p{0, -sideLength};
    p.computeC(minX, maxY);
    ImplicitLine q{-sqrt(3)/2*sideLength, sideLength/2};
    q.computeC(minX+sideLength, maxY);
    ImplicitLine r{sqrt(3)/2*sideLength, sideLength/2};
    r.computeC(minX, maxY);

    for (size_t i = 0; i < pic.cols; ++i) {
        for (size_t j = 0; j < pic.rows; ++j) {
            if ((p.getValue(i, j) >= 0) && (q.getValue(i, j) >= 0) && (r.getValue(i,j) >= 0)) {
                pic.at<Vec3w>(j,i) = Vec3w(MAX_COL*distance(i,j,minX, maxY)/sideLength,
                                           MAX_COL*distance(i,j,minX + sideLength, maxY)/sideLength,
                                           MAX_COL*distance(i,j,minX+sideLength/2, maxY-sqrt(3)/2*sideLength)/sideLength);
            }
        }
    }
}

void ellipse(Mat &pic, double x1, double y1, double x2, double y2, double maxDistance) {
    double focusDistance = distance(x1, y1, x2, y2);

    for (size_t i = 0; i < pic.cols; ++i) {
        for (size_t j = 0; j < pic.rows; ++j) {
            double distanceSum = distance(x1, y1, i, j) + distance(x2, y2, i, j);
            if (distanceSum <= maxDistance) {
                double colorIntensity = (distanceSum - focusDistance)/(maxDistance - focusDistance);
                pic.at<Vec3w>(j,i) = Vec3w(MAX_COL*colorIntensity ,
                                           MAX_COL*colorIntensity,
                                           MAX_COL*colorIntensity);
            }
        }
    }
}

/*void lineBresen(Mat &pic, unsigned x1, unsigned y1, unsigned x2, unsigned y2) {
    if (x2 == x1) {
        for (; y2 != y1; y2 += (y2 < y1) ? 1 : -1) {
            pic.at<Vec3w>(y2,x2) = BLACK;
        }
        return;
    }

    double deltaX = x2 - x1;
    double deltaY = y2 - y1;
    double deltaerr = abs(deltaY/deltaX);
    double error = 0.0;
    int y = y1;
    for (unsigned x = x1; x != x2; )
}*/

void line(Mat &pic, double x1, double y1, double x2, double y2) {
    double step = 1.0/(abs(y2 - y1) + abs(x2 - x1));///distance(x1, y1, x2, y2); //
    double x = x1;
    double y = y1;
    for (double lambda = 0.0; lambda <= 1.0; lambda += step) {
        pic.at<Vec3w>(lround(x),lround(y)) = 0;
        x += step * (x2-x1);
        y += step * (y2-y1);
    }
}

void fillPolygonRec(Mat &pic, unsigned x, unsigned y) {
    if (pic.at<Vec3w>(y, x).val[0] == MAX_COL) {
        while (y != 0 && pic.at<Vec3w>(y-1, x).val[0] == MAX_COL) {y -= 1;}

        while (pic.at<Vec3w>(y, x).val[0] == MAX_COL) {
            pic.at<Vec3w>(y, x) = BLACK;
            if (x != 0) {
                fillPolygonRec(pic, x-1, y);
            }
            if (x != pic.cols-1) {
                fillPolygonRec(pic, x+1, y);
            }
            y += 1;
        }
    }
}

void invertColors(Mat &pic) {
    for (unsigned i = 0; i < pic.cols; ++i) {
        for (unsigned j = 0; j < pic.rows; ++j) {
            if (pic.at<Vec3w>(j, i).val[0] == 0) {
                pic.at<Vec3w>(j, i) = WHITE;
            } else {
                pic.at<Vec3w>(j, i) = BLACK;
            }
        }
    }
}

void fillPolygon(Mat &pic) {
    fillPolygonRec(pic, pic.cols/2, pic.rows/2);
    if ((pic.at<Vec3w>(0,0).val[0] == 0) &&
        (pic.at<Vec3w>(0,pic.cols-1).val[0] == 0) &&
        (pic.at<Vec3w>(pic.rows-1,0).val[0] == 0) &&
        (pic.at<Vec3w>(pic.rows-1,pic.cols-1).val[0] == 0)) {
        invertColors(pic);
    }

}

void polygon(Mat &pic, const std::vector<Point2i> &points) {
    for (unsigned i = 0; i < points.size()-1; ++i) {
        line(pic, points[i].x, points[i].y, points[i+1].x, points[i+1].y);
    }
    line(pic, points.back().x, points.back().y, points[0].x, points[0].y);
    fillPolygon(pic);
}


int main() {

    /*cv::Mat picture(1000, 1000, CV_16UC3, WHITE);
    drawCircleImp(picture, 200, 120, 100, false);
    drawCircleImp(picture, 800, 120, 100, true);
    drawSpiral(picture, {780, 780}, 200);
    equaliteralTriangle(picture, 100, 900, 300);
    ellipse(picture, 400, 600, 600, 400, 400);
    cv::imwrite("task4A.png", picture);*/

    cv::Mat picture2(500, 500, CV_16UC3, WHITE);
    polygon(picture2, {{50, 50}, {250, 300}, {450, 50}, {350, 250},
                       {450, 450}, {250, 350}, {50, 450}, {150, 250}});
    imwrite("task4B.png", picture2);



    return 0;
}
