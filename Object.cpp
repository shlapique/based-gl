#include "Object.h"

Object::Object()
{
    printf("++++EMPTY OBJ CREATED!++++\n");
}

void Object::create_pyramid(double a, double h)
{
    printf("_____PYRAMID's CREATED______\n");
    double r = a / 2 * 1.4 + a / 2;
    this->vertex = {{a / 2, -h / 2, -r}, {r, -h / 2, -a / 2}, {r, -h / 2, a / 2}, {a / 2, -h / 2, r},
                    {-a / 2, -h / 2, r}, {-r, -h / 2, a / 2}, {-r, -h / 2, -a / 2}, {-a / 2, -h / 2, -r}, {0, h / 2, 0}};
    this->planeset = 
    {{0, 1, 2}, // bottom
     {0, 7, 8}, // plane0
     {1, 0, 8}, // .. 
     {2, 1, 8},
     {3, 2, 8}, // .. 
     {4, 3, 8},
     {5, 4, 8},
     {6, 5, 8}, // .. 
     {7, 6, 8}};// plane7

    this->connections = 
    {{1, 7, 8},
     {0, 2, 8},
     {1, 3, 8},
     {2, 4, 8},
     {3, 5, 8},
     {4, 6, 8},
     {5, 7, 8},
     {6, 0, 8},
     {0, 1, 2, 3, 4, 5, 6, 7}};
}

void Object::create_cube(double a)
{
    printf("_____CUBE's CREATED______\n");
    this->vertex = {{a / 2, a / 2, -a / 2}, {a / 2, a / 2, a / 2}, {-a / 2, a / 2, a / 2}, {-a / 2, a / 2, -a / 2},
                    {a / 2, -a / 2, -a / 2}, {a / 2, -a / 2, a / 2}, {-a / 2, -a / 2, a / 2}, {-a / 2, -a / 2, -a / 2}};
    this->planeset = 
    {{0, 3, 2}, // up
     {1, 2, 5}, // back
     {1, 5, 4}, // right
     {4, 5, 6}, // bottom
     {6, 2, 3}, // left
     {0, 4, 7}};// front

    this->connections = 
    {{1, 3, 4},
     {0, 2, 5},
     {1, 3, 6},
     {2, 0, 7},
     {0, 5, 7},
     {4, 1, 6},
     {5, 2, 7},
     {6, 3, 4}};
    
    //sides
    // *** 
    std::vector <V4> planes = GMath::get_planeset(vertex, planeset);
    sides.resize(planeset.size());
    for(size_t i = 0; i < planes.size(); ++i)
    {
        printf("\tSIDE[%d]: ", i);
        for(size_t j = 0; j < vertex.size(); ++j)
        {
            // if point belongs to plane ...
            V4 v = {vertex[j].x, vertex[j].y, vertex[j].z, 1};
            if(std::abs(std::round(GMath::scalar_mult(v, planes[i])*1000)/1000) == 0.000)
            {
                size_t m = j;
                sides[i].push_back(vertex.data() + m);
                printf(" %f", (vertex.data() + m)->y);
            }
        }
        printf("\n");
    } // here we make array in sides

    // here we're making array of triangles
    for(int i = 0; i < (int)sides.size(); ++i)
    {
        // number of triangles in poly (n - 2), where n -- number of vertecies
        for(int j = 2; j < (int)sides[i].size(); ++j)
        { 
            this->tri.push_back({sides[i][0], sides[i][j - 1], sides[i][j]});
        }

        printf("SIZE OF A SIDE VECTOR of %d plane is = %ld\n", i, sides[i].size());
    }
    // *** 

    printf("ALL TRIANGLES after init()\n");
    for(size_t t = 0; t < tri.size(); ++t)
    {
        printf(" [%ld] : { %d %d %d } \n", t, tri[t][0], tri[t][1], tri[t][2]);
    }
    
    printf("number of tri = %ld\n", tri.size());
    printf("number of sides = %ld\n", sides.size());
}


void Object::create_tetra(double a)
{
    printf("_____TETRA's CREATED______\n");
    this->vertex = {{a / 2, -a / 2, -a / 2}, {0, -a / 2, a / 2}, {-a / 2, -a / 2, -a / 2}, {0, a / 2, 0}};
    this->planeset = 
    {{0, 1, 2}, // bottom
     {0, 2, 3}, // front
     {0, 3, 1}, // right
     {1, 3, 2}};// left

    this->connections = 
    {{1, 2, 3},
     {0, 2, 3},
     {1, 3, 0},
     {0, 1, 2}};
}

// a -- for bigger radius of bottom ellipse, b -- for smaller one.
void Object::create_trunc_cylinder(double a, double b, double h, int prec)
{
    printf("_____TRUNC_CYLINGER's CREATED______\n");
    this->vertex.resize(prec * 2);
    Point* base = vertex.data();
    for(int i = 0; i < prec; ++i)
    {
        printf("I + == %d \n", i);
        this->vertex[i] = {{a * std::cos((6.28 / prec) * i)}, -h / 2 , {b * std::sin((6.28 / prec) * i)}};
        this->vertex[i + prec] = {vertex[i].x, {h / 2 + std::tan(3.14 / 6) * (vertex[0].x - vertex[i].x)}, vertex[i].z};

        if(i == 0)
        {
            this->planeset.push_back({0, prec - 1, prec - 1 + prec}); 
        }
        else 
        { 
            this->planeset.push_back({i, i - 1, i - 1 + prec});
        }
    }
    this->planeset.push_back({0, prec / 2 + 1, prec / 2 + 2});
    this->planeset.push_back({prec + prec / 2 + 2, prec + prec / 2 + 1, prec});

    for(int i = 0; i < prec * 2; ++i)
    {
        if(i == 0)
            this->connections.push_back({prec - 1, i + 1, i + prec});
        else 
        { 
            if (i == prec - 1)
                this->connections.push_back({i - 1, 0, i + prec}); 
            else
            {
                if (i == prec)
                    this->connections.push_back({0, i + 1, 2 * prec - 1});
                else
                {
                    if (i == 2 * prec - 1)
                        this->connections.push_back({i - 1, prec, prec - 1});
                    else
                    {
                        this->connections.push_back({i - 1, i + 1, i + prec});
                    }
                }
            }
        }
    }

    //+++++++++++++++++
    /// here we will triangulate all sides of the object
    std::vector <V4> planes = GMath::get_planeset(vertex, planeset);
    sides.resize(planeset.size());
    for(size_t i = 0; i < planes.size(); ++i)
    {
        printf("\tSIDE[%d]: ", i);
        for(size_t j = 0; j < vertex.size(); ++j)
        {
            // if point belongs to plane ...
            V4 v = {vertex[j].x, vertex[j].y, vertex[j].z, 1};
            if(std::abs(std::round(GMath::scalar_mult(v, planes[i])*1000)/1000) == 0.000)
            {
                size_t m = j;
                sides[i].push_back(base + m);
                printf(" %d", base + m);
            }
        }
        printf("\n");
    } // here we make array in sides

    // here we're making array of this->triangles
    for(int i = 0; i < (int)sides.size(); ++i)
    {
        if(i == 0)
        {
            this->tri.push_back({base, base + size_t(prec - 1), base + size_t(2 * prec - 1)}); // 1
            this->tri.push_back({base, base + size_t(prec), base + size_t(2 * prec - 1)});     // 2
        }
        if(i != 0)
        {
            if(i > 0 && i < prec)  // {1 .. prec - 1}
            {
                this->tri.push_back({base + size_t(i), base + size_t(i - 1), 
                        base + size_t(i - 1 + prec)});
                this->tri.push_back({base + size_t(i), base + size_t(i + prec), 
                        base + size_t(i - 1 + prec)});
            }

            if(i == prec)
            {
                // number of triangles in poly (n - 2), where n -- number of vertecies
                for(int j = 2; j < (int)sides[i].size(); ++j)
                { 
                    this->tri.push_back({base, base + size_t(j - 1), base + size_t(j)});
                }
            }
            if(i == prec + 1)
            {
                for(int j = 2; j < (int)sides[i].size(); ++j)
                {
                    this->tri.push_back({base + size_t(prec), base + size_t(prec + j - 1), 
                            base + size_t(prec + j)});
                }
            }
        }
        printf("SIZE OF A SIDE VECTOR of %d plane is = %ld\n", i, sides[i].size());
    }
    //+++++++++++++++++
    printf("ALL TRIANGLES after init()\n");
    for(size_t t = 0; t < tri.size(); ++t)
    {
        printf(" [%ld] : { %d %d %d } \n", t, tri[t][0], tri[t][1], tri[t][2]);
    }
    
    printf("number of tri = %ld\n", tri.size());
    printf("number of sides = %ld\n", sides.size());
}
