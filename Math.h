#ifndef __MATH_H__
#define __MATH_H__

#include <cmath>
#include "Core.h"
#include <vector>
#include <cstdio>

class GMath
{
    public:
        static double scalar_mult(V4, V4);
        static double scalar_mult(V3, V3);
        static V3 scalar_mult(V3, double);
        static V4 scalar_mult(V4, double);
        static double modul(V4);


        static Point polar_to_dec(double, double);
        static V4 plane_equation(Point, Point, Point);
        static std::vector <V4> get_planeset(std::vector <Point>, std::vector <std::vector <int>>);

        //in 2d
        static double dist_flat(Point, Point);

        //in 3d
        static double dist_stereo(Point, Point);

        static V4 normalize(V4);
            
        static Point real_point(Point, Point);
        static Point find_origin(int, int, double);
        static std::vector <Edge> edges_to_render(std::vector <V4>, std::vector <std::vector <int>>, 
                std::vector <Point>);
        static std::vector <std::vector <Point>> tri_to_render(std::vector <V4>, 
                std::vector <std::vector <int>>, std::vector <Point>);
        static std::vector <V4> visibility(std::vector <V4>, V4);
        static std::vector <std::vector <Point>> delaunay(std::vector <Point>);
        static std::vector <double> brightness(std::vector <V4>, 
                std::vector <std::vector <Point>>, V4);
        static void get_sides(std::vector <std::vector <int>> &, std::vector <std::vector <int>>,
                std::vector <Point>);
        static void convex_hull(std::vector <std::vector <int>> &, std::vector <std::vector <int>>);
        static std::vector <std::vector <int>> gypsy_delon(std::vector <std::vector <int>>);
};

#endif
