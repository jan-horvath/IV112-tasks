#include <iostream>
#include "../framework/labyrinth.hpp"
#include "../framework/framework.hpp"

using namespace std;
using namespace Labyrinth;
using namespace svg;

int main() {
    TriangleGraph TG(40);
    TG.generateLabyrinth();
    SVGFile tg_file("TG1.svg", 1000, 1000);
    TG.drawLabyrinth(tg_file);
    return 0;
}