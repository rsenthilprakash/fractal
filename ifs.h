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

/* Add new shapes here
 * some ref:
 * http://www.susqu.edu/brakke/aux/ifs/gallery/gallery.htm
 * http://classes.yale.edu/fractals/IntroToFrac/InvProb/InvProbNatFrac.html
 */

/* a kind of spiral */
struct affineTrans rules_spiral_1[] = { {0.8188251, 0.8711631, -0.1568270, 0.9451363, 0.5044009, 0.2600615, 0.5000000, 0},
                                        {0.3899677, -0.6386825, -0.2169008, -0.1310590, -0.4387605, 0.6664247, 0.5000000, 0} };
struct dimenstion dim_spiral_1 = {10, 10, 1, 2, 2, 4};

/* another kind of spiral */
struct affineTrans rules_spiral_2[] = { {0.79874, -0.29072, 0.29072, 0.79874, 0.0, 0.0, 0.88923, 0},
                                        {0.3, 0.0, 0.0, 0.3, 0.7, 0.0, 0.11077, 0} };
struct dimenstion dim_spiral_2 = {2, 2, 1, 1, 1, 1};


/* Queen Anne's Lace */
struct affineTrans rules_lace[] = { {0.27, 0.0, 0.0, 0.27, 1.0, 0.0, 0.08332, 0},
                                    {0.27, 0.0, 0.0, 0.27, 0.707, 0.707, 0.08332, 0},
                                    {0.27, 0.0, 0.0, 0.27, 0.0, 1.0, 0.08332, 0},
                                    {0.27, 0.0, 0.0, 0.27, -0.707, 0.707, 0.08332, 0},
                                    {0.27, 0.0, 0.0, 0.27, -1.0, 0.0, 0.08332, 0},
                                    {0.27, 0.0, 0.0, 0.27, -0.707, -0.707, 0.08332, 0},
                                    {0.27, 0.0, 0.0, 0.27, 0.0, -1.0, 0.08332, 0},
                                    {0.27, 0.0, 0.0, 0.27, 0.707, -0.707, 0.08332, 0},
                                    {0.46194, -0.19134, 0.19134, 0.46194, 0.0, 0.0, 0.30005, 0} };
struct dimenstion dim_lace = {4, 4, 1, 1, 2, 2};

/* star_1 */
struct affineTrans rules_star_1[] = { {0.4847246,  0.5698310, -0.1388086,  0.7466392, -0.2309255,  1.6087142,  0.5000000, 0},
                                      {0.7415562,  1.0137806, -0.2713263,  0.8852760, -1.8673942, -1.8310378,  0.5000000, 0} };
struct dimenstion dim_star_1 = {40, 40, 1, 1, 20, 20};

/* snowflake */
struct affineTrans rules_snowflake[] = { {0.6, 0.0, 0.0, 0.6, 0.0, 0.0, 0.25, 0},
                                         {0.4, 0.0, 0.0, 0.2, 0.6, 0.0, 0.06, 0},
                                         {0.5, -0.866, 0.866, 0.5, 0.0, 0.0, 0.69, 0} };
struct dimenstion dim_snowflake = {2, 2, 1, 1, 1, 1};


struct shapeData shape_database[] = { {"spiral_1", rules_spiral_1, 2, &dim_spiral_1},
                                      {"spiral_2", rules_spiral_2, 2, &dim_spiral_2},
                                      {"lace", rules_lace, 9, &dim_lace},
                                      {"star_1", rules_star_1, 2, &dim_star_1},
                                      {"snowflake", rules_snowflake, 3, &dim_snowflake} };

#endif
