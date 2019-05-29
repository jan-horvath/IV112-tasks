#ifndef IV112_TASKS_LABYRINTH_H
#define IV112_TASKS_LABYRINTH_H

#include <vector>
#include <tuple>
#include <queue>
#include "framework.hpp"

using namespace std;

namespace Labyrinth {
    typedef tuple<unsigned, unsigned, unsigned> Coords;
    struct Cell {
        Cell(Coords coords) : m_out(), m_idx(move(coords)), m_prev(nullptr), m_spl(0), m_processed(false) {}

        vector<pair<Cell*, int>> m_out;
        unsigned m_value;
        const Coords m_idx;

        Cell* m_prev;
        unsigned m_spl;//shortest path length
        bool m_processed;
    };

    bool hasGreaterSpl(Cell* lhs, Cell* rhs) {return lhs->m_spl > rhs->m_spl;}
    bool hasLesserSpl(Cell* lhs, Cell* rhs) {return hasGreaterSpl(rhs, lhs);}



    class Graph {
    public:
        void Dijkstra(const Coords& p_from, const Coords& p_to) {
            priority_queue<Cell*, vector<Cell*>, decltype(&hasGreaterSpl)> pq(hasGreaterSpl);
            unsigned umax = numeric_limits<unsigned>::max();

            for (auto& row : m_cells) {
                for (auto& cellBlock : row) {
                    for (auto& cell: cellBlock) {
                        cell.m_spl = umax;
                        cell.m_prev = nullptr;
                        cell.m_processed = false;
                        pq.push(&cell);
                    }
                }
            }

            Cell* current = &getCell(p_from);
            current->m_spl = 0;

            //TODO: Add special behaviour for the target cell to discover multiple paths
            while (current->m_idx != p_to) {
                if (!current->m_processed) {
                    for (const auto &edge : current->m_out) {
                        if (edge.first->m_spl > current->m_spl + edge.second) {
                            edge.first->m_spl = current->m_spl + edge.second;
                            edge.first->m_prev = current;
                            pq.push(edge.first);
                        }
                    }

                }
                current->m_processed = true;
                current = pq.top();
                pq.pop();
            }
            cout << "Shortest path length: " << getCell(p_to).m_spl << endl;
        }

    protected:
        virtual Cell& getCell(const Coords& c) = 0;

        vector<vector<vector<Cell>>> m_cells;
    };

    class NumberedGraph : public Graph {
    public:
        explicit NumberedGraph(const vector<vector<unsigned>>& data) {
            m_cells.resize(data.size());
            for (unsigned i = 0; i < data.size(); ++i) {
                m_cells[i].resize(data[i].size());
                for (unsigned j = 0; j < m_cells[i].size(); ++j) {
                    m_cells[i][j].emplace_back(Cell(make_tuple(j,i,0)));
                }
            }

            for (int row = 0; row < m_cells.size(); ++row) {
                for (int col = 0; col < m_cells[row].size(); ++col) {
                    int V = data[row][col];
                    m_cells[row][col][0].m_value = V;
                    if (V == 0) continue;

                    if (row - V >= 0) {
                        m_cells[row][col][0].m_out.emplace_back(make_pair(&m_cells[row-V][col][0],1));
                    }

                    if (row + V < m_cells.size()) {
                        m_cells[row][col][0].m_out.emplace_back(make_pair(&m_cells[row+V][col][0], 1));
                    }

                    if (col - V >= 0) {
                        m_cells[row][col][0].m_out.emplace_back(make_pair(&m_cells[row][col-V][0], 1));
                    }

                    if (col + V < m_cells[row].size()) {
                        m_cells[row][col][0].m_out.emplace_back(make_pair(&m_cells[row][col+V][0], 1));
                    }
                }
            }
        }

    protected:
        Cell& getCell(const Coords &c) override {
            return m_cells[get<1>(c)][get<0>(c)][get<2>(c)];
        }
    };



    /*class LabyrinthStructure {
    public:
        virtual Box& getBox(int x, int y, int z) = 0;
        //TODO virtual void drawStructure(svg::SVGFile& file) = 0;
    };*/

    /*class Labyrinth { //only square for now
    public:
        Labyrinth(unsigned X, unsigned Y, bool full = true) {
            if (full) {
                //_cells =
                auto x = vector<Cell>(Y, vector<Cell>(X, {nullptr, nullptr, nullptr, nullptr}));
            }
        }
    private:
        vector<vector<Cell>> _cells;
    };*/
}
#endif //IV112_TASKS_LABYRINTH_H
