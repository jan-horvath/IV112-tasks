
#ifndef IV112_TASKS_LABYRINTH_H
#define IV112_TASKS_LABYRINTH_H

#include <vector>
#include <framework.hpp>
#include "framework.hpp"

using namespace std;

namespace Labyrinth {
    class Graph {
    public:

        void setStart(int X, int Y, int Z) {
            assert(X < m_cells.size() && Y < m_cells[X].size() && Z < m_cells[X][Y].size());
            m_start = &m_cells[X][Y][Z];
        }

        void setTarget(int X, int Y, int Z) {
            assert(X < m_cells.size() && Y < m_cells[X].size() && Z < m_cells[X][Y].size());
            m_target = &m_cells[X][Y][Z];
        }

    private:
        vector<vector<vector<Cell>>> m_cells;
        Cell* m_start;
        Cell* m_target;
    };

    struct Cell {
        vector<Cell*> m_neighbours;
        //int value;
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
                auto x = vector<Cell>(Y, vector<Cell>(X, {nullptr, nullptr, nullptr, nullptr}));
            }
        }
    private:
        vector<vector<Cell>> _cells;
    };
}
#endif //IV112_TASKS_LABYRINTH_H
