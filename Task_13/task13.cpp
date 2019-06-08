#include <iostream>
#include "../framework/labyrinth.hpp"
#include "../framework/framework.hpp"

using namespace std;
using namespace Labyrinth;
using namespace svg;

int main() {
    cout << "Hello world!" << endl;

    TriangleGraph TG(10);
    SVGFile tg_file("TG1.svg", 1000, 1000);
    TG.drawLabyrinth(tg_file);
    return 0;
}