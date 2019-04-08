#include <iostream>
#include "../framework/framework.hpp"

using namespace std;
using namespace svg;

int main() {
    Matrix m1({2,2,3,4,1,1,0,1}, 2, 4);
    Matrix m2({0,2,4,6}, 4, 1);

    auto m = m1 * m2;
    cout << endl;
    return 0;
}
