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
    /* get sides out of planeset */
    GMath::get_sides(sides, planeset, vertex);
    /* get convex hull out of sides */
    GMath::convex_hull(sides, connections);
    /* get triangulation for every side */
    this->tri = GMath::gypsy_delon(sides);
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
    
    /* get sides out of planeset */
    GMath::get_sides(sides, planeset, vertex);
    /* get convex hull out of sides */
    GMath::convex_hull(sides, connections);
    /* get triangulation for every side */
    this->tri = GMath::gypsy_delon(sides);
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

    /* get sides out of planeset */
    GMath::get_sides(sides, planeset, vertex);
    /* get convex hull out of sides */
    GMath::convex_hull(sides, connections);
    /* get triangulation for every side */
    this->tri = GMath::gypsy_delon(sides);
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

    /* get sides out of planeset */
    GMath::get_sides(sides, planeset, vertex);
    printf("sides size from trunc cyl: %ld \n", sides.size());
    /* get convex hull out of sides */
    GMath::convex_hull(sides, connections);
    printf("CONVEX sides size from trunc cyl: %ld \n", sides.size());
    /* get triangulation for every side */
    this->tri = GMath::gypsy_delon(sides);
    printf("tri size from tunc cyl: %ld \n", tri.size());

    printf("ALL TRIANGLES after init()\n");
    for(size_t t = 0; t < tri.size(); ++t)
    {
        printf(" [%ld] : { %d %d %d } \n", t, tri[t][0], tri[t][1], tri[t][2]);
    }
    
    printf("number of tri = %ld\n", tri.size());
    printf("number of sides = %ld\n", sides.size());
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
    
    printf("END OF FILE!\n");
    printf("vert_size = %ld \n", vertex.size());
    printf("sides_size = %ld \n", sides.size());
    printf("tri_size = %ld \n", tri.size());
}

void Object::parse_vertex_coords(std::string line)
{
    double Multiplier = 25;
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
    std::vector <int> tmp_planesets = {res_side[0], res_side[1], res_side[2]};
    this->planeset.push_back(tmp_planesets);
    printf("planeset %d %d %d \n", res_side[0], res_side[1], res_side[2]);
}
