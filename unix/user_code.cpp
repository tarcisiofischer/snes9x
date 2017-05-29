#include "user_code.h"
#include "snes9x.h"
#include "controls.h"
#include "display.h"

#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>

// Yet another hack: Duplicated Image struct from x11.cpp file.
struct Image
{
  XImage* ximage;
  char *data;
  uint32 height;
  uint32 data_size;
  uint32 bits_per_pixel;
  uint32 bytes_per_line;
};


static bool loop_state = false;

void S9xRunUserDefinedExtraCode()
{
    // Keep pressing <enter> forever
    int ENTER_CODE = 36;
    S9xReportButton(ENTER_CODE, loop_state);
    loop_state = !loop_state;

    // A very ugly way to get the current image. Will only work when dealing with
    // x11 based unix compilations...
    //
    Image* image = (Image*)HACK_S9xGetCurrentImage();
    int bytes_per_pixel = image->bits_per_pixel / 8;
    int width = image->bytes_per_line / bytes_per_pixel;
    int height = image->height;

    std::cout << width << "x" << height << "\n";
    FILE *f = fopen("out.ppm", "wb");
    fprintf(f, "P6\n%i %i 255\n", width, height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int line_index = y * width * bytes_per_pixel;
            int column_index = x * bytes_per_pixel;
            int index = line_index + column_index;

            char red = image->data[index + 0];
            char green = image->data[index + 1];
            char blue = image->data[index + 2];

            fputc(red, f);
            fputc(green, f);
            fputc(blue, f);
        }
    }
    fclose(f);

}
