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
        Cell(Coords coords) : m_out(), m_idx(move(coords)), m_prev(), m_spl(0), m_processed(false) {}

        vector<pair<Cell*, int>> m_out;
        unsigned m_value;
        bool isWall;
        const Coords m_idx;

        vector<Cell*> m_prev;//cells from which this cell was reached with m_spl length
        unsigned m_spl;//shortest path length
        bool m_processed;
    };

    bool hasGreaterSpl(Cell* lhs, Cell* rhs) {return lhs->m_spl > rhs->m_spl;}
    bool hasLesserSpl(Cell* lhs, Cell* rhs) {return hasGreaterSpl(rhs, lhs);}



    class Graph {
    private:
        void printShortestPaths(Cell* p_cell, vector<Cell*> &p_shortestPath, vector<vector<Cell*>> &output) {
            p_shortestPath.push_back(p_cell);
            if (p_cell->m_prev.empty()) {
                output.push_back(p_shortestPath);
                for (unsigned i = p_shortestPath.size()-1; i > 0; --i) {
                    const Coords& A = p_shortestPath[i]->m_idx;
                    const Coords& B = p_shortestPath[i-1]->m_idx;
                    cout << "[" << get<0>(A) << "," << get<1>(A) << "," << get<2>(A) << "] -> ["
                                << get<0>(B) << "," << get<1>(B) << "," << get<2>(B) << "]" << endl;
                }
                cout << endl;
                p_shortestPath.pop_back();
                return;
            }

            for (Cell* prev : p_cell->m_prev) {
                printShortestPaths(prev, p_shortestPath, output);
            }
            p_shortestPath.pop_back();
        }
    public:
        vector<vector<Cell*>> Dijkstra(const Coords& p_from, const Coords& p_to) {
            //priority_queue<Cell*, vector<Cell*>, decltype(&hasGreaterSpl)> pq(hasGreaterSpl);
            vector<Cell*> pq;
            unsigned umaxhalf = numeric_limits<unsigned>::max()/2;

            for (auto& row : m_cells) {
                for (auto& cellBlock : row) {
                    for (auto& cell: cellBlock) {
                        cell.m_spl = umaxhalf;
                        cell.m_prev.clear();
                        cell.m_processed = false;
                        //pq.push_(&cell);
                        pq.push_back(&cell);
                    }
                }
            }

            Cell* current = &getCell(p_from);
            current->m_spl = 0;
            make_heap(pq.begin(), pq.end(), &hasGreaterSpl);
            pop_heap(pq.begin(), pq.end(), &hasGreaterSpl);
            pq.pop_back();

            while (current->m_idx != p_to) {
                //if (!current->m_processed) {
                for (const auto &edge : current->m_out) {
                    if (edge.first->m_spl == current->m_spl + edge.second) {
                        edge.first->m_prev.push_back(current);
                    }

                    if (edge.first->m_spl > current->m_spl + edge.second) {
                        edge.first->m_spl = current->m_spl + edge.second;
                        edge.first->m_prev.clear();
                        edge.first->m_prev.push_back(current);
                        //pq.push(edge.first); //insted of decrease-key we add the pointer again but with new m_spl value
                        make_heap(pq.begin(), pq.end(), &hasGreaterSpl);
                    }
                }

                //}
                //current->m_processed = true;
                //current = pq.top(); //extract min (look at min item)
                current = pq.front();
                //pq.pop(); //remove the min item
                pop_heap(pq.begin(), pq.end(), &hasGreaterSpl);
                pq.pop_back();

                auto b1 = current->m_idx != p_to;
                auto b2 = current->m_spl == umaxhalf;
            }
            cout << "Shortest path length: " << getCell(p_to).m_spl << endl;
            vector<Cell*> SP;
            vector<vector<Cell*>> output;
            printShortestPaths(&getCell(p_to), SP, output);
            return move(output);
        }

        void drawLabyrinth(svg::SVGFile& file, vector<vector<Cell*>> shortestPaths = {}) {
            using namespace svg;
            unsigned rows = m_cells.size();
            unsigned columns = 0;
            for (const auto& row : m_cells) {
                columns = row.size() > columns ? row.size() : columns;
            }
            double cellSize = min(0.95*file.m_width/columns, 0.95*file.m_height/columns);
            double leftX = file.m_width/2 - columns*cellSize/2;
            double topY = file.m_height/2 - rows*cellSize/2;

            for (unsigned i = 0; i < m_cells.size(); ++i) {
                double rowLength = m_cells[i].size() * cellSize;
                Point A{leftX, topY + i*cellSize};
                Point B{leftX + rowLength, topY + i*cellSize};
                Point C{leftX, topY + (i+1)*cellSize};
                Point D{leftX + rowLength, topY + (i+1)*cellSize};
                file.addLine(A, B);
                file.addLine(C, D);

                for (int j = 0; j <= m_cells[i].size(); ++j) {
                    file.addLine({leftX + j*cellSize, topY + i*cellSize}, {leftX + j*cellSize, topY + (i+1)*cellSize});
                    if (j < m_cells[i].size() && m_cells[i][j][0].isWall) {
                        file.addLine({leftX + j*cellSize, topY + i*cellSize}, {leftX + (j+1)*cellSize, topY + (i+1)*cellSize});
                        file.addLine({leftX + j*cellSize, topY + (i+1)*cellSize}, {leftX + (j+1)*cellSize, topY + i*cellSize});
                    }
                }
            }

            if (!shortestPaths.empty()) {
                Point A,B;
                for (unsigned i = 0; i < shortestPaths.size(); ++i) {
                    for (unsigned j = 1; j < shortestPaths[i].size(); ++j) {
                        Coords cA = shortestPaths[i][j-1]->m_idx;
                        Coords cB = shortestPaths[i][j]->m_idx;
                        A = {leftX + (get<0>(cA) + 0.5)*cellSize, topY + (get<1>(cA) + 0.5)*cellSize};
                        B = {leftX + (get<0>(cB) + 0.5)*cellSize, topY + (get<1>(cB) + 0.5)*cellSize};
                        file.addLine(A, B, COLORS[i+1]);
                    }
                }
            }
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

    class RobotGraph : public Graph {
    public:
        explicit RobotGraph(const vector<string>& data) {
            m_cells.resize(data.size());
            for (unsigned i = 0; i < data.size(); ++i) {
                m_cells[i].resize(data[i].length());
                for (unsigned j = 0; j < m_cells[i].size(); ++j) {
                    m_cells[i][j].emplace_back(Cell(make_tuple(j,i,0)));
                    m_cells[i][j].emplace_back(Cell(make_tuple(j,i,1)));
                    m_cells[i][j].emplace_back(Cell(make_tuple(j,i,2)));
                    m_cells[i][j].emplace_back(Cell(make_tuple(j,i,3)));
                }
            }

            for (int row = 0; row < m_cells.size(); ++row) {
                for (int col = 0; col < m_cells[row].size(); ++col) {
                    for (int rotation = 0; rotation < 4; ++rotation) {
                            auto left = &m_cells[row][col][mod(rotation - 1, 4)];
                            auto right = &m_cells[row][col][mod(rotation + 1, 4)];
                            m_cells[row][col][rotation].m_out.emplace_back(make_pair(left, 1));
                            m_cells[row][col][rotation].m_out.emplace_back(make_pair(right, 1));
                    }
                    vector<Cell> &current = m_cells[row][col];
                    if (data[row][col] == ' ') {
                        m_cells[row][col][0].isWall = false;
                        m_cells[row][col][1].isWall = false;
                        m_cells[row][col][2].isWall = false;
                        m_cells[row][col][3].isWall = false;

                        if (row != 0 && (data[row-1].size() >= col+1)) { //from above
                            m_cells[row - 1][col][1].m_out.emplace_back(make_pair(&current[1], 1));
                        }

                        if (row != m_cells.size() - 1 && (data[row+1].size() >= col+1)) { //from below
                            m_cells[row + 1][col][3].m_out.emplace_back(make_pair(&current[3], 1));
                        }

                        if (col != 0) { //from left
                            m_cells[row][col - 1][0].m_out.emplace_back(make_pair(&current[0], 1));
                        }

                        if (col != m_cells[row].size() - 1) { //from right
                            m_cells[row][col + 1][2].m_out.emplace_back(make_pair(&current[2], 1));
                        }
                    } else {
                        m_cells[row][col][0].isWall = true;
                        m_cells[row][col][1].isWall = true;
                        m_cells[row][col][2].isWall = true;
                        m_cells[row][col][3].isWall = true;
                    }
                }
            }
        }

    protected:
        Cell &getCell(const Coords &c) override {
            return m_cells[get<1>(c)][get<0>(c)][get<2>(c)];
        }

    private:
        int mod (int a, int b)
        {
            int ret = a % b;
            if(ret < 0)
                ret+=b;
            return ret;
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
