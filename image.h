#ifndef IMAGE_H
#define IMAGE_H
#include <iostream>
#include <vector>
#include <algorithm>
#include<iterator>
#include <cmath>

namespace constants {
    constexpr double alpha = 1.0;
};

struct Color {
    double r = 255.0;
    double g = 255.0;
    double b = 255.0;
    double alpha = constants::alpha;
};

struct Vertex {
    double x, y, z, w;
    Color c;
};

// Vertex, color, and I/O operations
Vertex operator+(const Vertex &v1, const Vertex &v2);
Vertex operator+(const Vertex &v1, double d);
Vertex operator+(double d, Vertex &v1);
Vertex operator-(const Vertex &v1, const Vertex &v2);
Vertex operator-(const Vertex &v1, double d);
Vertex operator-(double d, Vertex &v1);
Vertex operator/(const Vertex &v1, const Vertex &v2);
Vertex operator/(const Vertex &v1, double d);
Vertex operator/(double d, Vertex &v1);
Vertex operator*(const Vertex &v1, const Vertex &v2);
Vertex operator*(const Vertex &v1, double d);
Vertex operator*(double d, Vertex &v1);
std::ostream& operator<<(std::ostream& out, const Vertex& vertex);
std::ostream& operator<<(std::ostream& out, const Color& c);
Color operator+(const Color &c1, const Color &c2);
Color operator-(const Color &c1, const Color &c2);
Color operator/(const Color &c1, double d);
Color operator*(double d, const Color&c1);

class Image {
    private:
        int width;
        int height;
        std::string name;
        Color currentColor;
        std::vector<Vertex> vertices;
        std::vector<unsigned char> png;
        void DDAx(Vertex v1, Vertex v2);
        Vertex crossProduct(Vertex v1, Vertex v2);
        void computePixel(Vertex v);
        std::vector<Vertex> DDAy(Vertex v1, Vertex v2);
        static void swap(auto &a, auto &b);
        double dot(Vertex &v1, Vertex &v2);
        std::vector<Vertex> clippingPlanes;
        bool clip(Vertex v);
        bool srgb;
        bool cull;
        bool rgba;
    public:
        Image(int w = 0, int h = 0, std::string n = ""); 
        int getHeight();
        Color getColor();
        int getWidth();
        std::vector<unsigned char> const &getPng();
        std::string const &getName();
        void setColor(int r, int g, int b, double a = constants::alpha);
        void setSrgb();
        void setCull();
        void setRgba();
        void addPlane(Vertex v);
        void addVertex(double x, double y, double z, double w);
        void fillTriangle(int i1, int i2, int i3);
       // void printVertices();
};
#endif
