#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;
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
    return;
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
template <unsigned DICE>
unsigned throwDice() {
    unsigned n = (rand() % 21) + 1;
    if (DICE > 1) {DICE = rand() % 2;}
    if (DICE == 0) {
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

template <unsigned DICE>
double nThrowAverage(unsigned n) {
    double sum = 0.0;
    for (unsigned i = 0; i < n; ++i) {
        sum += throwDice<DICE>();
    }
    return sum/n;
};

template <unsigned DICE>
unsigned throwPossiblyFakeDice() {
    //TODO finish
}

// TODO create n-k thrower which outputs vector<double> for the plot function
// TODO plot in svg: plot(vector<double>) {sort, group together and draw line upward}

int main() {
    srand(35);
    //girlBoySimulation(10000000);
    //randomBreakdown(1); //write permutations of 6
    //randomBreakdown(2); //whenever a 2 is generated, replace it with 5 with 50% probability
    //randomBreakdown(4); //odd and even numbers alternate
    //randomBreakdown(5); // 2 is most likely followed by 5, 5 by 3, 3 by 4, 4 by 1, 1 by 6, 6 by 2
    //randomBreakdown(); 3,6,7



    return 0;
}

