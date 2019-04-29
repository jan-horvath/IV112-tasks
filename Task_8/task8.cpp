#include <iostream>
#include "../framework/framework.hpp"

using namespace std;
using namespace svg;

Object createRect(double X, double Y, double centerX, double centerY) {
    Object rect;

    Point p1(centerX - X/2, centerY - Y/2);
    Point p2(centerX + X/2, centerY - Y/2);
    Point p3(centerX + X/2, centerY + Y/2);
    Point p4(centerX - X/2, centerY + Y/2);

    rect.segments.emplace_back(p1, p2);
    rect.segments.emplace_back(p2, p3);
    rect.segments.emplace_back(p3, p4);
    rect.segments.emplace_back(p4, p1);

    rect.basisVectorA = {p4, p1};
    rect.basisVectorB = {p4, p3};

    return move(rect);
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

void MRCM(SVGFile &file, const Object& object, const vector<function<Matrix(const Object&)>>& transGen, unsigned iterations) {
    if (iterations == 0) {
        file.addObject(object);
        return;
    }

    for (const auto& generator: transGen) {
        //MRCM(file, transformObject(object, generator(object)), transGen, iterations-1);
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

    /*using namespace placeholders;
    const vector<function<Matrix(const Object&)>> tranformationGenerators;
    function<Matrix(const Object&)> func1 =
            bind(Matrix::composeTransformation, {bind(&Object::translationMatrix, _1, 0.5, 0), bind(&Object::scalingMatrix, _1, 0.5, 0)});
*/
    return 0;
}
