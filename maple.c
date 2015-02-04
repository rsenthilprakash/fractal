#include "png_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static unsigned char * alloc_image_buffer(size_t scale, size_t *width, size_t *height, size_t *scale_factor, size_t *offset)
{
    unsigned char *buffer;
    size_t w = 7 * scale;
    size_t h = 7 * scale;

    buffer = malloc(w * h * 3);

    memset(buffer, 0, w * h * 3);

    *width = w;
    *height = h;
    *scale_factor = (size_t)(1 * scale);
    *offset = (size_t)(3.5 * scale);

    return buffer;
}

static void free_image_buffer(unsigned char *buffer)
{
    free(buffer);
}

static void create_pattern_rgb(unsigned char *img_data, size_t scale, size_t width, size_t height, size_t scale_factor, size_t offset, unsigned long long int num_iters)
{
    unsigned long long int i = num_iters;

    unsigned int seed = time(NULL);

    double x_prev = 0.0;
    double y_prev = 0.0;

    srand48(seed);

    double x_max = 0;
    double x_min = 10000;
    double y_max = 0;
    double y_min = 10000;

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

        size_t pixel_offset;

        if (rand_num < 0.1) {
            a = 0.14;
            b = 0.01;
            c = 0.0;
            d = 0.51;
            e = -0.08;
            f = -1.31;
        }
        else if (rand_num < 0.45) {
            a = 0.43;
            b = 0.52;
            c = -0.45;
            d = 0.5;
            e = 1.49;
            f = -0.75;
        }
        else if (rand_num < 0.8) {
            a = 0.45;
            b = -0.49;
            c = 0.47;
            d = 0.47;
            e = -1.62;
            f = -0.74;
        }
        else {
            a = 0.49;
            b = 0.0;
            c = 0.0;
            d = 0.51;
            e = 0.02;
            f = 1.62;
        }

        x_cur = (a * x_prev) + (b * y_prev) + e;
        y_cur = (c * x_prev) + (d * y_prev) + f;

        if (x_cur > x_max)
            x_max = x_cur;
        if (y_cur > y_max)
            y_max = y_cur;
        if (x_cur < x_min)
            x_min = x_cur;
        if (y_cur < y_min)
            y_min = x_cur;

        row = height - ((size_t)(y_cur * scale_factor) + offset) - 1;
        col = (size_t)(x_cur * scale_factor) + offset;

        if (row >= height)
            row = height - 1;
        if (row < 0)
            row = 0;
        if (col >= width)
            col = width - 1;

        pixel_offset = (row * width * 3) + (col * 3);
        *(img_data + pixel_offset) = 255;
        *(img_data + pixel_offset + 1) = 100;
        *(img_data + pixel_offset + 2) = 0;

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
    size_t scale_factor;
    size_t offset;
    unsigned char *img_data = alloc_image_buffer(scale, &width, &height, &scale_factor, &offset);
    char png_name[25];

    sprintf(png_name, "maple.png");

    printf("%llu\n", num_iters);

    create_pattern_rgb(img_data, scale, width, height, scale_factor, offset, num_iters);

    write_png_color(img_data, width, height, png_name);

    free_image_buffer(img_data);

    return 0;
}
