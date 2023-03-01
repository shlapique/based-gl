#include "Scene.h"

Scene::Scene()
{
    printf("/// EMPTY SCENE CREATED! ///\n");
}

Scene::Scene(SDL_Renderer *renderer, V4 camera, V4 light)
{
    this->renderer = renderer;
    this->camera = camera;
    this->light = light;
}

// Where dir = 1 or -1
Point Scene::rotate_x(Point a, double phi, int dir)
{
    Point result;
    result.x = a.x;
    result.y = a.y * std::cos(phi) - dir * a.z * std::sin(phi);
    result.z = dir * a.y * std::sin(phi) + a.z * std::cos(phi);
    return result;
}

Point Scene::rotate_y(Point a, double phi, int dir)
{
    Point result;
    result.x = a.x * std::cos(phi) + dir * a.z * std::sin(phi);
    result.y = a.y;
    result.z = -dir * a.x * std::sin(phi) + a.z * std::cos(phi);
    return result;
}

Point Scene::rotate_z(Point a, double phi, int dir)
{
    Point result;
    result.x = a.x * std::cos(phi) - dir * a.y * std::sin(phi);
    result.y = dir * a.x * std::sin(phi) + a.y * std::cos(phi);
    result.z = a.z;
    return result;
}

//perspective projection
Point Scene::point_central_projection(Point a, double k)
{
    Point result;
    result.y = (k * a.y) / (a.z + k);
    result.x = (k * a.x) / (a.z + k);
    return result;
}

//perspective projection for array of edges
void Scene::edges_central_projection(Point origin, double k)
{
    for(size_t i = 0; i < edges.size(); ++i)
    {
        this->edges[i] = {GMath::real_point(origin, point_central_projection(edges[i].a, k)),
                    GMath::real_point(origin, point_central_projection(edges[i].b, k))};
    }
}

void Scene::tri_central_projection(Point origin, double k)
{
    for(size_t i = 0; i < tri_out.size(); ++i)
    {
        this->tri_out[i] = {GMath::real_point(origin, point_central_projection(tri_out[i][0], k)),
                    GMath::real_point(origin, point_central_projection(tri_out[i][1], k)), GMath::real_point(origin, point_central_projection(tri_out[i][2], k))};
    }
}


void Scene::vertex_isometric_projection(Point origin)
{
    for(size_t i = 0; i < vertex.size(); ++i)
    {
        this->vertex[i] = GMath::real_point(origin, vertex[i]);
    }
}

//isometric projection for array of edges
void Scene::edges_isometric_projection(Point origin)
{
    for(size_t i = 0; i < edges.size(); ++i)
    {
        this->edges[i] = {GMath::real_point(origin, edges[i].a), GMath::real_point(origin, edges[i].b)};
    }
}

void Scene::isometric_projection(Point origin)
{
    this->edges = GMath::edges_to_render(GMath::visibility(GMath::get_planeset(vertex, planeset), 
                camera), 
            connections, vertex);
    edges_isometric_projection(origin);
}

void Scene::transform(double k)
{
    for(size_t i = 0; i < vertex.size(); ++i)
    {
        this->vertex[i] = {vertex[i].x * k, vertex[i].y * k, vertex[i].z * k}; 
    }
}

void Scene::rotate(Axis axis, double mult, int dir, double k)
{
    for(size_t i = 0; i < vertex.size(); ++i)
    {
        switch(axis)
        {
            case X:
                this->vertex[i] = rotate_x(vertex[i], mult, dir);
                if(mult != 0)
                {
                    printf("A%ld = {%f, %f, %f}\n", i, vertex[i].x, vertex[i].y, vertex[i].z);
                }
                break;

            case Y:
                this->vertex[i] = rotate_y(vertex[i], mult, dir);
                if(mult != 0)
                {
                    printf("A%ld = {%f, %f, %f}\n", i, vertex[i].x, vertex[i].y, vertex[i].z);
                }
                break;

            case Z:
                this->vertex[i] = rotate_z(vertex[i], mult, dir);
                if(mult != 0)
                {
                    printf("A%ld = {%f, %f, %f}\n", i, vertex[i].x, vertex[i].y, vertex[i].z);
                }
                break;
            default:
                break;
        }
    }
}

void Scene::draw_segment(SDL_Renderer *renderer, Point a, Point b, Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
}


void Scene::draw_obj(SDL_Renderer *renderer, std::vector <Edge> edges, 
        std::vector <std::vector <Point>> tri_out, 
        std::vector <double> bright, Color color_carcas, 
        Color color_sides)
{
    SDL_SetRenderDrawColor(renderer, color_carcas.r, color_carcas.g, 
            color_carcas.b, SDL_ALPHA_OPAQUE);

    // 0 stands for a, 1 stands for b
    /* for(size_t i = 0; i < edges.size(); ++i) */
    /* { */
    /*     /1* draw_segment(renderer, edges[i].a, edges[i].b, color_carcas); *1/ */
    /* } */

    SDL_SetRenderDrawColor(renderer, color_sides.r, color_sides.g, color_sides.b, SDL_ALPHA_OPAQUE);
    for(size_t j = 0; j < tri_out.size(); ++j)
    {
        Uint8 real_r = color_sides.r * bright[j] / 100;
        Uint8 real_g = color_sides.g * bright[j] / 100;
        Uint8 real_b = color_sides.b * bright[j] / 100;
        std::vector <SDL_Vertex> verts =
        {{SDL_FPoint{(float)tri_out[j][0].x, (float)tri_out[j][0].y}, SDL_Color{real_r, real_g, real_b, 255 }, SDL_FPoint{ 0 }},
            {SDL_FPoint{(float)tri_out[j][1].x, (float)tri_out[j][1].y}, SDL_Color{real_r, real_g, real_b, 255 }, SDL_FPoint{ 0 }},
            {SDL_FPoint{(float)tri_out[j][2].x, (float)tri_out[j][2].y}, SDL_Color{real_r, real_g, real_b, 255 }, SDL_FPoint{ 0 }}};
        SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);
    }
}

void Scene::central_projection(Point origin, double k)
{
    std::vector <V4> visible_planesets = GMath::visibility(GMath::get_planeset(vertex, planeset), 
            camera);
    this->edges = GMath::edges_to_render(visible_planesets, connections, vertex);
    this->tri_out = GMath::tri_to_render(visible_planesets, tri, vertex);
    this->bright = GMath::brightness(visible_planesets, tri_out, light);
    edges_central_projection(origin, k);
    tri_central_projection(origin, k);
}

void Scene::draw(Color color_carcas, Color color_sides)
{
    draw_obj(renderer, edges, tri_out, bright, color_carcas, color_sides);
}

void Scene::render_text(SDL_Renderer *renderer, int x, int y, char *text,
        TTF_Font *font, SDL_Rect *rect, SDL_Color *color)
{
    SDL_Surface *surface;
    SDL_Texture *texture;
    surface = TTF_RenderText_Solid(font, text, *color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect->x = x;
    rect->y = y;
    rect->w = surface->w;
    rect->h = surface->h;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, rect);
    SDL_DestroyTexture(texture);
}
