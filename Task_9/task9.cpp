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
unsigned throwDie(unsigned diceIdx) {
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
        sum += throwDie(diceIdx);
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
    double min = 0.0;
    double max = 7.0;

    sort(CLT_Test_Output.begin(), CLT_Test_Output.end());
    double currentGroupThreshold = min + groupSize;
    unsigned currentGroupSize = 0;
    for (unsigned i = 0; i <= CLT_Test_Output.size(); ++i) {
        if ((i != CLT_Test_Output.size()) && (CLT_Test_Output[i] < currentGroupThreshold)) {
            ++currentGroupSize;
        } else {
            svg::Point A((currentGroupThreshold - groupSize/2) * file.m_width/(max - min), file.m_height * 0.9);
            svg::Point B(A.X, file.m_height * 0.9 - currentGroupSize*0.25);
            file.addLine(A, B);

            currentGroupSize = 0;
            currentGroupThreshold += groupSize;
            if (i == CLT_Test_Output.size()) break;
            --i;
        }
    }
    file.addLabels();
}

//N = 10, X = 5 (<1%)
//N = 100, X = 3 (<30%)
//N = 1000, X = 5 (<15%)
//N = 216, X = 3 (~50%)

unsigned throwDie(bool fake) {
    if (!fake) return (rand() % 6) + 1;
    return 6;
}

void fakeDieSimulation(unsigned N, unsigned K, unsigned iterations) {

    for (unsigned i = 0; i < iterations; ++i) {

    }

}

int main() {
    srand(4);
    //girlBoySimulation(10000000);
    //randomBreakdown(1); //write permutations of 6
    //randomBreakdown(2); //whenever a 2 is generated, replace it with 5 with 50% probability
    //randomBreakdown(4); //odd and even numbers alternate
    //randomBreakdown(5); // 2 is most likely followed by 5, 5 by 3, 3 by 4, 4 by 1, 1 by 6, 6 by 2
    //randomBreakdown(); 3,6,7

    /*SVGFile test1_plot("plot1.svg", 1000, 1000);
    auto test1 = CLT_Test(30000, 100, 1);
    plot_CLT_Test(test1_plot, test1, 0.03);

    SVGFile test2_plot("plot2.svg", 1000, 1000);
    auto test2 = CLT_Test(30000, 100, 2);
    plot_CLT_Test(test2_plot, test2, 0.03);

    SVGFile test3_plot("plot3.svg", 1000, 1000);
    auto test3 = CLT_Test(30000, 100, 3);
    plot_CLT_Test(test3_plot, test3, 0.03);*/



    return 0;
}

