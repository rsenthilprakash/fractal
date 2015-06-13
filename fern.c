#include "png_utils.h"
#include <stdio.h>
#include <stdlib.h>

static unsigned char * alloc_image_buffer(size_t scale, size_t *width, size_t *height, size_t *x_offset)
{
    unsigned char *buffer;
    size_t w = 6 * scale;
    size_t h = 10 * scale;

    buffer = malloc(w * h * 3);

    memset(buffer, 0, w * h * 3);

    *width = w;
    *height = h;
    *x_offset = 3 * scale;

    return buffer;
}

static void free_image_buffer(unsigned char *buffer)
{
    free(buffer);
}

static void create_pattern_rgb(unsigned char *img_data, size_t scale, size_t width, size_t height, size_t x_offset, unsigned long long int num_iters)
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

        size_t r;
        size_t c;

        size_t offset;

        if (rand_num < 0.1) {
            x_cur = 0.0;
            y_cur = 0.16 * y_prev;
        }
        else if (rand_num < 0.86) {
            x_cur = (0.85 * x_prev) + (0.04 * y_prev);
            y_cur = (-0.04 * x_prev) + (0.85 * y_prev) + 1.6;
        }
        else if (rand_num < 0.93) {
            x_cur = (0.2 * x_prev) + (-0.26 * y_prev);
            y_cur = (0.23 * x_prev) + (0.22 * y_prev) + 1.6;
        }
        else {
            x_cur = (-0.15 * x_prev) + (0.28 * y_prev);
            y_cur = (0.26 * x_prev) + (0.24 * y_prev) + 0.44;
        }

        r = height - (size_t)(y_cur * scale) - 1;
        c = (size_t)(x_cur * scale) + x_offset;

        if (r >= height)
            r = height - 1;
        if (r < 0)
            r = 0;
        if (c >= width)
            c = width - 1;

        offset = (r * width * 3) + (c * 3);
        *(img_data + offset) = 0;
        *(img_data + offset + 1) = 255;
        *(img_data + offset + 2) = 0;

        x_prev = x_cur;
        y_prev = y_cur;

        --i;
    };
}

int main(int argc, char *argv[])
{
    const size_t scale = 1000;
    const unsigned long long int num_iters = 1000000000;
    size_t width;
    size_t height;
    size_t x_offset;
    unsigned char *img_data = alloc_image_buffer(scale, &width, &height, &x_offset);
    char png_name[50];
    char *png_dir;

    if (argc == 2)
        png_dir = argv[1];
    else
        png_dir = ".";

    snprintf(png_name, 50, "%s/fern.png", png_dir);

    printf("%llu\n", num_iters);

    create_pattern_rgb(img_data, scale, width, height, x_offset, num_iters);

    write_png_color(img_data, width, height, png_name);

    free_image_buffer(img_data);

    return 0;
}
