#include "png_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

struct scaleOffset
{
    size_t scale_x;
    size_t scale_y;
    size_t offset_x;
    size_t offset_y;
};

static unsigned char * alloc_image_buffer_for_scale(size_t scale, size_t *width, size_t *height)
{
    unsigned char *buffer;
    size_t w = 10 * scale;
    size_t h = 10 * scale;

    buffer = malloc(w * h * 3);

    memset(buffer, 0, w * h * 3);

    *width = w;
    *height = h;

    return buffer;
}

static void free_image_buffer(unsigned char *buffer)
{
    free(buffer);
}

static void compute_cdf_for_rules(struct affineTrans *rules, size_t num_rules)
{
    size_t i;

    rules[0].cdf = rules[0].prob;

    for (i = 1; i < num_rules; i++) {
        rules[i].cdf = rules[i - 1].cdf + rules[i].prob;
    }
}

static void create_pattern_rgb(unsigned char *img_data, const struct affineTrans *rules, size_t num_rules,
                               size_t scale, size_t width, size_t height, const struct scaleOffset *scale_offset,
                               unsigned long long int num_iters)
{
    unsigned long long int i = num_iters;

    unsigned int seed = time(NULL);

    double x_prev = 0.0;
    double y_prev = 0.0;

    double x_max = 0;
    double x_min = 10000;
    double y_max = 0;
    double y_min = 10000;

    size_t scale_x = scale_offset->scale_x;
    size_t scale_y = scale_offset->scale_y;
    size_t offset_x = scale_offset->offset_x;
    size_t offset_y = scale_offset->offset_y;

    srand48(seed);

    while (i) {
        double rand_num = drand48();

        double x_cur;
        double y_cur;

        double a;
        double b;
        double c;
        double d;
        double e;
        double f;

        size_t row;
        size_t col;

        size_t offset;

        size_t j;
        size_t rule_index;

        for (j = 0; j < num_rules; j++) {
            if (rand_num < rules[j].cdf) {
                rule_index = j;
                break;
            }
        }

        a = rules[rule_index].a;
        b = rules[rule_index].b;
        c = rules[rule_index].c;
        d = rules[rule_index].d;
        e = rules[rule_index].e;
        f = rules[rule_index].f;

        x_cur = (a * x_prev) + (b * y_prev) + e;
        y_cur = (c * x_prev) + (d * y_prev) + f;

        if (x_cur > x_max)
            x_max = x_cur;
        if (y_cur > y_max)
            y_max = y_cur;
        if (x_cur < x_min)
            x_min = x_cur;
        if (y_cur < y_min)
            y_min = y_cur;

        row = height - ((size_t)(y_cur * scale_y) + offset_y) - 1;
        col = (size_t)(x_cur * scale_x) + offset_x;

        if (row >= height)
            row = height - 1;
        if (row < 0)
            row = 0;
        if (col >= width)
            col = width - 1;

        offset = (row * width * 3) + (col * 3);
        *(img_data + offset) = 255;
        *(img_data + offset + 1) = 255;
        *(img_data + offset + 2) = 255;

        x_prev = x_cur;
        y_prev = y_cur;

        --i;
    };
    printf("%lf %lf %lf %lf\n", x_max, x_min, y_max, y_min);
}

int main(void)
{
    const size_t scale = 1000;
    const unsigned long long int num_iters = 100000000;
    size_t width;
    size_t height;
    unsigned char *img_data = alloc_image_buffer_for_scale(scale, &width, &height);
    char png_name[25];


    struct affineTrans rules[] = { {0.8188251, 0.8711631, -0.1568270, 0.9451363, 0.5044009, 0.2600615, 0.5000000, 0},
                                   {0.3899677, -0.6386825, -0.2169008, -0.1310590, -0.4387605, 0.6664247, 0.5000000, 0} };
    struct scaleOffset scale_offset = {1 * scale, 2 * scale, 2 * scale, 4 * scale};

    /*
    struct affineTrans rules[] = { {0.5,   0.0,   0.0,   0.5,   0.0,   0.0,  0.3333, 0.0},
                                   {0.5,   0.0,   0.0,   0.5,   0.5,   0.0,  0.3333, 0.0},
                                   {0.5,   0.0,   0.0,   0.5,   0.0,   0.5,  0.3333, 0.0} };
    */

    size_t num_rules = sizeof (rules) / sizeof (rules[0]);

    sprintf(png_name, "ifs.png");

    printf("%llu %u\n", num_iters, num_rules);

    compute_cdf_for_rules(rules, num_rules);

    create_pattern_rgb(img_data, rules, num_rules, scale, width, height, &scale_offset, num_iters);

    write_png_color(img_data, width, height, png_name);

    free_image_buffer(img_data);

    return 0;
}
