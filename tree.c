#include "png_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static unsigned char * alloc_image_buffer(size_t scale, size_t *width, size_t *height, size_t *scale_factor)
{
    unsigned char *buffer;
    size_t w = 6 * scale;
    size_t h = 6 * scale;

    buffer = malloc(w * h * 3);

    memset(buffer, 0, w * h * 3);

    *width = w;
    *height = h;
    *scale_factor = 3 * scale;

    return buffer;
}

static void free_image_buffer(unsigned char *buffer)
{
    free(buffer);
}

static void create_pattern_rgb(unsigned char *img_data, size_t scale, size_t width, size_t height, size_t scale_factor, unsigned long long int num_iters)
{
    unsigned long long int i = num_iters;

    unsigned int seed = time(NULL);

    double x_prev = 0.0;
    double y_prev = 0.0;

    srand48(seed);

    while (i) {
        double rand_num = drand48();

        double x_cur;
        double y_cur;

        double r;
        double s;
        double theta;
        double phi;
        double e;
        double f;

        size_t row;
        size_t col;

        size_t offset;

        if (rand_num < 0.1) {
            r = 0.05;
            s = 0.60;
            theta = 0.0;
            phi = 0.0;
            e = 0.0;
            f = 0.0;
        }
        else if (rand_num < 0.2) {
            r = 0.05;
            s = -0.50;
            theta = 0.0;
            phi = 0.0;
            e = 0.0;
            f = 1.0;
        }
        else if (rand_num < 0.4) {
            r = 0.60;
            s = 0.50;
            theta = 0.698;
            phi = 0.698;
            e = 0.0;
            f = 0.6;
        }
        else if (rand_num < 0.6) {
            r = 0.50;
            s = 0.45;
            theta = 0.349;
            phi = 0.349;
            e = 0.0;
            f = 1.1;
        }
        else if (rand_num < 0.8) {
            r = 0.50;
            s = 0.55;
            theta = -0.524;
            phi = -0.524;
            e = 0.0;
            f = 1.0;
        }
        else {
            r = 0.55;
            s = 0.40;
            theta = -0.698;
            phi = -0.698;
            e = 0.0;
            f = 0.7;
        }

        x_cur = (r * cos(theta) * x_prev) - (s * sin(phi) * y_prev) + e;
        y_cur = (r * sin(theta) * x_prev) + (s * cos(phi) * y_prev) + f;

        row = height - (size_t)(y_cur * scale_factor) - 1;
        col = (size_t)(x_cur * scale_factor) + scale_factor;

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
}

int main(void)
{
    const size_t scale = 1000;
    const unsigned long long int num_iters = 10000000;
    size_t width;
    size_t height;
    size_t scale_factor;
    unsigned char *img_data = alloc_image_buffer(scale, &width, &height, &scale_factor);
    char png_name[25];

    sprintf(png_name, "tree.png");

    printf("%llu\n", num_iters);

    create_pattern_rgb(img_data, scale, width, height, scale_factor, num_iters);

    write_png_color(img_data, width, height, png_name);

    free_image_buffer(img_data);

    return 0;
}
