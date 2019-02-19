#include <iostream>
#include <fstream>

const std::string FILE_NAME = "output_image.svg";
const float IMAGE_HEIGHT = 1080;
const float IMAGE_WIDTH = 1920;

class SVGFile {
public:
    SVGFile() : SVGFile(FILE_NAME) {}

    SVGFile(const std::string &file_name) : m_file(file_name) {
        m_file  << "<html>\n<body>\n\n"
                << "<svg height=\""<< IMAGE_HEIGHT << "\" width=\"" << IMAGE_WIDTH << "\">";
    }

    void addLine(double x1, double y1, double x2, double y2) {
        m_file  << "   <line x1=\"" << x1 << "\" y1=\"" << y1
                << "\" x2=\"" << x2 << "\" y2=\"" << y2
                << "\" style=\"stroke:rgb(255,0,0);stroke-width:2\" />" << std::endl;
    }

    ~SVGFile() {
        m_file << "</svg>\n\n</body>\n</html>";
    }

private:
    std::ofstream m_file;
};

int main() {
    SVGFile svgFile;
    svgFile.addLine(100,100,500,500);
}