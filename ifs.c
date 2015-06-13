#include "ifs.h"
#include "png_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void print_available_shapes()
{
    size_t num_shapes = sizeof (shape_database) / sizeof (shape_database[0]);
    size_t i;

    printf("available shapes\n");
    for (i = 0; i < num_shapes; i++)
        printf("%s\n", shape_database[i].shape);

    printf("\n");
}

static size_t get_shape_index(const char *shape)
{
    size_t num_shapes = sizeof (shape_database) / sizeof (shape_database[0]);
    size_t i;
    size_t index = INVALID_SHAPE;

    for (i = 0; i < num_shapes; i++) {
        if (!strcmp(shape_database[i].shape, shape)) {
            index = i;
            break;
        }
    }

    printf("%u %s\n", i, shape);

    return index;
}

static void get_scaled_dims(struct dimenstion *scaled_dim, const struct dimenstion *dim, size_t scale)
{
    scaled_dim->width = scale * dim->width;
    scaled_dim->height = scale * dim->height;
    scaled_dim->scale_x = scale * dim->scale_x;
    scaled_dim->scale_y = scale * dim->scale_y;
    scaled_dim->offset_x = scale * dim->offset_x;
    scaled_dim->offset_y = scale * dim->offset_y;
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
                               const struct dimenstion *scaled_dim,
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

    size_t width = scaled_dim->width;
    size_t height = scaled_dim->height;
    size_t scale_x = scaled_dim->scale_x;
    size_t scale_y = scaled_dim->scale_y;
    size_t offset_x = scaled_dim->offset_x;
    size_t offset_y = scaled_dim->offset_y;

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

int main(int argc, char *argv[])
{
    const size_t scale = 200;
    const unsigned long long int num_iters = 100000000;

    unsigned char *img_data;
    char png_name[50];
    char *png_dir;

    size_t num_rules;
    size_t width;
    size_t height;

    struct affineTrans *rules;
    struct dimenstion *dim;
    struct dimenstion scaled_dim;

    const char *shape;
    size_t shape_index;

    if (argc != 2 && argc != 3) {
        printf ("Usage:\n %s <shape> <optional output dir>\n", argv[0]);
        print_available_shapes();
        return -1;
    }

    shape = argv[1];
    shape_index = get_shape_index(shape);

    if (argc == 3)
        png_dir = argv[2];
    else
        png_dir = ".";

    if (shape_index == INVALID_SHAPE) {
        printf("Invalid shape %s\n", shape);
        print_available_shapes();
        return -2;
    }

    rules = shape_database[shape_index].rules;
    dim = shape_database[shape_index].dim;
    num_rules = shape_database[shape_index].num_rules;

    get_scaled_dims(&scaled_dim, dim, scale);

    width = scaled_dim.width;
    height = scaled_dim.height;

    img_data = malloc(width * height * 3);
    memset(img_data, 50, width * height * 3);

    snprintf(png_name, 50, "%s/%s.png", png_dir, shape);

    printf("%llu %u\n", num_iters, num_rules);

    compute_cdf_for_rules(rules, num_rules);

    create_pattern_rgb(img_data, rules, num_rules, &scaled_dim, num_iters);

    write_png_color(img_data, width, height, png_name);

    free(img_data);

    return 0;
}
