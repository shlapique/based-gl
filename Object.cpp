#include "Object.h"

Object::Object()
{
    printf("++++EMPTY OBJ CREATED!++++\n");
}

/* Wavefront .obj stores indecies from 1..n, not 0..n-1!!! */
void Object::load_from_file(const char* path)
{   
    std::string line;
    std::ifstream file(path);
    if(file.is_open())
    {
        while (getline(file, line))
        {
            std::stringstream ss(line);
            std::string mod;
            ss >> mod;
            if(mod == "v")
            {
                parse_vertex_coords(line);
            }
            else if(mod == "f")
            {
                parse_faces(line);
            }
        }
        file.close();
    }
    else
        printf("unable to open file \n");

    //+++
    /* we already have convex hull for every side (bcs of .obj Wavefront format) */
    //GMath::convex_hull(sides, connections);

    /* get triangulation for every side */
    this->tri = GMath::gypsy_delon(sides);
    //---
    
    /* printf("TEST TEST TEST\n"); */
    /* for(size_t i = 0; i < sides.size(); ++i) */
    /* { */
    /*     sides[i] = GMath::chull(sides[i], vertex); */
    /* } */

    printf("END OF FILE!\n");
    printf("vert_size = %ld \n", vertex.size());
    printf("sides_size = %ld \n", sides.size());
    printf("tri_size = %ld \n", tri.size());
}

void Object::parse_vertex_coords(std::string line)
{
    double Multiplier = 1;
    double x;
    double y;
    double z;
    line.erase(0, 2);
    std::stringstream ss(line);
    ss >> x >> y >> z;
    Point tmp = {x * Multiplier, y * Multiplier, z * Multiplier};

    printf("x %f, y %f, z %f \n", x, y, z);
    this->vertex.push_back(tmp);
}

void Object::parse_faces(std::string line)
{
    line.erase(0, 2);
    std::stringstream ss(line);
    std::vector <int> res_side;
    printf("side: ");
    while(!ss.eof())
    {
        /* part of a line, exp "1//2" */
        /* or "1/2/3", we want only '1' from this */
        std::string tmp;
        ss >> tmp;
        /* find first occurrence of '/' */
        /* std::cout << "s = " << tmp << std::endl; */
        int pos = tmp.find("/");

        /* note: .obj models from CINEMA 4D 'll corrupt stoi here (exra it */
        /*  * adds space for 'f' lines)!! */
        /* run: ./prep_model urmodel.obj */ 
        int tmp_index = std::stoi(tmp.substr(0, pos));

        /* Wavefront .obj stores indecies from 1..n, not 0..n-1!!! */
        /* -> */
        tmp_index--;

        res_side.push_back(tmp_index);
        printf("%d ", tmp_index); 
    }
    printf(" \n");
    this->sides.push_back(res_side);
}
