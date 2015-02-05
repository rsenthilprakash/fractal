#ifndef IFS_H_INCLUDED
#define IFS_H_INCLUDED

#include <stddef.h>

struct affineTrans
{
    double a;
    double b;
    double c;
    double d;
    double e;
    double f;
    double prob;
    double cdf;
};

struct dimenstion
{
    size_t width;
    size_t height;
    size_t scale_x;
    size_t scale_y;
    size_t offset_x;
    size_t offset_y;
};

struct shapeData {
    char *shape;
    struct affineTrans *rules;
    size_t num_rules;
    struct dimenstion *dim;
};

static const size_t INVALID_SHAPE = 10000;

/* Add new shapes here */

/* a kind of spiral */
struct affineTrans rules_spiral_1[] = { {0.8188251, 0.8711631, -0.1568270, 0.9451363, 0.5044009, 0.2600615, 0.5000000, 0},
                                        {0.3899677, -0.6386825, -0.2169008, -0.1310590, -0.4387605, 0.6664247, 0.5000000, 0} };
struct dimenstion dim_spiral_1 = {10, 10, 1, 2, 2, 4};

/* another kind of spiral */
struct affineTrans rules_spiral_2[] = { {0.79874, -0.29072, 0.29072, 0.79874, 0.0, 0.0, 0.88923, 0},
                                        {0.3, 0.0, 0.0, 0.3, 0.7, 0.0, 0.11077, 0} };
struct dimenstion dim_spiral_2 = {2, 2, 1, 1, 1, 1};


/* snowflake */
struct affineTrans rules_snowflake[] = { {0.6, 0.0, 0.0, 0.6, 0.0, 0.0, 0.25, 0},
                                         {0.4, 0.0, 0.0, 0.2, 0.6, 0.0, 0.06, 0},
                                         {0.5, -0.866, 0.866, 0.5, 0.0, 0.0, 0.69, 0} };
struct dimenstion dim_snowflake = {2, 2, 1, 1, 1, 1};


struct shapeData shape_database[] = { {"spiral_1", rules_spiral_1, 2, &dim_spiral_1},
                                      {"spiral_2", rules_spiral_2, 2, &dim_spiral_2},
                                      {"snowflake", rules_snowflake, 3, &dim_snowflake} };

#endif
