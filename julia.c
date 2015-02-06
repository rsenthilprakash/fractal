#include "png_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>


int in_julia(double x0, double y0, int n, double cx, double cy)
{
    double x = x0;
    double y = y0;
    double xx, yy, c_ang;

    while (n > 0) {
        double x1 = x * x - y * y + cx;
        double y1 = 2 * x * y + cy;
        double val = x * x + y * y;

        n -= 1;

        if (val > 4.0) {
#if 0
            double x_sq = (x1 - x) * (x1 - x);
            double y_sq = (y1 - x) * (y1 - x);
            double c_ang_1 = x_sq / (x_sq + y_sq);

            return (int) (c_ang_1 * 255);
#else
            return 0;
#endif
        }

        x = x1;
        y = y1;
    }
    xx = x * x;
    yy = y * y;
    c_ang = xx / (xx + yy);

    return (int) (c_ang * 255);
}

int in_julia_color(double x0, double y0, int n, double cx, double cy, int *color_val)
{
    double x = x0;
    double y = y0;
    double xx, yy, c_ang;

    while (n > 0) {
        double x1 = x * x - y * y + cx;
        double y1 = 2 * x * y + cy;
        double val = x * x + y * y;

        n -= 1;

        if (val > 4.0) {
#if 1
            double x_sq = (x1 - x) * (x1 - x);
            double y_sq = (y1 - x) * (y1 - x);
            double c_ang_1 = x_sq / (x_sq + y_sq);

            *color_val = (int) (c_ang_1 * 255);
            return 0;
#else
            *color_val = 0;
            return 0;
#endif
        }

        x = x1;
        y = y1;
    }
    xx = x * x;
    yy = y * y;
    c_ang = xx / (xx + yy);

    *color_val = (int) (c_ang * 255);
    return 1;
}

int in_julia_hue(double x0, double y0, int n, double cx, double cy, double *color_val)
{
    double x = x0;
    double y = y0;
    double xx, yy, c_ang;

    while (n > 0) {
        double x1 = x * x - y * y + cx;
        double y1 = 2 * x * y + cy;
        double val = x * x + y * y;

        n -= 1;

        if (val > 4.0) {
#if 1
            double x_sq = (x1 - x) * (x1 - x);
            double y_sq = (y1 - x) * (y1 - x);
            double c_ang_1 = x_sq / (x_sq + y_sq);

            *color_val = c_ang_1 * 360;
            return 0;
#else
            *color_val = 0;
            return 0;
#endif
        }

        x = x1;
        y = y1;
    }
    xx = x * x;
    yy = y * y;
    c_ang = xx / (xx + yy);

    *color_val = c_ang * 360;
    return 1;
}


/*
 *  http://www.cs.rit.edu/~ncs/color/t_convert.html
 *  h - 0 to 360
 *  s - 0 to 1
 *  v - 0 to 1
 */
static void HSVtoRGB(int *red, int *green, int *blue, double h, double s, double v)
{
    int i;
    double f, p, q, t;
    double r, g, b;

    if (s == 0) {
        *red = *green = *blue = (int) (v * 255);
        return;
    }

    h /= 60; /* sector 0 to 5 */
    i = floor(h);
    f = h - i; /* factorial part of h */
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));

    switch (i) {
    case 0:
        r = v;
        g = t;
        b = p;
        break;
    case 1:
        r = q;
        g = v;
        b = p;
        break;
    case 2:
        r = p;
        g = v;
        b = t;
        break;
    case 3:
        r = p;
        g = q;
        b = v;
        break;
    case 4:
        r = t;
        g = p;
        b = v;
        break;
    case 5:
        r = v;
        g = p;
        b = q;
        break;
    default: /* just replicate case 5 */
        r = v;
        g = p;
        b = q;
        break;
    }

    *red = (int) (r * 255);
    *green = (int) (g * 255);
    *blue = (int) (b * 255);

}

int main(int argc, char *argv[])
{
    const int pixels = 1920;
    const double xmin = -1.85;
    const double ymin = -1.85;
    const double xmax = 1.85;
    const double ymax = 1.85;
    const double cx = -0.391;
    const double cy = -0.587;
    const int num_iters = 400;

    int color;
    const char *file_name;

    int width = pixels, height = pixels;
    int i, j;

    double xinc = (xmax - xmin) / pixels;
    double yinc = (ymax - ymin) / pixels;
    double x = xmin;
    double y = ymin;

    unsigned char *buffer = calloc(1, 3 * width * height);

    struct timeval tv_start, tv_end;

    if (argc != 3) {
        printf("Usage:\n %s <file_name> <color(0/1)>\n", argv[0]);
        return -1;
    }

    file_name = argv[1];
    color = atoi(argv[2]);

    gettimeofday(&tv_start, NULL);

    if (color) {
        unsigned char *ptr = buffer;

        for (i = 0; i < height; i++) {
            x = xmin;
            for (j = 0; j < width; j++) {
#if 0
                int val;
                int in_set = in_julia_color(x, y, num_iters, cx, cy, &val);
                if (in_set) {
                    *ptr++ = 128;
                    *ptr++ = val;
                    *ptr++ = val;
                }
                else {
                    *ptr++ = 128;
                    *ptr++ = val;
                    *ptr++ = 255 - val;
                }
#else
                double val;
                int in_set = in_julia_hue(x, y, num_iters, cx, cy, &val);
                int r, g, b;
                if (in_set) {
                    HSVtoRGB(&r, &g, &b, val, 1, 0.8);
                }
                else {
                    HSVtoRGB(&r, &g, &b, val, 0, 0.5);
                }
                *ptr++ = r;
                *ptr++ = g;
                *ptr++ = b;
#endif
                x += xinc;
            }
            y += yinc;
        }
    }
    else {
        unsigned char *ptr = buffer;

        for (i = 0; i < height; i++) {
            x = xmin;
            for (j = 0; j < width; j++) {
                unsigned char val = in_julia(x, y, num_iters, cx, cy);
                *ptr++ = val;
                *ptr++ = val;
                *ptr++ = val;
                x += xinc;
            }
            y += yinc;
        }
    }

    write_png_color(buffer, width, height, file_name);

    free(buffer);

    gettimeofday(&tv_end, NULL);

    printf("timetaken in microseconds: %ld\n",
           ((tv_end.tv_sec - tv_start.tv_sec) * 1000000) + (tv_end.tv_usec - tv_start.tv_usec));

    return 0;
}
