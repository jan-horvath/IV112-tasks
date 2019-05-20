#include <iostream>
#include <random>
#include "../framework/framework.hpp"

using namespace std;
using namespace svg;

double generateNumber(double min, double max, unsigned fineness) {
    return ((double)(rand() % fineness) / fineness) * (max - min) + min;
}

void drawLine(SVGFile& file, pair<double, double> line, const string &color = COLORS[0]) {
    double Y1 = -2 * line.first + line.second;
    double Y2 = +2 * line.first + line.second;
    file.addLine({-2, -Y1}, {2, -Y2}, color, true);
}

vector<Point> generateClusters(SVGFile &file, unsigned clusterCount, unsigned clusterSizeMin, unsigned clusterSizeMax) {
    vector<Point> points;
    for (unsigned i = 0; i < clusterCount; ++i) {
        double muX = generateNumber(-0.9, 0.9, 32);
        double muY = generateNumber(-0.9, 0.9, 32);
        double sigmaX = generateNumber(0.01, 0.1, 10);
        double sigmaY = generateNumber(0.01, 0.1, 10);

        std::mt19937 gen{500};
        std::normal_distribution<> ND_X{muX, sigmaX};
        std::normal_distribution<> ND_Y{muY, sigmaY};

        unsigned pointCount = generateNumber(clusterSizeMin, clusterSizeMax, clusterSizeMax-clusterSizeMin);

        for (unsigned j = 0; j < pointCount; ++j) {
            points.emplace_back(Point(ND_X(gen), ND_Y(gen)));
        }
    }
    for (const auto& point : points) {
        file.addCircle(point, 2, true, COLORS[0], true);
    }
    return move(points);
}

vector<Point>generateLinePoints(SVGFile &file, double a, double b, double sigmaY, unsigned pointCount) {
    vector<Point> points;
    std::mt19937 gen{2222};
    std::normal_distribution<> ND_Y{0, sigmaY};

    for  (unsigned i = 0; i < pointCount; ++i) {
        double X = i * 2.0/pointCount - 1;
        double Y = a * X + b + ND_Y(gen); //SVG has X axis upside down
        points.emplace_back(Point(X,Y));
    }
    for (const auto& point : points) {
        file.addCircle({point.X, -point.Y}, 2, true, COLORS[0], true);
    }
    return move(points);
}

pair<double, double> analyticSolution(SVGFile &file, const vector<Point> &points) {
    double n = points.size();
    double XY = 0.0;
    double X = 0.0;
    double Y = 0.0;
    double X2 = 0.0;

    for (const Point& point : points) {
        XY += point.X * point.Y;
        X += point.X;
        Y += point.Y;
        X2 += point.X * point.X;
    }

    double a = (n*XY-X*Y)/(n*X2-X*X);

    drawLine(file, make_pair(a, Y/n - a*X/n));
    return make_pair(a, Y/n - a*X/n);;
}

pair<double, double> gradientDescend(SVGFile &file, const vector<Point>& points, double learningRate, unsigned iterations) {
    double a = generateNumber(-10, 10, 200);
    double b = generateNumber(-1, 1, 200);
    double da = 0.0;
    double db = 0.0;
    for (unsigned i = 0; i < iterations; ++i) {
        for (const Point& p : points) { //compute gradients
            da -= p.X*(p.Y - a*p.X - b);
            db -= p.Y - a*p.X - b;
        }

        a -= da*learningRate;
        b -= db*learningRate;
        drawLine(file, make_pair(a, b));
        da = 0.0;
        db = 0.0;
    }
    auto pair = make_pair(a, b);
    drawLine(file, make_pair(a, b), COLORS[1]);
    return pair;
}

void kmeans(SVGFile& file, vector<Point> points, unsigned count) {
    unsigned changing = count;
    vector<vector<Point>> clusters(count, vector<Point>());
    while (changing != 0) {
        changing = count;
        for (unsigned i = 0; i < count; ++i) {
            
        }
    }
}

int main() {
    srand(527);

    SVGFile fileA("lin_reg.svg", 1000, 1000);
    vector<Point> pointsA = generateLinePoints(fileA, 0.75, 0.4, 0.05, 100);
    auto pairA = gradientDescend(fileA, pointsA, 0.005, 100);//analyticSolution(fileA, pointsA);//
    //drawLine(fileA, pairA, COLORS[3]);
    cout << "y = " << pairA.first << "x + " << pairA.second << endl;
    return 0;
}