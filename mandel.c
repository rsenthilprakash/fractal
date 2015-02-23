#include "png_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static const size_t pixels = 2000;
static const double xmin = -1.85;
static const double ymin = -1.125;
static const double xmax = 0.70;
static const double ymax = 1.125;
static const unsigned int num_iters = 400;

static bool in_mandel(double x0, double y0, int n, unsigned char *pix_val)
{
    double x = 0;
    double y = 0;

    while (n > 0) {
        double x1 = x * x - y * y + x0;
        double y1 = 2 * x * y + y0;
        double val = x * x + y * y;

        n -= 1;

        if (val > 4.0) {
            //double c_ang = (val) / 4;
            //*pix_val = (unsigned char) (c_ang * 255);

            int cn = (num_iters - n) % 10;

            *pix_val = (unsigned char)(cn * 25);

            return false;
        }

        x = x1;
        y = y1;
    }

    pix_val = 0;
    return true;
}

static void generate_mandel(unsigned char *buffer, size_t width, size_t height)
{
    double xinc = (xmax - xmin) / pixels;
    double yinc = (ymax - ymin) / pixels;
    double x = xmin;
    double y = ymin;

    size_t i;
    size_t j;

    unsigned char *ptr = buffer;

    for (i = 0; i < height; i++) {
        x = xmin;
        for (j = 0; j < width; j++) {
            unsigned char val;
            bool in_set = in_mandel(x, y, num_iters, &val);

            if (in_set) {
                *ptr++ = 255;
                *ptr++ = 255;
                *ptr++ = 255;
            }
            else {
                *ptr++ = val;
                *ptr++ = val;
                *ptr++ = val;
            }
            x += xinc;
        }
        y += yinc;
    }
}

int main(int argc, char *argv[])
{
    const char *file_name = "mandel.png";

    size_t width = pixels;
    size_t height = pixels;

    unsigned char *buffer = calloc(1, 3 * width * height);

    generate_mandel(buffer, width, height);

    write_png_color(buffer, width, height, file_name);

    printf("File %s written\n", file_name);

    free(buffer);

    return 0;
}
