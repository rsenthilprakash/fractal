#include "png_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define PNG_DEBUG 3

static void abort_(const char * s, ...)
{
    va_list args;
    va_start(args, s);
    vfprintf(stderr, s, args);
    fprintf(stderr, "\n");
    va_end(args);
    abort();
}

void write_png_color(unsigned char *img_data, int width, int height, const char *file_name)
{
    png_byte bit_depth = 8;
    png_byte color_type = PNG_COLOR_TYPE_RGB;

    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep * row_pointers;
    int i;

    FILE *fp = fopen(file_name, "wb");

    if (!fp)
        abort_("[write_png_file] File could not be opened for writing");


    /* initialize stuff */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
        abort_("[write_png_file] png_create_write_struct failed");

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        abort_("[write_png_file] png_create_info_struct failed");

    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during init_io");

    png_init_io(png_ptr, fp);


    /* write header */
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during writing header");

    png_set_IHDR(png_ptr, info_ptr, width, height,
                 bit_depth, color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);
    row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
    for (i = 0; i < height; i++)
        row_pointers[i] = img_data + (i * width * 3);

    /* write bytes */
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during writing bytes");

    png_write_image(png_ptr, row_pointers);

    /* end write */
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during end of write");

    png_write_end(png_ptr, NULL);

    free(row_pointers);

    fclose(fp);
}
