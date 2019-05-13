
#ifndef IV112_TASKS_LABYRINTH_H
#define IV112_TASKS_LABYRINTH_H

#include <vector>
#include <framework.hpp>
#include "framework.hpp"

using namespace std;

namespace Labyrinth {
    struct Cell {
        vector<Cell*> neighbours; //pair<Box*, value>
    };

    /*class LabyrinthStructure {
    public:
        virtual Box& getBox(int x, int y, int z) = 0;
        //TODO virtual void drawStructure(svg::SVGFile& file) = 0;
    };*/

    class Labyrinth { //only square for now
    public:
        Labyrinth(unsigned X, unsigned Y, bool full = true) {
            if (full) {
                //_cells =
                auto x = vector<Cell>(Y, vector<Cell>(X, Cell(4, {nullptr, nullptr, nullptr, nullptr})));
            }
        }
    private:
        vector<vector<Cell>> _cells;
    };
}
#endif //IV112_TASKS_LABYRINTH_H
