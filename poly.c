#include "png_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct point
{
    double x;
    double y;
};

static const double x_start = -1.0;
static const double x_limit = 1.0;
static const double y_start = -1.0;
static const double y_limit = 1.0;

static const unsigned int FIX_FAC = 100;

static unsigned char * alloc_image_buffer(size_t scale, size_t *width, size_t *height, size_t *padding)
{
    unsigned char *buffer;
    size_t pad = 10;
    size_t w = (size_t)((x_limit - x_start) * scale) + (2 * pad);
    size_t h = (size_t)((y_limit - y_start) * scale) + (2 * pad);
    //size_t w = (size_t)(3 * scale) + 20;
    //size_t h = (size_t)(2 * scale) + 20;

    buffer = malloc(w * h * 3);

    memset(buffer, 0, w * h * 3);

    *width = w;
    *height = h;
    *padding = pad;

    return buffer;
}

static void free_image_buffer(unsigned char *buffer)
{
    free(buffer);
}

static void get_new_point(struct point *p_mid, const struct point *p1, const struct point *p2, double a)
{
    double b = 1.0 - a;

    p_mid->x = (p1->x * a) + (p2->x * b);
    p_mid->y = (p1->y * a) + (p2->y * b);
}

static void create_pattern_rgb(unsigned char *img_data, size_t scale, size_t width, size_t height, size_t padding, unsigned long long int num_iters, size_t n_vertices, double distance_fraction)
{
    unsigned long long int i = num_iters;

    unsigned int seed = time(NULL);

    struct point p0;
    unsigned long long int count = 0;

    size_t j;
    struct point ver[n_vertices];
    double prob_bins[n_vertices];

    for (j = 0; j < n_vertices; ++j) {
        ver[j].x = cos(2 * M_PI * j / n_vertices);
        ver[j].y = sin(2 * M_PI * j / n_vertices);

        prob_bins[j] = (double)(j + 1) / (double)n_vertices;
    }

    srand48(seed);

    p0.x = (drand48() * x_limit);
    p0.y = 0;

    printf("%lf %lf %llu\n", p0.x, p0.y, count);

    while (i) {
        double rand_num = drand48();

        struct point p_cur;
        size_t ver_ref_index;

        size_t r;
        size_t c;

        size_t offset;

        unsigned char color;

        for (j = 0; j < n_vertices; ++j) {

            if (rand_num < prob_bins[j]) {
                ver_ref_index = j;
                break;
            }
        }

        get_new_point(&p_cur, &p0, &ver[ver_ref_index], distance_fraction);

        r = (size_t)((p_cur.y - y_start) * scale) + padding;
        c = (size_t)((p_cur.x - x_start) * scale) + padding;

        if (r >= height)
            r = height - 1;
        if (c >= width)
            c = width - 1;

        color = (unsigned int)( ((255 * FIX_FAC * (ver_ref_index + 1)) / n_vertices) / FIX_FAC );

        offset = (r * width * 3) + (c * 3);
        *(img_data + offset) = color;
        *(img_data + offset + 1) = 255 - color;
        *(img_data + offset + 2) = 255;

        p0.x = p_cur.x;
        p0.y = p_cur.y;

        --i;
    };
}

int main(void)
{
    const size_t scale = 2000;
    const unsigned long long int num_iters = 1000000000;

    const size_t n_vertices = 3;
    const double distance_fraction = 0.5;

    size_t width;
    size_t height;
    size_t padding;
    unsigned char *img_data = alloc_image_buffer(scale, &width, &height, &padding);
    char png_name[25];

    sprintf(png_name, "poly_%d_%04d.png", n_vertices, (int)(distance_fraction * 1000));

    printf("%llu %u %u %s\n", num_iters, width, height, png_name);

    create_pattern_rgb(img_data, scale, width, height, padding, num_iters, n_vertices, distance_fraction);

    write_png_color(img_data, width, height, png_name);

    free_image_buffer(img_data);

    return 0;
}
