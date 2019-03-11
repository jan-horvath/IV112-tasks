#include <iostream>
#include "../framework/framework.hpp"

using namespace std;
using namespace svg;

void drawPolygon(Turtle& t, unsigned n) {
    t.forward(400);
    t.left(90 + 360.0/n);
    double side = 400*2*sin(M_PI/n);
    for (size_t i = 0; i < n; ++i) {
        t.forward(side);
        t.left(360.0/n);
    }
}

int main() {
    Turtle t("task3A.svg", 1000, 1000);
    drawPolygon(t, 8);


    return 0;
}
