#include "julia.h"
#include "png_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static bool in_julia(double x0, double y0, int n, double cx, double cy, unsigned char *gray_val, unsigned char *bg_val)
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
            double x_sq = (x1 - x) * (x1 - x);
            double y_sq = (y1 - x) * (y1 - x);
            c_ang = x_sq / (x_sq + y_sq);

            *gray_val = 0;
            *bg_val = (unsigned char) (c_ang * 255);
            return false;
        }

        x = x1;
        y = y1;
    }

    xx = x * x;
    yy = y * y;
    c_ang = xx / (xx + yy);

    *gray_val = (unsigned char) (c_ang * 255);
    *bg_val = 0;

    return true;
}

static bool in_julia_color(double x0, double y0, int n, double cx, double cy, int *color_val)
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
            *color_val = 0;
            return false;
        }

        x = x1;
        y = y1;
    }
    xx = x * x;
    yy = y * y;
    c_ang = xx / (xx + yy);

    *color_val = (int) (c_ang * 255);
    return true;
}

static bool in_julia_hue(double x0, double y0, int n, double cx, double cy, double *color_val)
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
            *color_val = 0;
            return false;
        }

        x = x1;
        y = y1;
    }
    xx = x * x;
    yy = y * y;
    c_ang = xx / (xx + yy);

    *color_val = c_ang * 360;
    return true;
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

static void generate_julia_plain(unsigned char *buffer, size_t width, size_t height)
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
            unsigned char bg;
            bool in_set = in_julia(x, y, num_iters, cx, cy, &val, &bg);

            if (in_set) {
                *ptr++ = 255;
                *ptr++ = 255;
                *ptr++ = 255;
            }
            else {
                *ptr++ = 0;
                *ptr++ = 0;
                *ptr++ = 0;
            }
            x += xinc;
        }
        y += yinc;
    }
}

static void generate_julia_gray(unsigned char *buffer, size_t width, size_t height)
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
            unsigned char bg;
            bool in_set = in_julia(x, y, num_iters, cx, cy, &val, &bg);

            if (in_set) {
                *ptr++ = val;
                *ptr++ = val;
                *ptr++ = val;
            }
            else {
                *ptr++ = 0;
                *ptr++ = 0;
                *ptr++ = 0;
            }
            x += xinc;
        }
        y += yinc;
    }
}

static void generate_julia_gray_bg(unsigned char *buffer, size_t width, size_t height)
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
            unsigned char bg;
            bool in_set = in_julia(x, y, num_iters, cx, cy, &val, &bg);

            if (in_set) {
                *ptr++ = val;
                *ptr++ = val;
                *ptr++ = val;
            }
            else {
                *ptr++ = bg;
                *ptr++ = bg;
                *ptr++ = bg;
            }
            x += xinc;
        }
        y += yinc;
    }
}

static void generate_julia_rgb(unsigned char *buffer, size_t width, size_t height)
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
            int val;
            bool in_set = in_julia_color(x, y, num_iters, cx, cy, &val);
            if (in_set) {
                *ptr++ = 128;
                *ptr++ = val;
                *ptr++ = 255 - val;
            }
            else {
                *ptr++ = 128;
                *ptr++ = 128;
                *ptr++ = 128;
            }
            x += xinc;
        }
        y += yinc;
    }
}

static void generate_julia_hsv(unsigned char *buffer, size_t width, size_t height)
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
            double val;
            bool in_set = in_julia_hue(x, y, num_iters, cx, cy, &val);
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
            x += xinc;
        }
        y += yinc;
    }
}

int main(int argc, char *argv[])
{
    int color;
    char file_name[30];

    size_t width = pixels;
    size_t height = pixels;

    unsigned char *buffer = calloc(1, 3 * width * height);

    if (argc != 2) {
        printf("Usage:\n %s <color(0 - gray / 1 - rgb / 2 - hsv / 4 - binary / 5 - gray with bg)>\n", argv[0]);
        return -1;
    }

    color = atoi(argv[1]);

    sprintf(file_name, "julia_%05d_%05d_%d.png", (int)(cx * 1000), (int)(cy * 1000), color);

    switch (color) {
    case 0:
        generate_julia_gray(buffer, width, height);
        break;
    case 1:
        generate_julia_rgb(buffer, width, height);
        break;
    case 2:
        generate_julia_hsv(buffer, width, height);
        break;
    case 4:
        generate_julia_plain(buffer, width, height);
        break;
    case 5:
        generate_julia_gray_bg(buffer, width, height);
        break;
    default:
        generate_julia_gray(buffer, width, height);
        break;
    }

    write_png_color(buffer, width, height, file_name);

    printf("File %s written\n", file_name);

    free(buffer);

    return 0;
}
