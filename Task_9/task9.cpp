#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "../framework/framework.hpp"

using namespace std;
using namespace svg;
const unsigned A = 0;
const unsigned B = 1;

void girlBoySimulation(unsigned families) {
    unsigned girls = 0;
    unsigned boys = 0;

    for (unsigned i = 0; i < families; ++i) {
        while(rand() % 2 == 1) ++girls;
        ++boys;
    }

    cout << "For " << families << " families (boys), the number of girls is "
    << girls << " ("  << 100*(double)girls/(boys+girls) << "%)" << endl;
}

void randomBreakdown(unsigned fileNum) {
    ifstream inputFile("../random" + to_string(fileNum) + ".txt");
    unsigned num = 0;
    unsigned lastnum = 1;
    vector<unsigned> histogram(6,0);
    vector<vector<unsigned>> dHistogram(6,{0,0,0,0,0,0});
    while (inputFile >> num) {
        ++histogram[num-1];
        ++dHistogram[lastnum-1][num-1];
        lastnum = num;
    }
    --histogram[0];

    cout << "Histogram of individual values: " << endl;
    for (auto n : histogram) cout << n << " ";
    cout << endl << endl;

    cout << "Histogram of 2 consecutive numbers: "  << endl << "    ";
    for (int k = 1; k <= 6; ++k) {cout << setw(5) << k;}
    cout << "  (second)" << endl;
    unsigned i = 1;
    for (const auto& vec : dHistogram) {
        cout << "   " << i++;
        for (auto n : vec) {
            cout << setw(5) << n;
        }
        cout << endl;
    }
    cout << "(first)" << endl;
}

//TODO Make it nicer
unsigned throwDice(unsigned diceIdx) {
    unsigned n = (rand() % 21) + 1;
    if (diceIdx > 1) {diceIdx = rand() % 2;}
    if (diceIdx == 0) {
        if (n == 1) return 1;
        if (n <= 3) return 2;
        if (n <= 6) return 3;
        if (n <= 10) return 4;
        if (n <= 15) return 5;
        if (n <= 21) return 6;
    } else {
        if (n <= 6) return 1;
        if (n <= 11) return 2;
        if (n <= 15) return 3;
        if (n <= 18) return 4;
        if (n <= 20) return 5;
        if (n <= 21) return 6;
    }
}

double nThrowAverage(unsigned n, unsigned diceIdx) {
    double sum = 0.0;
    for (unsigned i = 0; i < n; ++i) {
        sum += throwDice(diceIdx);
    }
    return sum/n;
};

vector<double> CLT_Test(unsigned k, unsigned n, unsigned testNumber) {
    vector<double> output;
    output.reserve(k);
    for (unsigned i = 0; i < k; ++i) {
        double average = 0.0;
        if (testNumber == 1) {
            average = nThrowAverage(n, 0);
        }
        if (testNumber == 2) {
            average = nThrowAverage(n, 2);
        }
        if (testNumber == 3) {
            average = nThrowAverage(n, rand() % 2);
        }
        output.push_back(average);
    }
    return move(output);
}

void plot_CLT_Test(svg::SVGFile &file, vector<double> &CLT_Test_Output, double groupSize) {
    double min = 1.0;
    double max = 6.0;

    sort(CLT_Test_Output.begin(), CLT_Test_Output.end());
    double currentGroupThreshold = min + groupSize;
    unsigned currentGroupSize = 0;
    for (unsigned i = 0; i < CLT_Test_Output.size(); ++i) {
        if (CLT_Test_Output[i] < currentGroupThreshold) {
            ++currentGroupSize;
        } else {
            svg::Point A((currentGroupThreshold - groupSize/2 - 1) * file.m_width/(max - min), file.m_height * 0.9);
            svg::Point B((currentGroupThreshold - groupSize/2 - 1) * file.m_width/(max - min), file.m_height * 0.9 - currentGroupSize);
            file.addLine(A, B);

            currentGroupSize = 0;
            currentGroupThreshold += groupSize;
            --i;
        }
    }
}

template <unsigned DICE>
unsigned throwPossiblyFakeDice() {
    //TODO finish
}

// TODO plot in svg: plot(vector<double>) {sort, group together and draw line upward}

int main() {
    srand(35);
    //girlBoySimulation(10000000);
    //randomBreakdown(1); //write permutations of 6
    //randomBreakdown(2); //whenever a 2 is generated, replace it with 5 with 50% probability
    //randomBreakdown(4); //odd and even numbers alternate
    //randomBreakdown(5); // 2 is most likely followed by 5, 5 by 3, 3 by 4, 4 by 1, 1 by 6, 6 by 2
    //randomBreakdown(); 3,6,7


    auto test1 = CLT_Test(1, 10, 1);


    return 0;
}

