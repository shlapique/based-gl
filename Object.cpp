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
    

    /* sides */
    GMath::get_sides(sides, planeset, vertex);

    /* here 'll process sides to make them as "list" by connections */
    std::vector <std::vector <int>> new_sides;
    new_sides.resize(sides.size());
    /* for all sides */
    for(size_t i = 0; i < sides.size(); ++i) 
    {
        new_sides[i].push_back(sides[i][0]);
        bool flag_root = false;
        bool close_flag = false;

        /* prev - back - <- upcoming */
        int prev = sides[i][0];
        int close = -1; 


endof:
        if(new_sides[i].back() != close || flag_root == false)
        {
            /* printf("CLOSE = %d, new_sides[i].back() = %d \n", close, new_sides[i].back()); */
            /* for all con's for elem on side */
            for(size_t m = 0; m < connections[new_sides[i].back()].size(); ++m)
            {
                /* for all elems on side */
                for(size_t l = 0; l < sides[i].size(); ++l)
                {
                    /* first one "good" leaf */
                    if(sides[i][l] == connections[new_sides[i].back()][m] && flag_root == false)
                    {
                        // its first neighbor
                        if(close_flag == false)
                        {
                            close = sides[i][l];
                            close_flag = true;
                            printf("FIRST NEIGHBOR (CLOSE) = %d \n", close);
                        }
                        else
                        {
                            printf("SECOND NEIGHBOR = %d \n", sides[i][l]);
                            flag_root = true;
                            prev = new_sides[i].back();
                            new_sides[i].push_back(sides[i][l]);
                            goto endof;
                        } 
                    }
                    else
                    {
                        if(sides[i][l] == connections[new_sides[i].back()][m] && sides[i][l] != prev)
                        {
                            prev = new_sides[i].back();
                            new_sides[i].push_back(sides[i][l]);
                            goto endof;
                        }
                    }
                }
            }
        }
    }

    for(size_t i = 0; i < new_sides.size(); ++i)
    {
        printf("side[%d]: ", i);
        for(size_t j = 0; j < new_sides[i].size(); ++j)
        {
            printf(" %d", new_sides[i][j]);
        }
        printf("\n");
    }

    this->sides = new_sides;
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
    for(int i = 0; i < prec; ++i)
    {
        this->vertex[i] = {{a * std::cos((6.28 / prec) * i)}, -h / 2 , 
            {b * std::sin((6.28 / prec) * i)}};
        this->vertex[i + prec] = {vertex[i].x, {h / 2 + std::tan(3.14 / 6) * (vertex[0].x 
                - vertex[i].x)}, vertex[i].z};

        if(i == prec - 1)
        {
            this->planeset.push_back({0, i, i + prec});
        }
        else
        { 
            this->planeset.push_back({i + 1, i, i + prec});
        }
    }
    this->planeset.push_back({0, prec / 2 + 1, prec / 2 + 2});
    this->planeset.push_back({prec + prec / 2 + 2, prec + prec / 2 + 1, prec});

    for(int i = 0; i < (int)vertex.size(); ++i)
    {
        if(i == 0)
            this->connections.push_back({prec - 1, 1, prec});
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
                        if (i < prec)
                            this->connections.push_back({i - 1, i + 1, i + prec});
                        else
                            this->connections.push_back({i - 1, i + 1, i - prec});
                    }
                }
            }
        }
    }
 
    printf("CONNECTIONS: \n");
    for(size_t i = 0; i < connections.size(); ++i)
    {
        printf("[%d]: ", i);
        for(size_t j = 0; j < connections[i].size(); ++j)
        {
            printf(" %d", connections[i][j]);
        }
        printf("\n");
    }

    //sides
    // *** 
    GMath::get_sides(sides, planeset, vertex);

    /* here 'll process sides to make them as "list" by connections */
    std::vector <std::vector <int>> new_sides;
    new_sides.resize(sides.size());
    /* for all sides */
    for(size_t i = 0; i < sides.size(); ++i) 
    {
        /* printf("side [%d]: ", i); */
        new_sides[i].push_back(sides[i][0]);
        /* printf("root (%d) ", new_sides[i][0]); */
        bool flag_root = false;
        bool close_flag = false;

        /* prev - back - <- upcoming */
        int prev = sides[i][0];
        int close = -1; 

endof:
        if(new_sides[i].back() != close || flag_root == false)
        {
            /* for all con's for elem on side */
            printf("\nHEAD = %d \n", new_sides[i].back());
            for(size_t m = 0; m < connections[new_sides[i].back()].size(); ++m)
            {
                printf("connnect [%d] \n", connections[new_sides[i].back()][m]);
                /* for all elems on side */
                for(size_t l = 0; l < sides[i].size(); ++l)
                {
                    /* first one "good" leaf */
                    if(sides[i][l] == connections[new_sides[i].back()][m] && flag_root == false)
                    {
                        // its first neighbor
                        if(close_flag == false)
                        {
                            close = sides[i][l];
                            close_flag = true;
                        }
                        else
                        {
                            flag_root = true;
                            new_sides[i].push_back(sides[i][l]);
                            goto endof;
                        } 
                    }
                    else
                    {
                        if(sides[i][l] == connections[new_sides[i].back()][m] && sides[i][l] != prev)
                        {
                            printf("root + %d = %d \n", new_sides[i].size(), sides[i][l]);
                            prev = new_sides[i].back();
                            new_sides[i].push_back(sides[i][l]);
                            goto endof;
                        }
                    }
                }
            }
        }
    }

    printf("\n SIDES as convex closed hull\n");
    for(size_t i = 0; i < new_sides.size(); ++i)
    {
        printf("side[%d]: ", i);
        for(size_t j = 0; j < new_sides[i].size(); ++j)
        {
            printf(" %d", new_sides[i][j]);
        }
        printf("\n");
    }

    this->sides = new_sides;
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
