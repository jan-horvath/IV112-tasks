#include <iostream>
#include "../framework/framework.hpp"
#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>
#include <cmath>

auto getDivisorCount(unsigned number) {
    unsigned currentDivCount = 0;
    for (unsigned divisor = 1; divisor < number; ++divisor) {
        if (number % divisor == 0) {
            currentDivCount++;
        }
    }
    return currentDivCount;
}

auto maxDivisors(unsigned input) {
    std::vector<unsigned> maxDivCountNums;
    unsigned maxDivCount = 0;
    unsigned currentDivCount = 0;

    for (unsigned number = 0; number < input; ++number) {
        currentDivCount = getDivisorCount(number);

        if (currentDivCount > maxDivCount) {
            maxDivCountNums.clear();
            maxDivCount = currentDivCount;
            maxDivCountNums.push_back(number);
        } else if (currentDivCount == maxDivCount) {
            maxDivCountNums.push_back(number);
        }
    }

    maxDivCountNums.push_back(maxDivCount);
    return maxDivCountNums;
}

auto isThreeSquaresSummable(unsigned number) {
    for (unsigned i = 1; i < sqrt(number); ++i) {
        for (unsigned j = 1; j < sqrt(number); ++j) {
            for (unsigned k = 1; k < sqrt(number); ++k) {
                if (number < i*i + j*j + k*k) break;
                if (number == i*i + j*j + k*k) return true;
            }
        }
    }

    return false;
}

auto threeSquaresSum(unsigned input) {
    std::vector<unsigned> output;

    for (unsigned number = 1; number < input; ++number) {
        if (!isThreeSquaresSummable(number))
            output.push_back(number);
    }
    return output;
}

/*auto getCollatzSteps(unsigned i) {
    //TODO
}

auto maxStepsCollatzSequence(unsigned input) {
    unsigned maxSteps = 0;
    unsigned currentSteps = 0;
    std::vector<unsigned> maxStepsNumbers;

    for (unsigned i = 0; i < input; ++i) {
        currentSteps = getCollatzSteps(i);

        if (currentSteps > maxSteps) {
            maxSteps = currentSteps;
            maxStepsNumbers.clear();
            maxStepsNumbers.push_back(i);
        } else if (currentSteps == maxSteps) {
            maxStepsNumbers.push_back(i);
        }
    }

    maxStepsNumbers.push_back(maxSteps);
    return maxStepsNumbers;
}*/

int main() {
    /*auto maxDivisorsVector = maxDivisors(10000);
    for (auto number : maxDivisorsVector) {
        std::cout << number << std::endl;
    }

    auto TSS = threeSquaresSum(1000);
    for (auto number : TSS) {
        std::cout << number << ", ";
    }
    std::cout << std::endl << TSS.size();*/

    return 0;
}