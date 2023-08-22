#include "image.h"

// Operator overloads
Vertex operator+(const Vertex &v1, const Vertex &v2) {return Vertex{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w, Color{v1.c.r + v2.c.r, v1.c.g + v2.c.g, v1.c.b + v2.c.b, std::min(v1.c.alpha + v2.c.alpha, 1.0)}};}
Vertex operator+(const Vertex &v1, double d) {return Vertex{v1.x + d, v1.y + d, v1.z + d, v1.w + d, Color{v1.c.r  + d, v1.c.g + d, v1.c.b + d, std::min(v1.c.alpha + d, 1.0)}};}
Vertex operator+(double d, Vertex &v1) {return Vertex{v1.x + d, v1.y + d, v1.z + d, v1.w + d, Color{v1.c.r + d, v1.c.g + d, v1.c.b + d, std::min(v1.c.alpha + d, 1.0)}};}

Vertex operator-(const Vertex &v1, const Vertex &v2) {return Vertex{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w, Color{v1.c.r - v2.c.r, v1.c.g - v2.c.g, v1.c.b - v2.c.b, std::max(v1.c.alpha - v2.c.alpha, 0.0)}};}
Vertex operator-(const Vertex &v1, double d) {return Vertex{v1.x - d, v1.y - d, v1.z - d, v1.w - d, Color{v1.c.r - d, v1.c.g - d, v1.c.b - d, std::max(v1.c.alpha - d, 0.0)}};}
Vertex operator-(double d, Vertex &v1) {return Vertex{d - v1.x, d - v1.y, d - v1.z, d - v1.w, Color{d - v1.c.r, d - v1.c.g, d - v1.c.b, std::max(d - v1.c.alpha, 0.0)}};}

Vertex operator/(const Vertex &v1, const Vertex &v2) {return Vertex{v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w, Color{v1.c.r / v2.c.r, v1.c.g / v2.c.g, v1.c.b / v2.c.b, std::min(v1.c.alpha/v2.c.alpha, 1.0)}};}
Vertex operator/(const Vertex &v1, double d) {return Vertex{v1.x / d, v1.y / d, v1.z / d, v1.w / d, Color{v1.c.r / d, v1.c.g / d, v1.c.b / d, std::min(v1.c.alpha / d, 1.0)}};}
Vertex operator/(double d, Vertex &v1) {return Vertex{d / v1.x, d / v1.y, d / v1.z, d / v1.w, Color{d / v1.c.r , d / v1.c.g, d / v1.c.b, std::min(d / v1.c.alpha, 1.0)}};}

Vertex operator*(const Vertex &v1, const Vertex &v2) {return Vertex{v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w, Color{v1.c.r * v2.c.r, v1.c.g * v2.c.g, v1.c.b * v2.c.b, v1.c.alpha * v2.c.alpha}};}
Vertex operator*(const Vertex &v1, double d) {return Vertex{v1.x * d, v1.y * d, v1.z * d, v1.w * d, Color{v1.c.r * d, v1.c.g * d, v1.c.b * d, std::min(v1.c.alpha * d, 1.0)}};}
Vertex operator*(double d, Vertex &v1) {return Vertex{v1.x * d, v1.y * d, v1.z * d, v1.w * d, Color{v1.c.r * d, v1.c.g * d, v1.c.b * d, std::min(v1.c.alpha * d, 1.0)}};}

std::ostream& operator<<(std::ostream& out, const Vertex& v) { out << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ')'; return out;}
std::ostream& operator<<(std::ostream& out, const Color& c) { out << "(" << c.r << ", " << c.g << ", " << c.b << ')'; return out;}

Color operator+(const Color &c1, const Color &c2) {return Color{c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, std::min(c1.alpha + c2.alpha , 1.0)};}
Color operator-(const Color &c1, const Color &c2) {return Color{c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, std::max(c1.alpha - c2.alpha , 0.0)};}
Color operator/(const Color &c1, double d) {return Color{c1.r / d, c1.g / d, c1.b / d, c1.alpha / d};}
Color operator*(double d, const Color&c1) {return Color{d * c1.r, d * c1.g, d * c1.b, d * c1.alpha};}

// Image constructor
Image::Image(int w , int h, std::string n): width{w}, height{h}, name{n} {
    png.resize(height * width * 4);
    srgb = false;
}

// Height getter
int Image::getHeight() {return height;}

// Width getter
int Image::getWidth() {return width;}

// Coor getter
Color Image::getColor(){return currentColor;}

// Name getter
std::string const &Image::getName() {return name;}

// Png getter
std::vector<unsigned char> const &Image::getPng() {return png;}

// Set the color
void Image::setColor(int r, int g, int b, double a) {
    currentColor.r = srgb ? std::pow(r / 255.0, 2.4) : r;
    currentColor.g = srgb ? std::pow(g / 255.0, 2.4) : g;
    currentColor.b = srgb ? std::pow(b / 255.0, 2.4) : b;
    // currentColor.alpha = rgba ? std::pow(a, 2.4) : a;
    currentColor.alpha = a;
}

// Apply viewport transformation and add vertex
void Image::addVertex(double x, double y, double z, double w) {
    double x1 = ((x/w) + 1) * ((double) width / 2);
    double y1 = ((y/w) + 1) * ((double) height / 2);
    Vertex v = {x1, y1, z, w};
    vertices.push_back(v);
    
}

// Fill triangle using scanline
void Image::fillTriangle(int i1, int i2, int i3) {
    auto getVertex {
        [this](int dir) -> Vertex {
            if (dir > 0) {
                std::vector<Vertex>::iterator ptr = vertices.begin();
                advance(ptr, dir - 1);
                return *ptr;
            }
            std::vector<Vertex>::reverse_iterator ptr = vertices.rbegin();
            advance(ptr, std::abs(dir) - 1);
            return *ptr;
        }
    };

    Vertex v1 = getVertex(i1);
    Vertex v2 = getVertex(i2);
    Vertex v3 = getVertex(i3);

    // Order the vertices from v1 - v3 by y values descending
    if (v1.y < v2.y)
        swap(v1, v2);
    if (v1.y < v3.y)
        swap(v1, v3);
    if (v2.y < v3.y)
        swap(v2, v3);

    if (cull) {
        Vertex d1 = (v1 - v2) / (v1.x - v2.x);
        Vertex d2 = (v1 - v3) / (v1.x - v3.x);
        Vertex cross = crossProduct(d1, d2);
        if (cross.z > 0)
            return;
    }

    // Get all dda points in y
    std::vector<Vertex> pointsTopMid = DDAy(v1, v2);
    std::vector<Vertex> pointsTopBottom = DDAy(v1, v3);
    std::vector<Vertex> pointsMidBottom = DDAy(v2, v3);

    std::vector<Vertex>::reverse_iterator bigEdge = pointsTopBottom.rbegin();
    std::vector<Vertex>::reverse_iterator smallEdge1 = pointsTopMid.rbegin();
    std::vector<Vertex>::reverse_iterator smallEdge2 = pointsMidBottom.rbegin();

    // compute the scanlines
    while (smallEdge1 < pointsTopMid.rend() && bigEdge < pointsTopBottom.rend()) {
        DDAx(*smallEdge1, *bigEdge);
        bigEdge++;
        smallEdge1++;
    }
    while (smallEdge2 < pointsMidBottom.rend() && bigEdge < pointsTopBottom.rend()) {
        DDAx(*smallEdge2, *bigEdge);
        bigEdge++;
        smallEdge2++;
    }

}

void Image::computePixel(Vertex v) {
    if (!rgba) {
        png[((v.y * width) + v.x)*4 + 0] = srgb ? std::pow(v.c.r, 1.0 / 2.4) * 255.0 : v.c.r;
        png[((v.y * width) + v.x)*4 + 1] = srgb ? std::pow(v.c.g, 1.0 / 2.4) * 255.0 : v.c.g;
        png[((v.y * width) + v.x)*4 + 2] = srgb ? std::pow(v.c.b, 1.0 / 2.4) * 255.0 : v.c.b;
        png[((v.y * width) + v.x)*4 + 3] = rgba ? v.c.alpha * 255.0 : constants::alpha * 255.0;// rgba ? std::pow(v.c.alpha, 1.0 / 2.4) : constants::alpha * 255.0;
    }

    else {
        double rd = std::pow(png[((v.y * width) + v.x)*4 + 0] / 255.0, 2.4);
        double gd = std::pow(png[((v.y * width) + v.x)*4 + 1] / 255.0, 2.4);
        double bd = std::pow(png[((v.y * width) + v.x)*4 + 2] / 255.0, 2.4);
        double ad = png[((v.y * width) + v.x)*4 + 3] / 255.0; //std::pow(png[((v.y * width) + v.x)*4 + 3] / 255.0, 2.4);
        double a = v.c.alpha + ad * (1 - v.c.alpha);
        Color cCol = {rd, gd, bd, a};
       // std::cout << a << std::endl;
        Color newColor = (v.c.alpha / a) * v.c + (((1 - v.c.alpha) * cCol.alpha) / a) * cCol;
        std::cout << newColor << " alpha: " << a << std::endl;
        png[((v.y * width) + v.x)*4 + 0] = std::pow(newColor.r, 1.0 / 2.4) * 255.0;
        png[((v.y * width) + v.x)*4 + 1] = std::pow(newColor.g, 1.0 / 2.4) * 255.0;
        png[((v.y * width) + v.x)*4 + 2] = std::pow(newColor.b, 1.0 / 2.4) * 255.0;
        png[((v.y * width) + v.x)*4 + 3] = a * 255.0;
    }
    // double rd = srgb ? std::pow(png[((v.y * width) + v.x)*4 + 0] / 255.0, 2.4) : png[((v.y * width) + v.x)*4 + 0];
    // double gd = srgb ? std::pow(png[((v.y * width) + v.x)*4 + 1] / 255.0, 2.4) : png[((v.y * width) + v.x)*4 + 0];
    // double bd = srgb ? std::pow(png[((v.y * width) + v.x)*4 + 2] / 255.0, 2.4) : png[((v.y * width) + v.x)*4 + 0];
    // double ad = png[((v.y * width) + v.x)*4 + 3] / 255.0; //std::pow(png[((v.y * width) + v.x)*4 + 3] / 255.0, 2.4);
    // double a = v.c.alpha + ad * (1 - v.c.alpha);
    // Color cCol = {rd, gd, bd, a};
    // // std::cout << a << std::endl;
    // Color newColor = (v.c.alpha / a) * v.c + (((1 - v.c.alpha) * cCol.alpha) / a) * cCol;
    // std::cout << newColor << " alpha: " << a << std::endl;
    // png[((v.y * width) + v.x)*4 + 0] = srgb ? std::pow(newColor.r, 1.0 / 2.4) * 255.0 : newColor.r * 255.0;
    // png[((v.y * width) + v.x)*4 + 1] = srgb ? std::pow(newColor.g, 1.0 / 2.4) * 255.0 : newColor.g * 255.0;
    // png[((v.y * width) + v.x)*4 + 2] = srgb ? std::pow(newColor.b, 1.0 / 2.4) * 255.0 : newColor.b * 255.0;
    // png[((v.y * width) + v.x)*4 + 3] = a * 255.0;
}

// Helper to print out vertices
// void Image::printVertices() {
//     for (auto v : vertices)
//         std::cout << v.x << ", " << v.y << std::endl;
// }

// Helper swap function
void Image::swap(auto &a, auto &b) {
    auto temp = a;
    a = b;
    b = temp;
}

// Compute DDA in x between two vertices
void Image::DDAx(Vertex v1, Vertex v2) {
    if (v1.x == v2.x)
        return;
    if (v1.x > v2.x)
        swap(v1, v2);

    Vertex d = (v2 - v1) / (v2.x - v1.x);
    Vertex offset = (std::ceil(v1.x) - v1.x) * d;
    Vertex p = v1 + offset;

    while (p.x < v2.x) {
        computePixel(p);
        p = p + d;
    }
}

// Compute DDA in y between two vertices
std::vector<Vertex> Image::DDAy(Vertex v1, Vertex v2) {
    // Check if vertices are identical
    if (v1.y == v2.y)
        return {};
    // Swap if v1 > v2
    if (v1.y > v2.y)
        swap(v1, v2);

    std::vector<Vertex> points;

    Vertex d = (v2 - v1) / (v2.y - v1.y);
    Vertex offset = (std::ceil(v1.y) - v1.y) * d;
    Vertex p = v1 + offset;
    while (p.y < v2.y) {
        points.push_back(p);
        p = p + d;
    }
    return points;
}

void Image::setSrgb() {
    srgb = true;
    currentColor = Color {std::pow(currentColor.r / 255.0, 2.4), std::pow(currentColor.g / 255.0, 2.4), std::pow(currentColor.b / 255.0, 2.4)};
}

void Image::setCull() {
    cull = true;
}

void Image::setRgba() {
    rgba = true;
}

Vertex Image::crossProduct(Vertex v1, Vertex v2) {
    return Vertex {v1.y * v2.z - v1.z * v2.y, -1 * (v1.x * v2.z - v1.z * v2.x), v1.x * v2.y - v1.y * v2.x}; 
}

void Image::addPlane(Vertex v) {
    clippingPlanes.push_back(v);
}

double Image::dot(Vertex &v1, Vertex &v2) {
    std::cout << v1 << " | " << v2 << std::endl;
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

bool Image::clip(Vertex v) {
    if (clippingPlanes.size() <= 0)
        return false;
    for (auto &v1 : clippingPlanes) {
        if (dot(v1, v) < 0) {
            std::cout << "exit dot" << std::endl;
            return true;
        }
    }
    return false;
}
