#include <iostream>
#include "../framework/framework.hpp"

using namespace std;
using namespace svg;

void drawPolygon(Turtle& t, unsigned n, double radius) {
    t.drawing(false);
    t.forward(radius);
    t.drawing(true);
    t.left(90 + 360.0/n/2);
    double side = radius*2*sin(M_PI/n);
    for (size_t i = 0; i < n; ++i) {
        t.forward(side);
        t.left(360.0/n);
    }
    t.right(90 + 360.0/n/2);
    t.drawing(false);
    t.back(radius);
    t.drawing(true);
}

void drawStar(Turtle& t, unsigned n, double radius) {
    double half_angle = 90.0 - 360.0/n;
    double side = 2*radius*cos(half_angle * M_PI/180);

    t.drawing(false);
    t.forward(radius);
    t.drawing(true);
    t.left(180-half_angle);

    for (unsigned i = 0; i < n; ++i) {
        t.forward(side);
        t.left(180-2*half_angle);
    }
    t.right(180-half_angle);
    t.drawing(false);
    t.back(radius);
    t.drawing(true);
}

void drawPentagramR(Turtle& t, double radius) {
    drawPolygon(t, 5, radius);
    drawStar(t, 5, radius);
}

void drawPentagramA(SVGFile& file, double radius) {
    unsigned n = 5;
    double angle = 360.0/n * M_PI/180;

    for (unsigned i = 0; i < n; ++i) {
        Point from = {cos(angle*i)*radius, sin(angle*i)*radius};
        Point to1 = {cos(angle*(i+1))*radius, sin(angle*(i+1))*radius};
        Point to2 = {cos(angle*floor(i+n/2))*radius, sin(angle*floor(i+n/2))*radius};

        file.addLine(from + Vector{file.m_height/2, file.m_width/2}, to1 + Vector{file.m_height/2, file.m_width/2});
        file.addLine(from + Vector{file.m_height/2, file.m_width/2}, to2 + Vector{file.m_height/2, file.m_width/2});
    }
}

void recursiveSquare(Turtle& t, unsigned depth, double sideLength, double param) {
    t.drawing(false);
    t.forward(sideLength/2);
    t.right(90);
    t.back(sideLength/2);
    t.drawing(true);

    for (unsigned i = 0; i < depth; ++i) {
        for (unsigned j = 0; j < 4; ++j) {
            t.forward(sideLength);
            t.right(90);
        }
        t.forward(sideLength*param);
        t.right(atan(param/(1-param)), true);
        sideLength *= sqrt(2*param*param - 2*param + 1);
    }
}

void circularLattice(SVGFile& file, double radius, double spacing) {
    double distanceFromCenter = 0.0;
    double epsilon = 0.1;
    radius += epsilon;
    while (distanceFromCenter <= radius) {
        double X = distanceFromCenter;
        double Y = sqrt(radius * radius - X * X);
        double H = file.m_height/2;
        double W = file.m_width/2;

        file.addLine({H - Y, W + X}, {H + Y, W + X});
        file.addLine({H - Y, W - X}, {H + Y, W - X});
        file.addLine({H + X, W + Y}, {H + X, W - Y});
        file.addLine({H - X, W + Y}, {H - X, W - Y});

        distanceFromCenter += spacing;
    }
}

void polygonBowl(Turtle& t, unsigned n, double radius) {
    t.drawing(false);
    t.forward(radius);
    t.left(90 + 360.0/n/2);
    double side = radius*2*sin(M_PI/n);
    for (size_t i = 0; i < n; ++i) {

        t.left(90 + 360.0/n/2);
        drawPolygon(t, n, radius);
        t.right(90 + 360.0/n/2);

        t.drawing(false);
        t.forward(side);
        t.left(360.0/n);
    }
    t.right(90 + 360.0/n/2);
    t.back(radius);
    t.drawing(true);
}

void recBush(Turtle& t, double length, unsigned depth, unsigned seed) {
    if (depth == 0) {return;}

    t.forward(length);

    double angle = seed % 50 + 10;
    double lengthModifier1 = 0.5 + ((float)(seed % 17)) / 37;
    double lengthModifier2 = 0.5 + ((float)(seed % 23)) / 51;

    t.left(angle);
    recBush(t, length*lengthModifier1, depth-1, seed);
    t.right(2*angle);
    recBush(t, length*lengthModifier2, depth-1, seed);
    t.left(angle);
    t.back(length);
}

int main() {

    //TASK A
    Turtle tA("task3A.svg", 1000, 1000);
    for (unsigned i = 1; i < 4; ++i) {
        drawPolygon(tA, 3*i, 400);
    }
    tA.left(90);

    drawStar(tA, 5, 50);
    drawStar(tA, 5, 200);

    //TASK B
    Turtle A("task3B-A1.svg", 1000, 1000);
    SVGFile file("task3B-A2.svg", 1000, 1000);
    drawPentagramR(A, 400);
    drawPentagramA(file, 400);

    Turtle B1("task3B-B1", 1000, 1000);
    recursiveSquare(B1, 150, 50, 1.01);
    Turtle B2("task3B-B2", 1000, 1000);
    recursiveSquare(B2, 50, 600, 0.1);

    SVGFile C("task3B-C.svg", 1000, 1000);
    circularLattice(C, 400, 15);

    Turtle D("task3B-D.svg", 1000, 1000);
    D.left(90);
    for (unsigned i = 0; i < 30; ++i) {
        drawPolygon(D, 3, 10*i+4);
    }

    Turtle E("task3B-E.svg", 1000, 1000);
    E.left(90);
    polygonBowl(E, 20, 200);

    //TASK C
    Turtle tC("task3C-A.svg", 1000, 1000);
    tC.left(90);
    tC.drawing(false);
    tC.back(300);
    tC.drawing(true);
    recBush(tC, 120, 10, 11111111);


    return 0;
}
