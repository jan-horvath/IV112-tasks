#include <iostream>
#include "../framework/labyrinth.hpp"
#include "../framework/framework.hpp"

using namespace std;
using namespace Labyrinth;
using namespace svg;

vector<vector<unsigned>> getNumGraphFromFile(const string& filename) {
    ifstream ifs(filename, ios::in);
    vector<vector<unsigned>> output(1);

    char c;
    while (!ifs.eof()) {
        ifs.get(c);
        if (c == ' ') continue;
        if (c == '\n') {
            output.emplace_back(vector<unsigned>());
            continue;
        }
        unsigned value = c - '0';
        output.back().push_back(value);
    }
    //For some reason, the loop does not end
    // when the end of file is reached but finds
    // two more new lines, which creates two more (empty) vectors
    output.resize(output.size()-2);
    return move(output);
}

int main() {
    /*auto output = getNumGraphFromFile("ciselne-bludiste3.txt");
    NumberedGraph ng1(output);*/
    //ng1.Dijkstra(make_tuple(0,0,0), make_tuple(4,4,0));

    SVGFile file("robot_graph1.svg", 1000, 1000);
    RobotGraph rg({
        //" #  ",
        //"   "
        "     ",
        " ###   ",
        "  ##    ",
        "#  #   ",
        "##   "
    });
    auto shortestPaths = rg.Dijkstra({0,0,1}, {3,4,1});
    //auto shortestPaths = rg.Dijkstra({0,0,1}, {3,0,1)});
    rg.drawLabyrinth(file, shortestPaths);

    return 0;
}