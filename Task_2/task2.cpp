
#include <vector>
#include <cassert>
#include <iostream>
#include "../framework/framework.hpp"
#include <chrono>
#include <limits.h>

using namespace std;
using namespace svg;

const long double REAL_PI = 3.1415926535897932;

vector<vector<char>> variations(const vector<char> &in, unsigned k, bool repetition = false) {
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
    double blockSize = 0.9*2/rows;
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

void leibniz(long timeLimit) {
    auto start = std::chrono::high_resolution_clock::now();
    auto workingTime = (chrono::high_resolution_clock::now() - start) / chrono::milliseconds(1);

    long double PI = 1;

    for (size_t i = 0; timeLimit > workingTime; ++i) {
        PI = PI - 1.0/(4*i+3) + 1.0/(4*i+5);
        workingTime = (chrono::high_resolution_clock::now() - start) / chrono::milliseconds(1);
    }
    cout << "Computation time using leibniz sequence = " << workingTime << " milliseconds." << endl;
    cout << "Difference between computed pi and the pi constant = " << 4*PI - REAL_PI << endl << endl;
}

void archimedes_polygon(unsigned iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    auto workingTime = (chrono::high_resolution_clock::now() - start) / chrono::nanoseconds(1);

    long double PI = 0;
    long double polygonSideLength = 1.0;
    unsigned long long polygon_N = 6;

    for (unsigned i = 0; i < iterations; ++i) {
        long double B = 1.0 - sqrt(1.0 - polygonSideLength/2.0 * polygonSideLength/2.0);
        polygonSideLength = sqrt(B * B + polygonSideLength/2 * polygonSideLength/2);
        polygon_N = 2 * polygon_N;
        PI = polygon_N * polygonSideLength;

        workingTime = (chrono::high_resolution_clock::now() - start) / chrono::nanoseconds(1);
    }
    cout << "Computation time using archimedes polygon approximation = " << workingTime << " nanoseconds." << endl;
    cout << "Difference between computed pi and the pi constant = " << PI/2 - REAL_PI << endl << endl;
}

void monte_carlo(double timeLimit) {
    auto start = std::chrono::high_resolution_clock::now();
    auto workingTime = (chrono::high_resolution_clock::now() - start) / chrono::milliseconds(1);
    srand(500);

    size_t in = 0;
    size_t total = 0;

    while (timeLimit > workingTime) {
        double X = 2*((double)rand() / RAND_MAX) - 1;
        double Y = 2*((double)rand() / RAND_MAX) - 1;
        if (sqrt(X*X + Y*Y) <= 1.0) {
            ++in;
        }
        ++total;

        workingTime = (chrono::high_resolution_clock::now() - start) / chrono::milliseconds(1);
    }

    cout << "Computation time using monte carlo randomized algorithm = " << workingTime << " milliseconds." << endl;
    cout << "Difference between computed pi and the pi constant = " << 4 * (double)in / total - REAL_PI << endl << endl;
}

unsigned modulo(unsigned base, unsigned divisor) {
    while (base >= divisor) {
        base -= divisor;
    }
    return base;
}

    /**
     * Complexity is O(log(exp) * divisor)
     * @param base
     * @param exp
     * @param divisor
     * @return
     */

unsigned modulo(unsigned base, unsigned long long exp, unsigned divisor) {
    if (exp == 1) {
        return modulo(base, divisor);
    }

    unsigned newBase = modulo(base*base, divisor);
    if ((exp & 1) == 1) {
        unsigned tmpModulo = base * modulo(newBase, (exp-1)/2, divisor);
        return modulo(tmpModulo, divisor);
    } else {
        return modulo(newBase, exp/2, divisor);
    }
}

unsigned modulo_trivial(unsigned base, unsigned long long exp, unsigned divisor) {
    unsigned result = 1;
    for (size_t i = 0; i < exp; ++i) {
        result *= base;
        result = modulo(result, divisor);
    }
    return result;
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


    //SVGFile file("task2B.svg", 500, 500);
    //drawPascalTriangle(file, 250, 5);


    /*cout.precision(25);
    leibniz(500);
    archimedes_polygon(25);
    monte_carlo(500);*/


    auto start = std::chrono::high_resolution_clock::now();
    cout << modulo(7, 197648225, 50) << endl;
    cout << "microseconds: " << (chrono::high_resolution_clock::now() - start) / chrono::microseconds(1) << endl;

    start = std::chrono::high_resolution_clock::now();
    cout << modulo_trivial(7, 197648225, 50) << endl;
    cout << "microseconds: " << (chrono::high_resolution_clock::now() - start) / chrono::microseconds(1) << endl;

    start = std::chrono::high_resolution_clock::now();
    cout << modulo(7, 30000, 50) << endl;
    cout << "microseconds: " << (chrono::high_resolution_clock::now() - start) / chrono::microseconds(1) << endl;

    start = std::chrono::high_resolution_clock::now();
    cout << modulo_trivial(7, 30000, 50) << endl;
    cout << "microseconds: " << (chrono::high_resolution_clock::now() - start) / chrono::microseconds(1) << endl;

    return 0;
}