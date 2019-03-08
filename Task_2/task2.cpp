
#include <vector>
#include <cassert>
#include <iostream>
#include "../framework/framework.hpp"
#include <chrono>

using namespace std;
using namespace svg;

vector<vector<char>> variations(const vector<char> &in, unsigned k, bool repetition = false) {
    assert(k <= in.size());

    vector<vector<char>> output;
    if (k == 1) {
        for (char c : in) {
            output.emplace_back(vector<char>({c}));
        }
        return output;
    }

    for (size_t i = 0; i < in.size(); ++i) {
        auto copy = in;
        auto erased = copy[i];

        if (!repetition) {copy.erase(copy.begin()+i);}

        auto toAppend = variations(copy, k-1);
        for (auto& item: toAppend) {
            item.push_back(erased);
        }

        output.insert(output.end(), toAppend.begin(), toAppend.end());
    }

    return output;
}

vector<vector<char>> permutations(const vector<char> &in) {
    return variations(in, in.size());
}

vector<vector<char>> combinations(const vector<char> &input, unsigned k, bool repetition = false) {
    if (!repetition && input.size() == k) {return {input};}
    if (k == 0) {return {{}};}

    if (repetition && input.size() == 1) {
        return vector<vector<char>>({vector<char>(k, input.front())});
    }

    vector<vector<char>> merged;
    unsigned maxIterations = repetition ? k : 1;

    for (size_t i = 0; i <= maxIterations; ++i) {
        auto include_i_times = combinations({input.begin()+1, input.end()}, k-i, repetition);

        for (auto& combination : include_i_times) {
            for (size_t j = 0; j < i; ++j) {
                combination.push_back(input[0]);
            }
        }
        merged.insert(merged.end(), include_i_times.begin(), include_i_times.end());
    }
    return merged;
}

void drawPascalTriangle(SVGFile& file, unsigned rows, unsigned divisor) {
    vector<unsigned> row{1};
    double blockSize = 0.007;
    double centerY = -blockSize * rows / 2.0;
    double centerX = 0.0;
    unsigned colorID = 0;

    for (size_t i = 0; i < rows; ++i) {
        for (int j = row.size() - 1; j >= 0; --j) {
            if (j != row.size()-1 && j != 0) {
                row[j] = (row[j] + row[j-1]) % divisor;
            }
            colorID = row[j] % COLORS.size();
            file.addRect({centerX, centerY}, blockSize, blockSize, COLORS[colorID], true);
            centerX += blockSize;
        }
        row.push_back(1);
        centerX = -((row.size() - 1) / 2.0) * blockSize;
        centerY += blockSize;
    }
}

long double leibniz(long timeLimit) {
    auto start = std::chrono::high_resolution_clock::now();
    auto workingTime = (chrono::high_resolution_clock::now() - start) / chrono::milliseconds(1);

    long double PI = 1;

    for (size_t i = 0; timeLimit > workingTime; ++i) {
        PI = PI - 1.0/(4*i+3) + 1.0/(4*i+5);
        workingTime = (chrono::high_resolution_clock::now() - start) / chrono::milliseconds(1);
    }
    return PI * 4;
}

long double archimedes_polygon(double timeLimit) {
    auto start = std::chrono::high_resolution_clock::now();
    auto workingTime = (chrono::high_resolution_clock::now() - start) / chrono::nanoseconds(1);

    long double PI = 0;
    long double polygonSideLength = 1.0;
    unsigned long long polygon_N = 6;
    cout << polygonSideLength << endl;

    for (unsigned i = 0; i < 30; ++i) {
        long double B = 1.0 - sqrt(1.0 - polygonSideLength/2.0 * polygonSideLength/2.0);
        polygonSideLength = sqrt(B * B + polygonSideLength/2 * polygonSideLength/2);
        polygon_N = 2 * polygon_N;
        PI = polygon_N * polygonSideLength;

        workingTime = (chrono::high_resolution_clock::now() - start) / chrono::nanoseconds(1);
    }
    cout << "Working time = " << workingTime << endl;
    return PI/2;
}

long double monte_carlo(double timeLimit) {

}



int main() {
    /*auto perms = permutations({'A', 'B', 'C', 'D'});
    for (auto &item : perms) {
        for (char c :item) {cout << c;}
        cout << endl;
    }*/

    /*auto var = combinations({'A', 'B', 'C', 'D'}, 4, true);

    for (auto &item : var) {
        for (char c :item) {cout << c;}
        cout << endl;
    }*/

    /*
    SVGFile file("task2B.svg", 1400, 1400);
    drawPascalTriangle(file, 280, 2);
    */

    cout.precision(25);
    const long double REAL_PI = 3.1415926535897932;
    cout << REAL_PI << endl;

    long double pi_liebniz = leibniz(500);
    cout << pi_liebniz - REAL_PI << endl;

    long double pi_archimedes = archimedes(0.0000000000000001);
    cout << pi_archimedes - REAL_PI << endl;

    return 0;
}