#include "Math.h"

double GMath::scalar_mult(V4 vec1, V4 vec2)
{
    double result = vec1.a * vec2.a + vec1.b * vec2.b
                    + vec1.c * vec2.c + vec1.d * vec2.d;
    return result;
}

//for normalize(V4)
V4 GMath::scalar_mult(V4 vec, double g)
{
    return {vec.a * g, vec.b * g, vec.c * g};
}

double GMath::modul(V4 vec)
{
    double result = std::sqrt(std::pow(vec.a, 2) + std::pow(vec.b, 2)
            + std::pow(vec.c, 2) + std::pow(vec.d, 2));
    return result;
}

Point GMath::polar_to_dec(double ro, double phi)
{
    Point point;
    point.rx = ro * std::cos(phi);
    point.ry = ro * std::sin(phi);
    return point;
}

V4 GMath::plane_equation(Point p1, Point p2, Point p3)
{
    // + - +
    p2 = {p2.x - p1.x, p2.y - p1.y, p2.z - p1.z};
    p3 = {p3.x - p1.x, p3.y - p1.y, p3.z - p1.z};
    p1 = {-p1.x, -p1.y, -p1.z};

    Point minors= {(p2.y * p3.z - p3.y * p2.z), (p2.x * p3.z - p3.x * p2.z), 
        (p2.x * p3.y - p3.x * p2.y)};
    V4 result = {minors.x, -minors.y, minors.z, p1.x * minors.x + (-1) * p1.y * minors.y + p1.z * 
        minors.z};
    //printf("%f, %f, %f, %f \n", result.a, result.b, result.c, result.d);
    return result;
}

std::vector <V4> GMath::get_planeset(std::vector <Point> vertex, 
        std::vector <std::vector <int>> sides)
{
    std::vector <V4> result(sides.size());
    for(size_t i = 0; i < sides.size(); ++i)
    {
        result[i] = plane_equation(vertex[sides[i][0]], vertex[sides[i][1]], 
                vertex[sides[i][2]]);
    }
    return result;
}

//in 2d
double GMath::dist_flat(Point p1, Point p2)
{
    double a = std::abs(p2.x - p1.x);
    double b = std::abs(p2.y - p1.y);
    return std::sqrt(std::pow(a, 2) + std::pow(b, 2));
}

//in 3d
double GMath::dist_stereo(Point p1, Point p2)
{
    double a = std::abs(p2.x - p1.x);
    double b = std::abs(p2.y - p1.y);
    double c = std::abs(p2.z - p1.z);
    return std::sqrt(std::pow(a, 2) + std::pow(b, 2) + std::pow(c, 2));
}

V4 GMath::normalize(V4 vec)
{
    double inv_length = 1.0 / std::sqrt(std::pow(vec.a, 2) + std::pow(vec.b, 2) + std::pow(vec.c, 2))
        + std::pow(vec.d, 2);
    return scalar_mult(vec, inv_length);
}

Point GMath::real_point(Point origin, Point a)
{   
    return {origin.x + a.x, origin.y - a.y, origin.z + a.z};
}

//screen center
Point GMath::find_origin(int size_x, int size_y, double k)
{
    return {std::round(size_x / 2), std::round(size_y / 2), k};
}

std::vector <Edge> GMath::edges_to_render(std::vector <V4> planes, 
        std::vector <std::vector <int>> connections, std::vector <Point> obj)
{
    std::vector <Edge> result;
    for(size_t i = 0; i < planes.size(); ++i)
    {
        for(size_t j = 0; j < obj.size(); ++j)
        {
            // if point belongs to plane ...
            V4 v = {obj[j].x, obj[j].y, obj[j].z, 1};
            if(std::abs(std::round(scalar_mult(v, planes[i])*1000)/1000) == 0.000)
            {
                for(size_t t = 0; t < connections[j].size(); ++t)
                {
                    V4 v1 = {obj[connections[j][t]].x, obj[connections[j][t]].y, 
                        obj[connections[j][t]].z, 1};
                    if(std::abs(std::round(scalar_mult(v1, planes[i])*1000)/1000) == 0.000)
                    {
                        Edge edges = {obj[j], obj[connections[j][t]]};
                        result.push_back(edges);
                    }
                }
            }
        }
    }
    return result;
}

// here we take visible sides (equations of planes) 
// and mesure brightness (from max -> min)
std::vector <double> GMath::brightness(std::vector <V4> planes, 
        std::vector <std::vector <Point>> tri_out, V4 light)
{
    std::vector <double> result(tri_out.size());
    double max = -1;

    bool flag = true; // means, that all 3 verticies are on current plane
    for(size_t i = 0; i < planes.size(); ++i)
    {
        for(size_t j = 0; j < tri_out.size(); ++j)
        {
            for(size_t t = 0; t < tri_out[j].size(); ++t) // t = {0 .. 2}
            {
                // if point belongs to plane ...
                V4 v = {tri_out[j][t].x, tri_out[j][t].y, tri_out[j][t].z, 1};
                if(std::abs(std::round(scalar_mult(v, planes[i])*1000)/1000) != 0.000)
                {
                    flag = false; 
                }
            }
            if(flag == true)
            {
                result[j] = scalar_mult(light, normalize(planes[i])) / (modul(light)
                    * modul(normalize(planes[i]))) * 100;

                if(result[j] > max)
                {
                    max = result[j];
                }
                if(result[j] < 0)
                    result[j] = 0;
            }
            else
            {
                flag = true;
            }
        }
    }
    return result;
}

std::vector <std::vector <Point>> GMath::tri_to_render(std::vector <V4> planes, 
        std::vector <std::vector <int>> tri, std::vector <Point> obj)
{
    std::vector <std::vector <Point>> result;
    bool flag = true; // green light to push_back in result
    for(size_t i = 0; i < planes.size(); ++i)
    {
        for(size_t j = 0; j < tri.size(); ++j)
        {
            std::vector <Point> tmp = {};
            for(int t = 0; t < tri[j].size(); ++t) // t = {0 .. 2}
            {
                // if point belongs to plane ...
                V4 v = {obj[tri[j][t]].x, obj[tri[j][t]].y, obj[tri[j][t]].z, 1};
                if(std::abs(std::round(scalar_mult(v, planes[i])*1000)/1000) == 0.000)
                {
                    tmp.push_back(obj[tri[j][t]]); 
                }
                else
                {
                    flag = false; // ur unable to push in result
                }
            }
            if(flag == true)
            {
                result.push_back(tmp);
            }
            else
            {
                tmp = {};
                flag = true;
            }
        }
    }
    return result;
}

std::vector <V4> GMath::visibility(std::vector <V4> list, V4 camera)
{
    std::vector <V4> result;
    for(size_t i = 0; i < list.size(); ++i)
    {
        /* change for clockwise or counterclockwise systems! */
         
        if(scalar_mult(camera, list[i]) > 0)
        {
            result.push_back(list[i]);
        }
    }
    return result;
}

void GMath::convex_hull(std::vector <std::vector <int>> &sides, 
        std::vector <std::vector <int>> connections)
{
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
                            prev = new_sides[i].back();
                            new_sides[i].push_back(sides[i][l]);
                            goto endof;
                        }
                    }
                }
            }
        }
    }
    sides = new_sides;
}

/* works only for convex hull!! */
std::vector <std::vector <int>> GMath::gypsy_delon(std::vector <std::vector <int>> sides)
{
    std::vector <std::vector <int>> result;
    for(int i = 0; i < (int)sides.size(); ++i)
    {
        // number of triangles in poly (n - 2), where n -- number of vertecies
        for(int j = 2; j < (int)sides[i].size(); ++j)
        { 
            result.push_back({sides[i][0], sides[i][j - 1], sides[i][j]});
        }
    }
    return result;
}


/* GSA */
struct Dublin
{
    Point p;
    int index;
};

double GMath::side_radius(std::vector <double> nums)
{
    if(nums.empty())
        return 0.0;
    double min = *std::min_element(nums.begin(), nums.end());
    double max = *std::max_element(nums.begin(), nums.end());
    return std::abs(max - min);
}

/* returns 2d flat projection of a 3d plane */
std::vector <Point> GMath::flatten(std::vector <int> side, std::vector <Point> vertex)
{
    std::vector <Point> result(side.size());
    std::vector <double> x_nums;
    std::vector <double> y_nums;
    std::vector <double> z_nums;
    /* find the axis with smallest projection radius */
    for(size_t i = 0; i < side.size(); ++i)
    {
        x_nums.push_back(vertex[side[i]].x);
        y_nums.push_back(vertex[side[i]].y);
        z_nums.push_back(vertex[side[i]].z);
    }
    double x_nums_r = GMath::side_radius(x_nums); 
    double y_nums_r = GMath::side_radius(y_nums); 
    double z_nums_r = GMath::side_radius(z_nums); 
    double key = std::min(std::min(x_nums_r, y_nums_r), z_nums_r);
    /* printf("key from flatten: %f \n", key); */
    if(key == x_nums_r)
        for(size_t i = 0; i < side.size(); ++i)
            result[i] = {y_nums[i], z_nums[i]};
    if(key == y_nums_r)
        for(size_t i = 0; i < side.size(); ++i)
            result[i] = {x_nums[i], z_nums[i]};
    if(key == z_nums_r)
        for(size_t i = 0; i < side.size(); ++i)
            result[i] = {x_nums[i], y_nums[i]};
    return result;
}

/* GSA */
int find_wise(Point a, Point b, Point c)
{
    int val = (b.y - a.y) * (c.x - b.x)
        - (b.x - a.x) * (c.y - b.y);
    if(val == 0)
        return 0;
    return (val > 0) ? 1 : 2;
    /* 2 -- counterclock wise */
}

/* GSA */
struct compare_by_angle
{
    Point p0;
    compare_by_angle(Point p0) {this->p0 = p0;}
    inline bool operator()(const Dublin &a, const Dublin &b)
    {
        int wise = find_wise(p0, a.p, b.p);
        if(wise == 0)
            return (GMath::dist_flat(p0, b.p) >= GMath::dist_flat(p0, a.p)) ? true : false;
        return (wise == 2) ? true : false;
    }
};

/* GSA */
Dublin after_top(std::stack <Dublin> S)
{
    Dublin p = S.top();
    S.pop();
    Dublin res = S.top();
    S.push(p);
    return res;
}

/* Graham Scan Algorithm */
/* FIXME */
/* fix chull inversion -> counterclockwise */
std::vector <int> GMath::chull(std::vector <int> set, std::vector <Point> vertex)
{
    std::vector <int> result;
    if(set.size() < 3)
    {
        printf("ERROR face.size() < 3! \n");
        return {};
    }

    std::vector <Point> p_set_tmp = GMath::flatten(set, vertex);
    std::vector <Dublin> p_set(set.size());
    for(size_t i = 0; i < p_set.size(); ++i)
        p_set[i] = {p_set_tmp[i], set[i]};

    double min_y = p_set[0].p.y;
    int min_index = 0;
    for(size_t i = 1; i < p_set.size(); ++i)
    {
        if(p_set[i].p.y < min_y || (p_set[i].p.y == min_y && p_set[i].p.x < p_set[min_index].p.x))
        {
            min_y = p_set[i].p.y;
            min_index = i;
        }
    }
    std::swap(p_set[0], p_set[min_index]);

    Point p0 = p_set[0].p;
    std::sort(p_set.begin()+1, p_set.end(), compare_by_angle(p0));

    for(size_t i = 0; i < p_set.size(); ++i)
        printf("%f %f\n", p_set[i].p.x, p_set[i].p.y);

    std::stack <Dublin> s;
    s.push(p_set[0]);
    s.push(p_set[1]);
    s.push(p_set[2]);

    for(size_t i = 3; i < p_set.size(); ++i)
    {
        while(find_wise(after_top(s).p, s.top().p, p_set[i].p) != 2)
            s.pop();
        s.push(p_set[i]);
    }

    std::stack <Dublin> s_copy = s;
    while(!s_copy.empty())
    {
        Point pp = s_copy.top().p;
        int ind = s_copy.top().index;
        /* printf("%f, %f -- %d \n", pp.x, pp.y, ind); */
        /* printf("%f, %f -- %d \n", pp.x, pp.y, ind); */
        result.push_back(ind);
        s_copy.pop();
    }
    /* std::reverse(result.begin(), result.end()); */

    for(size_t i = 0; i < result.size(); ++i)
        printf("%d ", result[i]);
    printf("\n");
    return result;
}
