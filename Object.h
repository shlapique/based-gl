#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <vector>
#include <list>
#include "Core.h"
#include "Math.h"

class Object 
{
    public:
        Object();
        void create_pyramid(double a, double h);
        void create_cube(double a);
        void create_tetra(double a);
        void create_trunc_cylinder(double a, double b, double h, int prec);

        std::vector <Point> vertex;
        std::vector <std::vector <int>> planeset;
        std::vector <std::vector <int>> connections;
        std::vector <std::vector <int>> tri; // triangles after triangulation
        std::vector <std::vector <int>> sides; // sides of poly
};

#endif
