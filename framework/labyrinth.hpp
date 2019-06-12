#ifndef IV112_TASKS_LABYRINTH_H
#define IV112_TASKS_LABYRINTH_H

#include <vector>
#include <tuple>
#include <queue>
#include <random>
#include "framework.hpp"

using namespace std;

namespace Labyrinth {
    struct Coords {
        unsigned X;
        unsigned Y;
        unsigned Z;

        friend bool operator==(const Coords &lhs, const Coords &rhs) {
            return lhs.X == rhs.X &&
                   lhs.Y == rhs.Y &&
                   lhs.Z == rhs.Z;
        }

        friend bool operator!=(const Coords &lhs, const Coords &rhs) {
            return !(rhs == lhs);
        }
    };

    struct Cell {
        Cell(Coords coords) : m_out(), m_idx(coords), m_prev(), m_spl(0), m_processed(false) {}

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
                    cout << "[" << A.X << "," << A.Y << "," << A.Z << "] -> ["
                                << B.X << "," << B.Y << "," << B.Z << "]" << endl;
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
            vector<Cell*> pq; //priority queue
            unsigned umaxhalf = numeric_limits<unsigned>::max()/2;

            for (auto& row : m_cells) {
                for (auto& cellBlock : row) {
                    for (auto& cell: cellBlock) {
                        cell.m_spl = umaxhalf;
                        cell.m_prev.clear();
                        cell.m_processed = false;
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
                for (const auto &edge : current->m_out) {
                    if (edge.first->m_spl == current->m_spl + edge.second) {
                        edge.first->m_prev.push_back(current);
                    }

                    if (edge.first->m_spl > current->m_spl + edge.second) {//decrease-key
                        edge.first->m_spl = current->m_spl + edge.second;
                        edge.first->m_prev.clear();
                        edge.first->m_prev.push_back(current);
                        make_heap(pq.begin(), pq.end(), &hasGreaterSpl);
                    }
                }

                current = pq.front();
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

        virtual void drawLabyrinth(svg::SVGFile& file, vector<vector<Cell*>> shortestPaths = {}) {
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
                        A = {leftX + (cA.X + 0.5)*cellSize, topY + (cA.Y + 0.5)*cellSize};
                        B = {leftX + (cB.X + 0.5)*cellSize, topY + (cB.Y + 0.5)*cellSize};
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
                    m_cells[i][j].emplace_back(Cell({j,i,0}));
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
            return m_cells[c.Y][c.X][c.Z];
        }
    };

    class RobotGraph : public Graph {
    public:
        explicit RobotGraph(const vector<string>& data) {
            m_cells.resize(data.size());
            for (unsigned i = 0; i < data.size(); ++i) {
                m_cells[i].resize(data[i].length());
                for (unsigned j = 0; j < m_cells[i].size(); ++j) {
                    m_cells[i][j].emplace_back(Cell({j,i,0}));
                    m_cells[i][j].emplace_back(Cell({j,i,1}));
                    m_cells[i][j].emplace_back(Cell({j,i,2}));
                    m_cells[i][j].emplace_back(Cell({j,i,3}));
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
            return m_cells[c.Y][c.X][c.Z];
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

    class TriangleGraph : Graph {
    public:
        explicit TriangleGraph(unsigned size) {
            unsigned wall = 2*size*size;

            for (unsigned row = 0; row < size; ++row) {
                m_cells.emplace_back();
                m_cells.back().resize(2*row + 1);
                for (unsigned col = 0; col < 2*row + 1; ++col) {
                    m_cells.back()[col].emplace_back(Cell({col, row, 0}));
                }
            }

            for (unsigned row = 0; row < size; ++row) {
                for (unsigned col = 0; col < 2*row + 1; ++col) {
                    if ((row != size-1) && (isEven(col))) { //add the neighbour below
                        m_cells[row][col][0].m_out.emplace_back(make_pair(&m_cells[row+1][col+1][0], wall));
                    }

                    if (!isEven(col)) { //add the neighbour above
                        m_cells[row][col][0].m_out.emplace_back(make_pair(&m_cells[row-1][col-1][0], wall));
                    }

                    if (col != 0) { //add left neighbour
                        m_cells[row][col][0].m_out.emplace_back(make_pair(&m_cells[row][col-1][0], wall));
                    }

                    if (col != m_cells[row].size()-1) {
                        m_cells[row][col][0].m_out.emplace_back(make_pair(&m_cells[row][col+1][0], wall));
                    }
                }
            }
        }

        void generateLabyrinth() {
            stack<Cell*> S;
            S.push(&m_cells[m_cells.size()/2][m_cells[m_cells.size()/2].size()/2][0]);
            S.top()->m_processed = true;

            while (!S.empty()) {
                pair<Cell*, int>* next = getNextNeighbour(S.top());

                if (next == nullptr) {
                    S.pop();
                } else {
                    next->second = 1;
                    next->first->m_processed = true;
                    unsigned control = 0;
                    for (auto& edge : next->first->m_out) {
                        if (edge.first == S.top()) {
                            edge.second = 1;
                            control++;
                        }
                    }
                    assert(control == 1);
                    S.push(next->first);
                }
            }
        }

        void drawLabyrinth(svg::SVGFile &file, vector<vector<Cell *>> shortestPaths = {}) override {
            svg::Point A{file.m_width/2, 0.1*file.m_height}; //TODO make smaller
            svg::Point B{file.m_width/2 - 0.8*file.m_width/sqrt(3), 0.9*file.m_height};
            svg::Point C{file.m_width/2 + 0.8*file.m_width/sqrt(3), 0.9*file.m_height};

            file.addLine(A,B);
            file.addLine(B,C);
            file.addLine(C,A);

            double rowHeight = 0.8*file.m_height/m_cells.size();
            double cellSize = 2*0.8*file.m_width/sqrt(3)/(m_cells.back().size()/2 + 1);

            for (unsigned row = 0; row < m_cells.size(); ++row) {
                for (unsigned col = 0; col < 2 * row + 1; ++col) {
                    for (const auto& edge : m_cells[row][col][0].m_out) {
                        if (edge.second > 1) {
                            const Coords &NC = edge.first->m_idx; //negihbour colors
                            if (row == NC.Y) { assert(col == NC.X + 1 || NC.X == col + 1); } //TODO remove later
                            if (row == NC.Y && NC.X == col + 1) { //edge on the right of current cell
                                double PXcoef = -1.0 / 2 * row + (col + 1) / 2;
                                double RXcoef = -1.0 / 2 * (row - 1) + col / 2;
                                svg::Point P{file.m_width / 2 + PXcoef * cellSize,
                                             0.1 * file.m_height + rowHeight * row};
                                svg::Point R{file.m_width / 2 + RXcoef * cellSize,
                                             0.1 * file.m_height + rowHeight * (row + 1)};
                                file.addLine(P, R);
                            }
                            if (row + 1 == NC.Y && col + 1 == NC.X) {
                                double Xcoef = -1.0/2 * (row+1) + col/2;
                                svg::Point P{file.m_width / 2 + Xcoef * cellSize, 0.1 * file.m_height + rowHeight * (row+1)};
                                svg::Point R{file.m_width / 2 + (Xcoef+1) * cellSize, 0.1 * file.m_height + rowHeight * (row+1)};
                                file.addLine(P,R);
                            }
                        }
                    }
                }
            }
        }

        Cell &getCell(const Coords &c) override {
            return m_cells[c.Y][c.X][0];
        }

    private:
        pair<Cell*, int>* getNextNeighbour(Cell* c) const {
            std::random_device rd;
            std::mt19937 g(rd());
            vector<unsigned> indices(c->m_out.size());
            iota(indices.begin(), indices.end(), 0);
            shuffle(indices.begin(), indices.end(), g);

            pair<Cell*, int>* next = nullptr;

            for (unsigned i : indices) {
                if (!c->m_out[i].first->m_processed) {
                    next = &c->m_out[i];
                    break;
                }
            }
            return next;
        }

        bool isEven(unsigned num) {return (num % 2) == 0;}
    };
}
#endif //IV112_TASKS_LABYRINTH_H
