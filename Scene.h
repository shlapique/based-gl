#ifndef __SCENE_H__
#define __SCENE_H__

#include "Core.h"
#include "Math.h"
#include "Object.h"
#include <SDL2/SDL.h>

class Scene : public Object
{
    public:
        Scene();
        Scene(SDL_Renderer *renderer, V4, V4);
        Point rotate_x(Point, double, int);
        Point rotate_y(Point, double, int);
        Point rotate_z(Point, double, int);
        void rotate(Axis, double, int, double);

        //perspective projection
        Point point_central_projection(Point, double);
        //perspective projection for array of edges
        void edges_central_projection(Point, double);
        void central_projection(Point, double);
        void tri_central_projection(Point, double);

        //isometric projection for array of Points
        void vertex_isometric_projection(Point);
        //isomtric projection for array of edges
        void edges_isometric_projection(Point);
        void isometric_projection(Point);

        void transform(double);

        void draw_segment(SDL_Renderer *renderer, Point a, Point b, Color color);
        void draw_obj(SDL_Renderer *renderer, std::vector <Edge> edges, 
                std::vector <std::vector <Point>>, std::vector <double>, Color, Color);

        //=====
        void draw(Color color_carcas, Color color_sides);

    private:
        SDL_Renderer *renderer = NULL; 
        double *mult;
        int *dir; //default for my SC is clockwise (oZ looks away from me)
        Axis *axis; // to change axis of rotation
        double scale = 1; // size of obj in "times"
        double scale_time = 1; // default distance (k) from proj to screen
        double k = 600;
        Color color = {231, 222, 111};
        V4 camera;
        V4 light;
        std::vector <Edge> edges;
        std::vector <std::vector <Point>> tri_out; // tri out to draw()
        std::vector <double> bright; // brightness of each triangle with corresp index
};

#endif
