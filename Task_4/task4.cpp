#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

const auto MAX_COL_8BIT = 255;
const auto MAX_COL = 65535;
const auto BLACK = Vec3w(0,0,0);
const auto WHITE = Vec3w(MAX_COL,MAX_COL,MAX_COL);
const auto BLUE = Vec3w(MAX_COL,0,0);
const auto GREEN = Vec3w(0,MAX_COL,0);
const auto RED = Vec3w(0,0,MAX_COL);

void drawCircleImp(Mat &pic, double x, double y, double r, bool fill) {
    const double DELTA = 0.015;

    for (size_t i = 0; i < pic.cols; ++i) {
        for (size_t j = 0; j < pic.rows; ++j) {
            if (!fill) {
                if (abs(sqrt((i - x) * (i - x) + (j - y) * (j - y)) / r - 1) < DELTA) {
                    pic.at<Vec3w>(j, i) = BLACK;
                }
            } else {
                if (sqrt((i - x) * (i - x) + (j - y) * (j - y)) / r - 1 < DELTA) {
                    pic.at<Vec3w>(j, i) = BLACK;
                }
            }
        }
    }
}

void colorQuadpixel(Mat &pic, double X, double Y, const Vec3w &colour) {
    pic.at<Vec3w>(static_cast<int>(floor(Y)), static_cast<int>(floor(X))) = colour;
    pic.at<Vec3w>(static_cast<int>(floor(Y)), static_cast<int>(ceil(X))) = colour;
    pic.at<Vec3w>(static_cast<int>(ceil(Y)), static_cast<int>(floor(X))) = colour;
    pic.at<Vec3w>(static_cast<int>(ceil(Y)), static_cast<int>(ceil(X))) = colour;
}

void drawSpiral(Mat &pic, const Point2d& center, double maxRadius) {
    double radius = maxRadius;
    double radiusDecrement = 0.01;
    double angle = M_PI_4;
    double angleIncrement = 0.005;

    while (radius > 0.5) {
        double X = radius * cos(angle) + center.x;
        double Y = radius * sin(angle) + center.y;
        double ratio = radius/maxRadius;
        colorQuadpixel(pic, X, Y, Vec3w(ratio*MAX_COL, (1-ratio)*MAX_COL, 4*(1-ratio)*ratio*MAX_COL));
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
    double step = 1.0/(abs(y2 - y1) + abs(x2 - x1));
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

void swapColor(Vec3w &color) {
    if (color.val[0] == 0) {
        color = WHITE;
    } else {
        color = BLACK;
    }
}

void optical1(Mat &pic, unsigned squareSize, unsigned circleParam) {
    unsigned centerX = pic.cols/2;
    unsigned centerY = pic.rows/2;
    Vec3w color;

    for (unsigned x = 0; x < pic.cols; ++x) {
        for (unsigned y = 0; y < pic.rows; ++y) {
            color = WHITE;
            if ((((x / squareSize) % 2) + ((y / squareSize) % 2)) == 1) {
                swapColor(color);
            }
            if (lround(distance(x, y, centerX, centerY) / circleParam) % 2 == 1) {
                swapColor(color);
            }
            pic.at<Vec3w>(y, x) = color;
        }
    }
}

void optical2(Mat &pic, double period, unsigned squareParam) {
    double centerX = pic.cols/2;
    double centerY = pic.rows/2;
    double dist;
    double colorIntensity;

    for (unsigned x = 0; x < pic.cols; ++x) {
        for (unsigned y = 0; y < pic.rows; ++y) {
            dist = distance(x, y, centerX, centerY);
            colorIntensity = ((cos(dist/period * 2*M_PI) + 1)/2);
            if ((lround(max(abs(x-centerX), abs(y-centerY))) / squareParam) % 2 == 1) {
                colorIntensity = 1- colorIntensity;
            }
            pic.at<Vec3w>(y, x) = Vec3w(MAX_COL*colorIntensity, MAX_COL*colorIntensity, MAX_COL*colorIntensity);
        }
    }
}

void optical3(Mat &pic, unsigned squareSize) {
    const Vec3w GRAY{MAX_COL/2, MAX_COL/2, MAX_COL/2};
    unsigned lineWidth = 5;
    unsigned adjustment = squareSize * (pic.cols / squareSize + 1);

    for (unsigned x = 0; x < pic.cols; ++x) {
        for (unsigned y = 0; y < pic.rows; ++y) {
            if (((x + squareSize/2) % squareSize <= lineWidth)
            || ((x + squareSize/2) % squareSize >= squareSize-lineWidth)
            || ((y + squareSize/2) % squareSize <= lineWidth)
            || ((y + squareSize/2) % squareSize >= squareSize-lineWidth)
            || ((x - y + adjustment) % squareSize <= sqrt(2)*lineWidth)
            || ((x - y + adjustment) % squareSize >= squareSize-sqrt(2)*lineWidth)
            || ((x + y) % squareSize <= sqrt(2)*lineWidth)
            || ((x + y) % squareSize >= squareSize-sqrt(2)*lineWidth)) {
                pic.at<Vec3w>(y,x) = GRAY;
            }
        }
    }

    for (unsigned x = 0; x < pic.cols; ++x) {
        for (unsigned y = 0; y < pic.rows; ++y) {
            unsigned closestCenterX = squareSize * (x / squareSize + 1) - squareSize/2;
            unsigned closestCenterY = squareSize * (y / squareSize + 1) - squareSize/2;
            if (distance(x, y, closestCenterX, closestCenterY) < 2*lineWidth) {
                pic.at<Vec3w>(y, x) = BLACK;
            }
        }
    }
}

void solveRiddle1(Mat &pic) {
    for (unsigned x = 0; x < pic.cols; ++x) {
        for (unsigned y = 0; y < pic.rows; ++y) {
            pic.at<Vec3w>(y,x).val[0] *= 24;
            pic.at<Vec3w>(y,x).val[1] = 0;
            pic.at<Vec3w>(y,x).val[2] = 0;
        }
    }
}

void solveRiddle2(Mat &pic) {
    for (unsigned x = 0; x < pic.cols-1; ++x) {
        for (unsigned y = 0; y < pic.rows; ++y) {
            pic.at<Vec3w>(y,x).val[0] = 40*abs(pic.at<Vec3w>(y,x).val[0] - pic.at<Vec3w>(y,x+1).val[0]);
            pic.at<Vec3w>(y,x).val[1] = 40*abs(pic.at<Vec3w>(y,x).val[1] - pic.at<Vec3w>(y,x+1).val[1]);
            pic.at<Vec3w>(y,x).val[2] = 40*abs(pic.at<Vec3w>(y,x).val[2] - pic.at<Vec3w>(y,x+1).val[2]);
        }
    }
}

void solveRiddle3(Mat &pic) {
    for (unsigned x = 0; x < pic.cols; ++x) {
        for (unsigned y = 0; y < pic.rows; ++y) {
            if ((x+y) % 2 == 1) {
                pic.at<Vec3w>(y,x).val[0] = MAX_COL - pic.at<Vec3w>(y,x).val[0];
                pic.at<Vec3w>(y,x).val[1] = MAX_COL - pic.at<Vec3w>(y,x).val[1];
                pic.at<Vec3w>(y,x).val[2] = MAX_COL - pic.at<Vec3w>(y,x).val[2];
            }
        }
    }
}

void convertToRiddle(Mat &pic) {
    const uint8_t PRIME = 23;
    const uint8_t ALPHA = MAX_COL_8BIT/PRIME;
    for (unsigned x = 0; x < pic.cols; ++x) {
        for (unsigned y = 0; y < pic.rows-1; ++y) {
            if (pic.at<Vec3b>(y,x).val[0] > pic.at<Vec3b>(y,x).val[1]) {
                pic.at<Vec3b>(y,x) = Vec3b((rand() % ALPHA) * PRIME + 1,
                        (rand() % ALPHA) * PRIME + 1, (rand() % ALPHA) * PRIME + 1);
            } else {
                pic.at<Vec3b>(y,x) = Vec3b((rand() % ALPHA) * PRIME, (rand() % ALPHA) * PRIME, (rand() % ALPHA) * PRIME);
            }
        }
    }
}

void solveRiddle(Mat &pic) {
    const uint8_t PRIME = 23;
    const uint8_t ALPHA = MAX_COL_8BIT/PRIME;
    for (unsigned x = 0; x < pic.cols; ++x) {
        for (unsigned y = 0; y < pic.rows-1; ++y) {
            pic.at<Vec3b>(y,x).val[0] = (pic.at<Vec3b>(y,x).val[0] % PRIME) * MAX_COL_8BIT;
            pic.at<Vec3b>(y,x).val[1] = (pic.at<Vec3b>(y,x).val[1] % PRIME) * MAX_COL_8BIT;
            pic.at<Vec3b>(y,x).val[2] = (pic.at<Vec3b>(y,x).val[2] % PRIME) * MAX_COL_8BIT;
        }
    }
}


int main() {

    /*Mat picture(1000, 1000, CV_16UC3, WHITE);
    drawCircleImp(picture, 200, 120, 100, false);
    drawCircleImp(picture, 800, 120, 100, true);
    drawSpiral(picture, {780, 780}, 200);
    equaliteralTriangle(picture, 100, 900, 300);
    ellipse(picture, 400, 600, 600, 400, 400);
    imwrite("task4A.png", picture);

    Mat picture2(500, 500, CV_16UC3, WHITE);
    polygon(picture2, {{50, 50}, {250, 300}, {450, 50}, {350, 250},
                       {450, 450}, {250, 350}, {50, 450}, {150, 250}});
    imwrite("task4B.png", picture2);

    Mat picture3A(1000, 1000, CV_16UC3, WHITE);
    optical1(picture3A, 40, 125);
    imwrite("task4C-A.png", picture3A);

    Mat picture3B(1000, 1000, CV_16UC3, WHITE);
    optical2(picture3B, 75, 400);
    imwrite("task4C-B.png", picture3B);
    
    Mat picture3C(1000, 1000, CV_16UC3, WHITE);
    optical3(picture3C, 200);
    imwrite("task4C-C.png", picture3C);*/

    auto riddle1 = imread("skryvacka1.png");
    riddle1.convertTo(riddle1, CV_16UC3, 256);
    solveRiddle1(riddle1);
    imwrite("solution1.png", riddle1);

    auto riddle2 = imread("skryvacka2.png");
    riddle2.convertTo(riddle2, CV_16UC3, 256);
    solveRiddle2(riddle2);
    imwrite("solution2.png", riddle2);

    auto riddle3 = imread("skryvacka3.png");
    riddle3.convertTo(riddle3, CV_16UC3, 256);
    solveRiddle3(riddle3);
    imwrite("solution3.png", riddle3);

    auto original = imread("original.png");
    convertToRiddle(original);
    imwrite("riddle.png", original);

    auto riddle = imread("riddle.png");
    solveRiddle(riddle);
    imwrite("solution.png", riddle);

    return 0;
}
