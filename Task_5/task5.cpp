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
                if ((x == minX) || (y == minY) || (x + spacing > maxX) || (y + spacing > maxY)) {
                    points.push_back({(double)(x), (double)y});
                } else {
                    int noiseX = (rand() % 9) + 4;
                    int noiseY = (rand() % 9) + 4;
                    points.push_back({(double) (x + noiseX), (double) (y + noiseY)});
                }
                file.addCircle(points.back(), 2, true);
            }
        }
    }
    return move(points);
}

vector<Point> generatePoints_circle(SVGFile& file, unsigned radius, unsigned count, Point center, bool includeCenter) {
    vector<Point> points;
    double angle = 2*M_PI / count;
    for (unsigned i = 0; i < count; ++i) {
        points.push_back({radius * cos(angle * i + 0.01) + center.X, radius * sin(angle*i+0.01) + center.Y});
        file.addCircle(points.back(), 2, true);
    }

    if (includeCenter) {
        points.push_back(center);
        file.addCircle(points.back(), 2, true);
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
    srand(88);
    /*SVGFile fileA("task5A.svg", 1000, 1000);
    auto lineSegments = generateLineSegments(fileA, 35, 300);
    detectIntersections(fileA, lineSegments);*/

    /*SVGFile fileB1("task5B-1.svg", 1000, 1000);
    auto pointsB1 = generatePoints_lattice(fileB1, 100, 150, 850, 150, 850, 0);
    triangulate(fileB1, pointsB1, true);

    SVGFile fileB0("task5B-0.svg", 1000, 1000);
    auto pointsB0 = generatePoints_lattice(fileB0, 100, 150, 850,150, 850, 50);
    triangulate(fileB0, pointsB0, true);

    SVGFile fileB2("task5B-2.svg", 1000, 1000);
    auto pointsB2 = generatePoints_normal(fileB2, 100, 500, 100, 500, 100);
    triangulate(fileB2, pointsB2, true);

    SVGFile fileB3("task5B-3.svg", 1000, 1000);
    auto pointsB3 = generatePoints_circle(fileB3, 400, 20, {500, 500}, true);
    triangulate(fileB3, pointsB3, true);*/

    SVGFile fileC1("task5C-1.svg", 1000, 1000);
    auto pointsC1 = generatePoints_lattice(fileC1, 100, 150, 850, 150, 850, 0);
    konvexHullJarvis(fileC1, pointsC1);

    SVGFile fileC0("task5C-0.svg", 1000, 1000);
    auto pointsC0 = generatePoints_lattice(fileC0, 100, 150, 850, 150, 850, 50);
    konvexHullJarvis(fileC0, pointsC0);

    SVGFile fileC2("task5C-2.svg", 1000, 1000);
    auto pointsC2 = generatePoints_normal(fileC2, 100, 500, 100, 500, 100);
    konvexHullJarvis(fileC2, pointsC2);

    SVGFile fileC3("task5C-3.svg", 1000, 1000);
    auto pointsC3 = generatePoints_circle(fileC3, 400, 15, {500, 500}, true);
    konvexHullJarvis(fileC3, pointsC3);

    return 0;
}