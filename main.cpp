#include <fstream>
#include <string>
#include <sstream>
#include <typeinfo>  
#include "image.h"
#include "lodepng.h"

// Source: https://itecnote.com/tecnote/c-how-to-check-if-string-ends-with-txt/
bool has_suffix(const std::string &str, const std::string &suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// File IO source: learncpp.com
int main(int argc, char *argv[]) {
    std::ifstream inf{ argv[1]};
    std::vector<std::string> line;
    Image *img;

    if (argc != 2) {
        std::cerr << "No input file!\n";
        return 1;
    }
    if (!inf) {
        std::cerr << "Error opening file!\n";
        return 1;
    }

    while (inf)
    {   
        line.clear();
        std::string word;
        std::string strInput;

        std::getline(inf, strInput);
        std::stringstream s(strInput);

        while (s >> word)  
           line.push_back(word);
        if (line.empty())
            continue;

        if (line[0] == "png") {
            if (line.size() != 4 || typeid(line[3]) != typeid(std::string) || !has_suffix(line[3], ".png"))
                break;

            // revisit this if necessary
            // if (img.name != "") {
            //     unsigned error = lodepng::encode(img.name, img.png, img.width, img.height);
            //     if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
            // }
            img = new Image(std::stoi(line[1]), std::stoi(line[2]), line[3]);
            unsigned error = lodepng::encode(img->getName(), img->getPng(), img->getWidth(), img->getHeight());
            if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
        }

        else if (line[0] == "xyzw") {
            if (line.size() != 5) 
                break;
            
            // Grab pixel values
            double x = std::stof(line[1]);
            double y = std::stof(line[2]);
            double z = std::stof(line[3]);
            double w = std::stof(line[4]);
            
            // Add vertex
            img->addVertex(x,y,z,w);

        }

        else if (line[0] == "rgb") {
            if (line.size() != 4)
                break;
            img->setColor(std::stoi(line[1]), std::stoi(line[2]), std::stoi(line[3]));
        }

        else if (line[0] == "rgba") {
            if (line.size() != 5)
                break;
            img->setRgba();
            img->setColor(std::stoi(line[1]), std::stoi(line[2]), std::stoi(line[3]), std::stof(line[4]));
        }

        else if (line[0] == "tri") {
            if (line.size() != 4)
                break;
            img->fillTriangle(std::stoi(line[1]), std::stoi(line[2]), std::stoi(line[3]));
        }

        else if (line[0] == "sRGB") 
            img->setSrgb();
        
        else if (line[0] == "cull")
            img->setCull();
        
        else if (line[0] == "clipplane") {
            if (line.size() != 5)
                break;
            img->addPlane(Vertex{std::stof(line[1]), std::stof(line[2]), std::stof(line[3]), std::stof(line[4])});
        }
        
    }

    if (img->getName() != "") {
        unsigned error = lodepng::encode(img->getName(), img->getPng(), img->getWidth(), img->getHeight());
        if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
    }
    delete img;
}