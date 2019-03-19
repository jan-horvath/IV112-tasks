#include <iostream>
#include "../framework/framework.hpp"
#include <random>

using namespace std;
using namespace svg;

vector<LineSegment> generateLineSegments(SVGFile& file, unsigned lineCount, double length) {
    vector<LineSegment> LS;
    unsigned H = file.m_height;
    unsigned W = file.m_width;
    for (unsigned i = 0; i < lineCount; ++i) {
        //TODO remove copy paste
        Point A{(double)(rand() % (W*100))/100, (double) (rand() % (H*100))/100};
        Point B{(double)(rand() % (W*100))/100, (double) (rand() % (H*100))/100};
        LS.push_back({A, A + (B-A).norm()*length});
        file.addLine(LS.back().P1, LS.back().P2);
    }
    return std::move(LS);
}

Point detectIntersection(LineSegment a, LineSegment b) {
    double X;
    /*
     * u=(cy.bx-ay.bx-cx.by+ax.by)/(dx.by-dy.bx)
        u=(bx(cy-ay) +by(ax-cx))/(dx.by-dy.bx) //tidied up slightly
     */
}

void detectIntersections(SVGFile& file, vector<LineSegment>& lineSegments) {
    for (size_t i = 0; i < lineSegments.size(); ++i) {
        for (size_t j = 0; j < lineSegments.size(); ++j) {

        }
    }
}

int main() {
    SVGFile file("task5A.svg", 1000, 1000);
    auto LineSegments = generateLineSegments(file, 50, 300);

    return 0;
}