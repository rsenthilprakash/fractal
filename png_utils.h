#ifndef PNG_UTILS_H_INCLUDED
#define PNG_UTILS_H_INCLUDED

#include <png.h>

#ifdef __cplusplus
extern "C" {
#endif

void write_png_color(unsigned char *img_data, int width, int height, const char *file_name);

#ifdef __cplusplus
}
#endif

#endif
