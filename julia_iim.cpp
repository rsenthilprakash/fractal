#include "png_utils.h"
#include <iostream>
#include <new>
#include <complex>
#include <stack>

/* Comparing Rendering Methods for Julia Sets, V.Drakopoulos */

struct IIMRec
{
    int label;
    std::complex<double> Z;
};

void generate_julia_iim(unsigned char *img_buffer, int width, int height, std::complex<double> &C)
{
    int x;
    int y;
    int max_depth = 15;
    std::stack<struct IIMRec> st;
    struct IIMRec root;
    struct IIMRec data;
    unsigned char *img_ptr;

    data.label = 0;
    data.Z = std::complex<double>(1, 0);

    st.push(data);

    while(!st.empty()) {
        data = st.top();
        st.pop();

        x = int((data.Z.real() + 1.5) * 640); // derive 1.5 and 640 from width
        y = int((data.Z.imag() + 1.5) * 640); // derive 1.5 and 640 from height

        img_ptr = img_buffer + (y * width * 3) + x;
        *img_ptr = 255;
        *(img_ptr + 1) = 255;
        *(img_ptr + 2) = 255;

        if (data.label < max_depth) {
            std::complex<double> Z1 = std::sqrt(data.Z - C);
            std::complex<double> Z2 = std::complex<double>(-1 * Z1.real(), -1 * Z1.imag());

            root.Z = Z1;
            root.label = data.label + 1;
            st.push(root);
            root.Z = Z2;
            st.push(root);
        }
    }
}

int main(int argc, char *argv[])
{
    int width = 1920;
    int height = 1920;

    char png_name[50];

    std::complex<double> C(-0.391, -0.587);

    unsigned char *img_buffer = new unsigned char[width * height * 3];

    memset(img_buffer, 0, width * height * 3);

    generate_julia_iim(img_buffer, width, height, C);

    if (argc == 2)
        snprintf(png_name, 50, "%s/julia_iim_test.png", argv[1]);
    else
        snprintf(png_name, 50, "julia_iim_test.png");


    write_png_color(img_buffer, width, height, png_name);

    delete[] img_buffer;
}
