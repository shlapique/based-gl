#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <cmath>
#include <vector>
#include <ctime>

#include "Core.h"
#include "Math.h"
#include "Scene.h"
#include "Object.h"
 
int main(int argc, char *argv[])
{
    int size_x = 640;
    int size_y = 480; 
    double mult = 0;
    int dir = 1; //default for my SC is clockwise (oZ looks away from me)
    Axis axis = X; // to change axis of rotation
    double scale = 1; // size of obj in "times"
    double scale_time = 1;

    /// default distance (k) from proj to screen
    double k = 600;
    /* V4 light = {-1000, 0, 0, 1}; */
    V4 light = {0, 0, -600, 1};
    V4 camera = {0, 0, -600, 1};

    Color color_carcas = {2, 0, 200};
    /* Color color_sides = {155, 38, 182}; */
    /* Color color_sides = {85, 36, 61}; */
    Color color_sides = {231, 222, 111};
    ///
    ///
    /* for fps counter */
    clock_t cur_ticks, delta_ticks;
    clock_t fps = 0;
    SDL_Rect rect;
    char text[200];
    SDL_Color text_color{155, 38, 182, 255};
    /* fps */               

    if (SDL_Init(SDL_INIT_VIDEO) == 0) 
    {
        SDL_Window *window = NULL;
        SDL_Renderer *renderer = NULL;

        if (SDL_CreateWindowAndRenderer(size_x, size_y, SDL_WINDOW_RESIZABLE, &window, &renderer) == 0) 
        {
            SDL_bool done = SDL_FALSE;
            TTF_Init();
            TTF_Font * font = TTF_OpenFont("./font/SweetsSmile.ttf", 23);
            if (font == NULL)
            {
                printf("error ! font not found\n");
                exit(EXIT_FAILURE);
            }

            //+++++++++++++++
            Scene scene(renderer, camera, light);
            scene.create_trunc_cylinder(500, 300, 500, 200);
            /* scene.create_cube(500); */
            /* scene.create_tetra(500); */
            /* scene.create_pyramid(500, 1000); */
            //+++++++++++++++

            while (!done) 
            {
                cur_ticks = clock();
                SDL_Event event;
                int x, y;
                Uint32 buttons;
                SDL_PumpEvents();  // make sure we have the latest mouse state.
                buttons = SDL_GetMouseState(&x, &y);
                SDL_GetWindowSize(window, &size_x, &size_y);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer); 

                //find center (origin)
                Point origin = GMath::find_origin(size_x, size_y, k);

                //////
                scene.rotate(axis, mult, dir, k);
                scene.central_projection(origin, k);
                scene.draw(color_carcas, color_sides);
                mult = 0;
                //////

                delta_ticks = clock() - cur_ticks;
                if(delta_ticks > 0)
                {
                    fps = CLOCKS_PER_SEC / delta_ticks;
                }   
                sprintf(text, "%ld", fps); 
                scene.render_text(renderer, 0, 0, text, font, &rect, &text_color);
                //+++++
                SDL_RenderPresent(renderer);
                //+++++
                while (SDL_PollEvent(&event))
                {
                    switch (event.type)
                    {
                        case SDL_QUIT:
                            done = SDL_TRUE;
                            break;
                        case SDL_MOUSEWHEEL:
                            if(event.wheel.y > 0) // scroll up
                            {
                                if(scale_time + 0.1 <= 3)
                                {
                                    scale += 0.1;
                                    scale_time += 0.1;
                                    scene.transform(scale);
                                    //printf("SCALE = %f, scale_time = %f\n", scale, scale_time);
                                    scale = 1;
                                }
                            }
                            else if(event.wheel.y < 0) // scroll down
                            {
                                if(scale_time - 0.1 >= 0.1)
                                {
                                    scale -= 0.1;
                                    scale_time -= 0.1;
                                    scene.transform(scale);
                                    //printf("SCALE = %f, scale_time = %f\n", scale, scale_time);
                                    scale = 1;
                                }
                                else
                                {
                                    //printf("SCALE arg is < 0.1!!!\n");
                                }
                            }
                            break;                            

                        case SDL_KEYDOWN:
                            switch(event.key.keysym.sym)
                            {
                                // SDLK_UP
                                case SDLK_k:
                                    dir = 1;
                                    axis = X;
                                    mult += 0.05;
                                    break;
                                
                                // SDLK_DOWN
                                case SDLK_j:
                                    dir = -1;
                                    axis = X;
                                    mult += 0.05;
                                    break;

                                // SDLK_LEFT
                                case SDLK_h:
                                    dir = 1;
                                    axis = Y;
                                    mult += 0.05;
                                    break;

                                // SDLK_RIGHT
                                case SDLK_l:
                                    dir = -1;
                                    axis = Y;
                                    mult += 0.05;
                                    break;

                                case SDLK_u:
                                    dir = 1;
                                    axis = Z;
                                    mult += 0.05;
                                    break;

                                case SDLK_d:
                                    dir = -1;
                                    axis = Z;
                                    mult += 0.05;
                                    break;

                                default:
                                    break;
                            }
                    }
                }
                SDL_Delay(20);
            }
        }

        TTF_Quit();

        if (renderer) 
        {
            SDL_DestroyRenderer(renderer);
        }
        if (window) 
        {
            SDL_DestroyWindow(window);
        }
    }

    SDL_Quit();
    return 0;
}
