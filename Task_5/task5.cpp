#include <iostream>
#include "../framework/framework.hpp"
#include <random>
#include <algorithm>

using namespace std;
using namespace svg;

Point generatePoint(unsigned minX, unsigned maxX, unsigned minY, unsigned maxY) {
    unsigned detail = 100;
    unsigned X = maxX - minX;
    unsigned Y = maxY - minY;
    return {minX + static_cast<double>(rand() % (X * detail))/detail,
            minY + static_cast<double>(rand() % (Y * detail))/detail};
}

vector<LineSegment> generateLineSegments(SVGFile& file, unsigned lineCount, double length) {
    vector<LineSegment> LS;
    for (unsigned i = 0; i < lineCount; ++i) {
        Point A = generatePoint(0, file.m_width, 0, file.m_height);
        Point B = generatePoint(0, file.m_width, 0, file.m_height);;
        LS.push_back({A, A + (B-A).norm()*length});
        file.addLine(LS.back().P1, LS.back().P2);
    }
    return std::move(LS);
}

void detectIntersections(SVGFile& file, vector<LineSegment>& lineSegments) {
    for (size_t i = 0; i < lineSegments.size(); ++i) {
        for (size_t j = i+1; j < lineSegments.size(); ++j) {
            Point p = lineSegments[i].intersect(lineSegments[j]);
            if (p.X != LineSegment::NO_INTERSECTION) {
                file.addCircle(p, 3, true);
            }
        }
    }
}

vector<Point> generatePoints_uniform(SVGFile& file, unsigned pointCount,
                             unsigned minX, unsigned maxX, unsigned minY, unsigned maxY) {
    vector<Point> points;
    for (unsigned i = 0; i < pointCount; ++i) {
        Point p = generatePoint(minX, maxX, minY, maxY);
        points.push_back(p);
        file.addCircle(p, 2, true);
    }
    return std::move(points);
}

vector<Point> generatePoints_normal(SVGFile& file, unsigned pointCount,
        double muX, double sigmaX, double muY, double sigmaY) {
    std::mt19937 gen{100};
    std::normal_distribution<> ND_X{muX, sigmaX};
    std::normal_distribution<> ND_Y{muY, sigmaY};
    vector<Point> points;
    for (unsigned i = 0; i < pointCount; ++i) {
        points.push_back({ND_X(gen), ND_Y(gen)});
        file.addCircle(points.back(), 3, true);
    }
    return move(points);
}

vector<Point> generatePoints_lattice(SVGFile& file, unsigned spacing, unsigned minX, unsigned maxX, unsigned minY, unsigned maxY,
        unsigned percentageToDump) {
    vector<Point> points;
    for (unsigned x = minX; x <= maxX; x += spacing) {
        for (unsigned y = minY; y <= maxY; y += spacing) {
            if ((rand() % 100) + 1 > percentageToDump) {
                int noiseX = (rand() % 9) + 4;
                int noiseY = (rand() % 9) + 4;
                points.push_back({(double)(x + noiseX), (double)(y+ noiseY)});
                file.addCircle(points.back(), 2, true);
            }
        }
    }
    return move(points);
}

vector<LineSegment> getLineSegmentsFromPoints(const vector<Point> &points) {
    vector<LineSegment> segments;
    for (unsigned i = 0; i < points.size(); ++i) {
        for (unsigned j = i+1; j < points.size(); ++j) {
            segments.push_back({points[i], points[j]});
        }
    }
    return std::move(segments);
}

bool segmentIntersectsOthers(const LineSegment& segment, const vector<LineSegment>& segments) {
    for (const auto& s : segments) {
        if (segment.intersect(s).X != LineSegment::NO_INTERSECTION) {
            return true;
        }
    }
    return false;
}

bool isShorterThan(const LineSegment& s1, const LineSegment& s2) {
    return s1.getLength() < s2.getLength();
}

void triangulate(SVGFile& file, const vector<Point> &points, bool shouldBeSorted) {
    vector<LineSegment> segments = getLineSegmentsFromPoints(points);
    vector<LineSegment> drawn;

    if (shouldBeSorted) {
        std::sort(segments.begin(), segments.end(), isShorterThan);
    }

    for (const auto& segment : segments) {
        if (!segmentIntersectsOthers(segment, drawn)) {
            file.addLine(segment.P1, segment.P2);
            drawn.push_back(segment);
        }
    }
}

void konvexHullJarvis(SVGFile& file, const vector<Point> &points) {
    Point leftmost = *std::max_element(points.begin(), points.end(),[](const Point& a, const Point& b) {
        return a.X > b.X;
    });
    Vector A{0, 1};
    Vector B{0, 1};
    Point latest = leftmost;
    bool loopStart = false;
    while (latest != leftmost || !loopStart) {
        loopStart = true;
        double greatestAngle = A.getAngle(B);
        Point pointWithGreatestAngle = latest;
        for (const Point& p: points) {
            if (A.getAngle(p - latest) > greatestAngle) {
                pointWithGreatestAngle = p;
                B = pointWithGreatestAngle - latest;
                greatestAngle = A.getAngle(B);
            }
        }
        file.addLine(latest, pointWithGreatestAngle);
        latest = pointWithGreatestAngle;
        A = {-B.X, -B.Y};
        B = A;
    }
}

int main() {
    srand(5);
    SVGFile fileA("task5A.svg", 1000, 1000);
    auto lineSegments = generateLineSegments(fileA, 35, 300);
    detectIntersections(fileA, lineSegments);

    SVGFile fileB("task5B.svg", 1000, 1000);
    auto points = generatePoints_lattice(fileB, 100, 300, 700, 300, 700, 0);
    triangulate(fileB, points, true);

    SVGFile fileC("task5C.svg", 1000, 1000);
    auto points2 = generatePoints_normal(fileC, 20, 500, 300, 200, 50);//generatePoints_uniform(fileC, 50, 0, 1000, 0, 1000);
    konvexHullJarvis(fileC, points2);



    return 0;
}