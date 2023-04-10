#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include "Core.h"
#include "Math.h"

const int MAX_FILE_NAME   = 1024;
const int MAX_LINE_LENGTH = 1024;

class Object 
{
    public:
        Object();
        void create_pyramid(double a, double h);
        void create_cube(double a);
        void create_tetra(double a);
        void create_trunc_cylinder(double a, double b, double h, int prec);
        void load_from_file(const char*);
        void parse_vertex_coords(std::string);
        void parse_vertex_normals(std::string);
        void parse_faces(std::string);

        std::vector <Point> vertex;
        std::vector <std::vector <int>> planeset;
        std::vector <std::vector <int>> connections;
        std::vector <std::vector <int>> tri; // triangles after triangulation
        std::vector <std::vector <int>> sides; // sides of poly
};

#endif
