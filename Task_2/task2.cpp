
#include <vector>
#include <cassert>

using namespace std;

vector<vector<char>> variations(const vector<char> &in, unsigned k) {
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
        copy.erase(copy.begin()+i);

        auto toAppend = variations(copy, k-1);
        for (auto& item: toAppend) {
            item.push_back(erased);
        }

        output.insert(output.end(), toAppend.begin(), toAppend.end());
    }

    return output;
}

vector<vector<char>> permutations(const vector<char> &in) {
    vector<vector<char>> output;
    if (in.size() == 1) {
        return {in};
    }

    for (size_t i = 0; i < in.size(); ++i) {
        auto copy = in;
        auto erased = copy[i];
        copy.erase(copy.begin()+i);

        auto toAppend = permutations(copy);
        for (auto& item: toAppend) {
            item.push_back(erased);
        }

        output.insert(output.end(), toAppend.begin(), toAppend.end());
    }

    return output;
}

int main() {
    /*auto perms = permutations({'A', 'B', 'C', 'D'});
    for (auto &item : perms) {
        for (char c :item) {cout << c;}
        cout << endl;
    }*/

    /*auto var = variations({'A', 'B', 'C', 'D', 'E'}, 2);
    for (auto &item : var) {
        for (char c :item) {cout << c;}
        cout << endl;
    }*/

    return 0;
}