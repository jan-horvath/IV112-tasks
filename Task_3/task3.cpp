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


void kochRec(Turtle &t, double length, unsigned depth) {
    if (depth == 0) {
        t.forward(length);
        return;
    }
    kochRec(t, length/3, depth-1);
    t.left(60);
    kochRec(t, length/3, depth-1);
    t.right(120);
    kochRec(t, length/3, depth-1);
    t.left(60);
    kochRec(t, length/3, depth-1);
}

void koch(Turtle& t, double radius, unsigned depth) {
    t.drawing(false);
    t.left(60);
    t.forward(radius);
    t.right(180-30);
    t.drawing(true);

    kochRec(t, 2 * radius * sin(M_PI/3), depth);
    t.right(120);
    kochRec(t, 2 * radius * sin(M_PI/3), depth);
    t.right(120);
    kochRec(t, 2* radius * sin(M_PI/3), depth);
}

void sierpinskiRec(Turtle &t, double radius, unsigned depth) {
    drawPolygon(t, 3, radius);
    t.drawing(false);

    if (depth > 0) {
        for (unsigned i = 0; i < 3; ++i) {
            t.drawing(false);
            t.back(radius);
            sierpinskiRec(t, radius/2, depth - 1);
            t.forward(radius);
            t.left(120);
        }
    }
}

void sierpinski(Turtle &t, double radius, unsigned depth) {
    drawPolygon(t, 3, radius);
    t.drawing(false);
    t.left(180);

    if (depth > 0) {
        sierpinskiRec(t, radius/2, depth - 1);
    }
    t.drawing(true);
}

/*void hilbert(Turtle &t, double sideLength, unsigned depth) {
    t.drawing(false);
    t.back(sideLength/2);
    t.right(90);
    t.back(sideLength/2);
    t.left(90);
}*/

void polySnowflake(Turtle &t, unsigned polyDegree, double radius, unsigned depth) {
    if (depth == 0) {
        if (polyDegree % 2 == 0) {t.left(180.0/polyDegree);}
        drawPolygon(t, polyDegree, radius);
        if (polyDegree % 2 == 0) {t.right(180.0/polyDegree);}
        return;
    }

    double polygonTriangleHeight = radius/(2+1.0/cos(M_PI/polyDegree));
    double newRadius = radius - 2 * polygonTriangleHeight;

    if (polyDegree % 2 == 1) {t.left(180.0/polyDegree);}
    polySnowflake(t, polyDegree, newRadius, depth-1);
    if (polyDegree % 2 == 1) {t.right(180.0/polyDegree);}
    t.drawing(false);
    for (unsigned i = 0; i < polyDegree; ++i) {
        t.forward(2*polygonTriangleHeight);
        polySnowflake(t, polyDegree, newRadius, depth-1);
        t.drawing(false);
        t.back(2*polygonTriangleHeight);
        t.left(360.0/polyDegree);
    }
}

void drawLadder(SVGFile& file, Point A, Point B, Point C, Point D, double frequency) {
    file.addLine(A, B, COLORS[1]);
    file.addLine(C, D, COLORS[1]);

    double lambda = 0.0;
    while (lambda < 1.0) {
        file.addLine(A + ((B - A) * lambda), C + ((D - C) * lambda), COLORS[13]);
        lambda += frequency;
    }
    file.addLine(B, D, COLORS[13]);
}

void drawHexaladder(SVGFile& file, Point center, double radius) {
    for (unsigned i = 0; i < 6; ++i) {
        Point A = {radius*cos(i*M_PI/3) + center.X, radius * sin(i*M_PI/3) + center.Y};
        Point B = {radius*cos((i+1)*M_PI/3) + center.X, radius * sin((i+1)*M_PI/3) + center.Y};
        Point C = {radius*cos((i+2)*M_PI/3) + center.X, radius * sin((i+2)*M_PI/3) + center.Y};
        Point D = {radius*cos((i+3)*M_PI/3) + center.X, radius * sin((i+3)*M_PI/3) + center.Y};
        drawLadder(file, A, B, C, D, 1.0/3);
    }
}

void customFractal1(SVGFile& file, Point center, double radius, unsigned depth) {
    if (depth == 0) {
        drawHexaladder(file, center, radius);
        return;
    }

    double centerRadius = sqrt(3) * 1.0/3 * radius;
    customFractal1(file, center, radius/3, depth-1);
    for (unsigned i = 0; i < 6; ++i) {
        Point newCenter = {centerRadius*cos(i*M_PI/3 + M_PI/6) + center.X, centerRadius * sin(i*M_PI/3 + M_PI/6) + center.Y};
        customFractal1(file, newCenter, radius/3, depth-1);
    }
}

void customFractal2(Turtle &t, double length, unsigned depth) {
    if (depth == 0) {
        t.forward(length);
        return;
    }

    double quarter = length/4;

    customFractal2(t, 1.5*quarter - quarter/sqrt(2), depth-1);
    t.left(45);
    customFractal2(t, length/4, depth-1);
    t.left(45);
    customFractal2(t, length/4, depth-1);
    t.right(45);
    customFractal2(t, length/4, depth-1);
    t.right(45);
    customFractal2(t, length/4, depth-1);
    t.right(180-45);
    customFractal2(t, length/4, depth-1);
    t.left(45);
    customFractal2(t, length/4, depth-1);
    t.left(45);
    customFractal2(t, length/4, depth-1);
    t.left(45);
    customFractal2(t, 1.5*quarter - quarter/sqrt(2), depth-1);
}

void customFractal3(Turtle &t, double length, unsigned depth) {
    if (depth == 0) {
        t.forward(length);
        return;
    }

    double quarter = length/4;

    customFractal3(t, quarter, depth-1);
    t.left(45);
    customFractal3(t, quarter*sqrt(2), depth-1);
    t.left(45);
    customFractal3(t, quarter, depth-1);
    t.right(180);
    customFractal3(t, quarter, depth-1);
    t.left(45);
    customFractal3(t, quarter*sqrt(2), depth-1);
    t.left(45);
    customFractal3(t, quarter, depth-1);

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

    Turtle B1("task3B-B1.svg", 1000, 1000);
    Turtle B2("task3B-B2.svg", 1000, 1000);
    recursiveSquare(B1, 150, 50, 1.01);
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
    Turtle Ca("task3C-A.svg", 1000, 1000);
    Ca.left(90);
    Ca.drawing(false);
    Ca.back(300);
    Ca.drawing(true);
    recBush(Ca, 120, 10, 11111111);


    Turtle Cb("task3C-B.svg", 1000, 1000);
    Cb.left(90);
    koch(Cb, 400, 5);

    Turtle Cc("task3C-C.svg", 1000, 1000);
    Cc.drawing(false);
    Cc.left(90);
    Cc.back(100);
    sierpinski(Cc, 500, 8);

    Turtle Ce("task3C-E.svg", 1000, 1000);
    polySnowflake(Ce, 5, 500, 3);

    SVGFile D1("task3D-1.svg", 1250, 1250, COLORS[0]);
    customFractal1(D1, {625, 625}, 700, 1);

    Turtle D2("task3D-2.svg", 1000, 1000);
    D2.drawing(false);
    D2.back(500);
    D2.left(90);
    D2.back(400);
    D2.right(90);
    D2.drawing(true);
    customFractal2(D2, 1000, 3);

    Turtle D3("task3D-3.svg", 1000, 1000);
    D3.drawing(false);
    D3.back(450);
    D3.left(90);
    D3.back(450);
    D3.right(90);
    D3.drawing(true);
    for (unsigned i = 0; i < 4; ++i) {
        customFractal3(D3, 900, 4);
        D3.left(90);
    }


    return 0;
}
