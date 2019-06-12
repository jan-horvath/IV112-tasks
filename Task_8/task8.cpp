#include <iostream>
#include "../framework/framework.hpp"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace svg;

using Object = vector<LineSegment>;

const ushort MAX_COL = 65535;
const auto BLACK = cv::Vec3w(0,0,0);
const auto WHITE = cv::Vec3w(MAX_COL,MAX_COL,MAX_COL);

Object createRect(double X, double Y, double centerX, double centerY) {
    Object rect;

    Point p1(centerX - X/2, centerY - Y/2);
    Point p2(centerX + X/2, centerY - Y/2);
    Point p3(centerX + X/2, centerY + Y/2);
    Point p4(centerX - X/2, centerY + Y/2);

    rect.emplace_back(p1, p2);
    rect.emplace_back(p2, p3);
    rect.emplace_back(p3, p4);
    rect.emplace_back(p4, p1);

    return move(rect);
}

Object createArrow(double X, double Y, double centerX, double centerY) {
    Object arrow;

    Point p1(centerX, centerY + Y/2);
    Point p2(centerX, centerY - Y/2);
    Point p3(centerX - X/2, centerY + Y/4);
    Point p4(centerX + X/2, centerY + Y/4);
    Point p5(centerX - X/2, centerY - Y/2);

    arrow.emplace_back(p1, p2);
    arrow.emplace_back(p1, p3);
    arrow.emplace_back(p1, p4);
    arrow.emplace_back(p2, p5);

    return move(arrow);
}

Object createStar(double radius, double centerX, double centerY) {
    Object star;

    Point p1(centerX + radius*cos(0)           , centerY + radius*sin(0));
    Point p2(centerX + radius*cos(2.0/3 * M_PI), centerY + radius*sin(2.0/3 * M_PI));
    Point p3(centerX + radius*cos(4.0/3 * M_PI), centerY + radius*sin(4.0/3 * M_PI));

    Point p4(centerX + radius*cos(1.0/3 * M_PI), centerY + radius*sin(1.0/3 * M_PI));
    Point p5(centerX + radius*cos(M_PI)        , centerY + radius*sin(M_PI));
    Point p6(centerX + radius*cos(5.0/3 * M_PI), centerY + radius*sin(5.0/3*M_PI));

    star.emplace_back(p1, p2);
    star.emplace_back(p2, p3);
    star.emplace_back(p3, p1);

    star.emplace_back(p4, p5);
    star.emplace_back(p5, p6);
    star.emplace_back(p6, p4);

    return move(star);
}

vector<Object> repeatedTransformation(Object obj, const Matrix& trans, unsigned iterations) {
    vector<Object> objects;
    objects.reserve(iterations);
    for (unsigned i = 0; i < iterations; ++i) {
        objects.emplace_back(obj);
        obj = transformObject(obj, trans);
    }
    return move(objects);
}

Object upscaleObject(Object object, double width, double height) {
    for (LineSegment& segment : object) {
        segment.P1.X *= width;
        segment.P1.Y *= height;
        segment.P2.X *= width;
        segment.P2.Y *= height;
    }
    return move(object);
}

void MRCM(SVGFile &file, const Object& object, const vector<Matrix>& transformations, unsigned iterations) {
    if (iterations == 0) {
        file.addObject(upscaleObject(object, file.m_width, file.m_height));
        return;
    }

    for (const auto& t: transformations) {
        MRCM(file, transformObject(object, t), transformations, iterations-1);
    }
}

void approximatedInfiniteMRCM(cv::Mat &file, const vector<Matrix>& transformations, unsigned iterations) {
    svg::Point P(0.5, 0.5);
    for (unsigned i = 0; i < iterations; ++i) {
        if (i > iterations/100) {
            file.at<cv::Vec3w>(P.Y * file.rows, P.X * file.cols) = BLACK;
        }
        P.transform(transformations.at(rand() % transformations.size()));
    }
}


int main() {
    /*SVGFile fileA("task8A.svg", 1000, 1000);
    Object squareA = createRect(50, 50, 500, 450);
    Matrix mA = Matrix::composeTransformation({Matrix::rotation(20, 500, 500),
                                              Matrix::scaling(1.08, 1.08, 500, 500)});
    auto objectsA = repeatedTransformation(squareA, mA, 30);
    for (const Object& o : objectsA) {
        fileA.addObject(o);
    }

    SVGFile fileB("task8B.svg", 1000, 1000);
    Object squareB = createRect(400, 400, 500, 500);
    Matrix mB = Matrix::composeTransformation({Matrix::rotation(5, 500, 500),
                                               Matrix::scaling(1.05, 0.9, 500, 500)});
    auto objectsB = repeatedTransformation(squareB, mB, 90);
    for (const Object& o : objectsB) {
        fileB.addObject(o);
    }

    SVGFile fileC("task8C.svg", 1000, 1000);
    Object squareC = createRect(100, 100, 200, 500);
    Matrix mC = Matrix::composeTransformation({Matrix::shear(-1.3, 0, 500, 500),
                                               Matrix::rotation(10, 500, 500),
                                               Matrix::scaling(0.9, 0.9, 500, 500)});

    auto objectsC = repeatedTransformation(squareC, mC, 90);
    for (const Object& o : objectsC) {
        fileC.addObject(o);
    }

    SVGFile fileD("task8D.svg", 1000, 1000);
    Object line;
    line.segments = {LineSegment({700, 93}, {700, 907})};
    line.basisVectorA = LineSegment({699, 501}, {701, 501});
    line.basisVectorA = LineSegment({699, 501}, {699, 499});

    Matrix mD = Matrix::rotation(7.5, 500, 500);
    auto objectsD = repeatedTransformation(line, mD, 48);
    for (const Object& o : objectsD) {
        fileD.addObject(o);
    }*/

    /*for (unsigned i = 0; i < 64; ++i) {
        SVGFile fileMRCM("MRCM/MRCM-Sierp" + to_string(i) + ".svg", 1000, 1000);
        Object MRCM_sierp= createStar(0.4, 0.5, 0.5);
        vector<Matrix> transformations;
        transformations.emplace_back(Matrix::rotation((i / 16) * 90, 0.25, 0.25) * Matrix::scaling(0.5, 0.5));
        transformations.emplace_back(Matrix::translation(0.25, 0.5) * Matrix::rotation((i /  4) * 90, 0.25, 0.25) * Matrix::scaling(0.5, 0.5));
        transformations.emplace_back(Matrix::translation(0.5, 0) * Matrix::rotation((i %  4) * 90, 0.25, 0.25) * Matrix::scaling(0.5, 0.5));
        MRCM(fileMRCM, MRCM_sierp, transformations, 7);
    }*/

    SVGFile fileFern("MRCM-Fern.svg", 1000, 1000);

    cv::Mat fern(1000, 1000, CV_16UC3, WHITE);

    Object arrow= createArrow(0.2, 0.4, 0.5, 0.5);

    vector<Matrix> transformations;
    transformations.emplace_back(Matrix({0.849, 0.037, 0.075, -0.037, 0.849, 0.183, 0, 0, 1},3,3));
    transformations.emplace_back(Matrix({0.197, -0.226, 0.4, 0.226, 0.197, 0.049, 0, 0, 1}, 3, 3));
    transformations.emplace_back(Matrix({-0.15, 0.283, 0.575, 0.26, 0.237, 0.084, 0, 0, 1}, 3, 3));
    transformations.emplace_back(Matrix({0, 0, 0.5, 0, 0.16, 0, 0, 0, 1}, 3, 3));

    MRCM(fileFern, arrow, transformations, 7);

    approximatedInfiniteMRCM(fern, transformations, 5000000);
    cv::imwrite("MRCM-Fern.png", fern);
    return 0;
}
