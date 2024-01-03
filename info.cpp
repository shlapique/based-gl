#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) 
{
    int num_vert = 0;
    int num_faces = 0;
    int num_triang = 0;
    std::string line;
    std::ifstream file(argv[1]);
    if (file.is_open()) 
    {
        while (getline(file, line)) 
        {
            std::string mode = line.substr(0, line.find(' '));
            if (mode == "v") 
                num_vert++;
            else 
            if (mode == "f") 
            {
                num_faces++;
                if (line.find(' ') == line.rfind(' ')) 
                    num_triang++;
            }
        }
        file.close();
        std::cout << "polygons (faces) : " << num_faces << std::endl;
        std::cout << "vertices         : " << num_vert << std::endl;
        std::cout << "triangles        : " << num_triang << std::endl;
    }
    else
    {
        std::cout << "Unable to open file." << std::endl;
    }
    return 0;
}
