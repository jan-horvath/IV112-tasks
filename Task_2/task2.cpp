
#include <vector>
#include <cassert>
#include <iostream>

using namespace std;

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
    if (repetition && input.empty()) {return {};}
    if (k == 0) {return {{}};}

    vector<vector<char>> merged;
    unsigned maxIterations = repetition ? k : 2;

    for (size_t i = 0; i < maxIterations; ++i) {
        auto include_i_times = combinations({input.begin()+1, input.end()}, k-i, repetition);

        for (auto& combination : include_i_times) {
            for (size_t j = 0; j < i; ++j) {
                combination.push_back(input[0]);
            }
        }
        merged.insert(merged.end(), include_i_times.begin(), include_i_times.end());
    }
    /*auto includeFirst = combinations({input.begin()+1, input.end()}, k-1, repetition);
    auto excludeFirst = combinations({input.begin()+1, input.end()}, k, repetition);

    merged = includeFirst;
    merged.insert(merged.end(), excludeFirst.begin(), excludeFirst.end());*/

    return merged;
}

int main() {
    /*auto perms = permutations({'A', 'B', 'C', 'D'});
    for (auto &item : perms) {
        for (char c :item) {cout << c;}
        cout << endl;
    }*/

    auto var = combinations({'A', 'B', 'C', 'D'}, 2, true);
    for (auto &item : var) {
        for (char c :item) {cout << c;}
        cout << endl;
    }

    return 0;
}