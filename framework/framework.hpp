#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

namespace svg {

    const std::string FILE_NAME = "output_image.svg";
    const double IMAGE_HEIGHT = 1080;
    const double IMAGE_WIDTH = 1920;

    const std::vector<std::string> COLORS {
        "black", "red", "yellow", "green", "cyan", "blue", "pink",
        "tomato", "greenyelow", "turquoise", "dodgerblue", "purple", "mediumvioletred",
        "darkorange", "lightgreen", "aquamarine", "royalblue", "mediumpurple", "hotpink"
    };

    struct Vector {
        double X;
        double Y;

        Vector operator*(double n) {
            return {this->X * n, this->Y * n};
        }

        Vector norm() {
            double length = sqrt(X*X + Y*Y);
            return {X/length, Y/length};
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

    struct LineSegment {
        Point P1;
        Point P2;

        Vector getVec() {
            return P2 - P1;
        }
    };

    class SVGFile {
    public:
        SVGFile() : SVGFile(FILE_NAME) {}

        SVGFile(const std::string &file_name) : SVGFile(file_name, IMAGE_HEIGHT, IMAGE_WIDTH) {}

        SVGFile(const std::string &file_name, double height, double width) :
        m_file(file_name), m_height(height), m_width(width) {
            m_file << "<html>\n<body>\n\n"
                   << "<svg height=\"" << height << "\" width=\"" << width << "\">\n"
                   << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";
        }

        /**
         *
         * @param a - Start point
         * @param b - End point
         * @param col - Color
         * @param upscale - True means, that point coordinates are in interval <-1, 1> and need to be upscaled for SVG
         */
        void addLine(Point a, Point b, const std::string& col = COLORS[0], bool upscale = false) {
            if (upscale) {
                a.X = m_width / 2 + a.X * m_width / 2;
                b.X = m_width / 2 + b.X * m_width / 2;
                a.Y = m_height / 2 + a.Y * m_height / 2;
                b.Y = m_height / 2 + b.Y * m_height / 2;

            }
            m_file << "   <line x1=\"" << a.X << "\" y1=\"" << a.Y
                   << "\" x2=\"" << b.X << "\" y2=\"" << b.Y
                   << "\" stroke=\"" << col << "\" />" << std::endl;
        }

        /**
         *
         * @param c - center
         * @param rad - radius
         * @param fill - whether the circle should be filled with colour
         * @param col - Colour for circle and fill
         * @param upscale - True means, that point coordinates are in interval <-1, 1>, radius is in interval <0, 2>
         * and need to be upscaled for SVG
         */
        void addCircle(Point c, double rad, bool fill, const std::string& col = COLORS[0], bool upscale = false) {
            if (upscale) {
                c.X = m_width / 2 + c.X * m_width / 2;
                c.Y = m_height / 2 + c.Y * m_height / 2;
            }

            m_file << "   <circle cx=\"" << c.X << "\" cy=\"" << c.Y
                   << "\" r=\"" << rad
                   << "\" stroke=\"" << col << "\" ";
            if (fill) {
                m_file << "fill=\"" << col << "\" ";
            }
            m_file << "/>" << std::endl;
        }

        /**
         * Creates a filled rectangle
         * @param c - center
         * @param width - width of rectangle
         * @param height - height of rectangle
         * @param upscale - True means, that point coordinates are in interval <-1, 1>, width and height are
         * in interval <0, 2> and need to be upscaled for SVG
         */
        void addRect(Point c, double width, double height, const std::string& col = COLORS[0], bool upscale = false) {
            if (upscale) {
                width *= m_width / 2;
                height *= m_height / 2;
                c.X = m_width / 2 + c.X * m_width / 2 - width / 2;
                c.Y = m_height / 2 + c.Y * m_height /2 - height / 2;
            } else {
                c.X -= width / 2;
                c.Y -= height / 2;
            }

            m_file  << "   <rect x=\"" << c.X << "\" y=\"" << c.Y
                    << "\" width=\"" << width
                    << "\" height=\"" << height
                    << "\" fill=\"" << col
                    << "\" />" << std::endl;
        }

        ~SVGFile() {
            m_file << "</svg>\n\n</body>\n</html>";
        }

        const double m_height;
        const double m_width;

    private:
        std::ofstream m_file;
    };

    class Turtle {
    public:
        Turtle(const std::string &filename) :
                m_file(filename),
                m_pos{m_file.m_width/2, m_file.m_height/2} {}

        Turtle(const std::string &filename, double height, double width) :
                m_file(filename, height, width),
                m_pos{width/2, height/2} {}

        void forward(double len) {
            double radDegree = toRad(m_degree);
            Point newPos = m_pos + Vector{cos(radDegree), -sin(radDegree)} * len;
            if (m_drawing) m_file.addLine(m_pos, newPos);
            m_pos = newPos;
        }

        void back(double len) {
            forward(-len);
        }

        void left(double angle, bool radians = false) {
            if (radians) {
                angle = toDeg(angle);
            }
            m_degree += angle;
        }

        void right(double angle, bool radians = false) {
            left(-angle, radians);
        }

        void drawing(bool draw) {
            m_drawing = draw;
        }

    private:
        double toRad(double degrees) {return M_PI/180 * degrees;}
        double toDeg(double radians) { return 180/M_PI * radians;}

        SVGFile m_file;
        Point m_pos;
        double m_degree = 0.0;
        bool m_drawing = true;
    };
};

//TODO possibly add lines at the end