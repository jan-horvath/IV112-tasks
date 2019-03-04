#include <iostream>
#include <fstream>
#include <cmath>

namespace svg {

    const std::string FILE_NAME = "output_image.svg";
    const float IMAGE_HEIGHT = 1080;
    const float IMAGE_WIDTH = 1920;

    struct Vector {
        double X;
        double Y;

        Vector operator*(double n) {
            return {this->X * n, this->Y * n};
        }
    };

    struct Point {
        double X;
        double Y;

        Vector operator-(const Point& p) const {
            return {this->X - p.X, this->Y - p.Y};
        }

        Point operator+(const Vector& v) const {
            return {this->X + v.X, this->Y + v.Y};
        }
    };

    class SVGFile {
    public:
        SVGFile() : SVGFile(FILE_NAME) {}

        SVGFile(const std::string &file_name) : SVGFile(file_name, IMAGE_HEIGHT, IMAGE_WIDTH) {}

        SVGFile(const std::string &file_name, float height, float width) :
        m_file(file_name), m_height(height), m_width(width) {
            m_file << "<html>\n<body>\n\n"
                   << "<svg height=\"" << height << "\" width=\"" << width << "\">\n"
                   << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";
        }

        void addLine(Point a, Point b, bool upscale = false) {
            if (upscale) {
                a.X = m_width / 2 + a.X * m_width / 2;
                b.X = m_width / 2 + b.X * m_width / 2;
                a.Y = m_height / 2 + a.Y * m_height / 2;
                b.Y = m_height / 2 + b.Y * m_height / 2;

            }
            m_file << "   <line x1=\"" << a.X << "\" y1=\"" << a.Y
                   << "\" x2=\"" << b.X << "\" y2=\"" << b.Y
                   << "\" style=\"stroke:rgb(255,0,0);stroke-width:1\" />" << std::endl;
        }

        void addCircle(Point c, double rad, bool fill, bool upscale = false) {
            if (upscale) {
                c.X = m_width / 2 + c.X * m_width / 2;
                c.Y = m_height / 2 + c.Y * m_height / 2;
            }

            m_file << "   <circle cx=\"" << c.X << "\" cy=\"" << c.Y
                   << "\" r=\"" << rad
                   << "\" stroke=\"red\" stroke-width=\"1\" ";
            if (fill) {
                m_file << "fill=\"red\"";
            }
            m_file << "/>" << std::endl;
        }

        ~SVGFile() {
            m_file << "</svg>\n\n</body>\n</html>";
        }

        const float m_height;
        const float m_width;

    private:
        std::ofstream m_file;
    };

    class Turtle {
    public:
        Turtle(const std::string &filename) :
                m_file(filename),
                m_pos{m_file.m_width/2, m_file.m_height/2} {}

        Turtle(const std::string &filename, float height, float width) :
                m_file(filename, height, width),
                m_pos{width/2, height/2} {}

        void forward(float len) {
            double radDegree = toRad(m_degree);
            Point newPos = m_pos + Vector{cos(radDegree), -sin(radDegree)} * len;
            if (m_drawing) m_file.addLine(m_pos, newPos);
            m_pos = newPos;
        }

        void back(float len) {
            forward(-len);
        }

        void left(float deg) {
            m_degree += deg;
        }

        void right(double deg) {
            left(-deg);
        }

        void drawing(bool draw) {
            m_drawing = draw;
        }

    private:
        double toRad(double degrees) {return M_PI/180 * degrees;}

        SVGFile m_file;
        Point m_pos;
        double m_degree = 0.0;
        bool m_drawing = true;
    };
};

//TODO work with doubles, not floats
//TODO possibly add lines at the end