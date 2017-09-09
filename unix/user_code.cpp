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
// LEFT, RIGHT, UP, DOWN, JUMP
static int codes[] = {113, 114, 0, 0, 54};
static int snapshot_rate = 0;

void S9xRunUserDefinedExtraCode()
{
    loop_state = !loop_state;

    FILE *action_f = fopen("/tmp/action.txt", "r");
    if (action_f) {
        char buff[10];
        fgets(buff, 10, action_f);
        fclose(action_f);

	for (int i = 0; i < 5; ++i) {
            if (buff[i] == '1') {
                S9xReportButton(codes[i], loop_state);
            }
        }
    }

    // A very ugly way to get the current image. Will only work when dealing with
    // x11 based unix compilations...
    //
    snapshot_rate++;
    std::cout << snapshot_rate << "\n";
    if (snapshot_rate > 5) {
    	Image* image = (Image*)HACK_S9xGetCurrentImage();
    	int bytes_per_pixel = image->bits_per_pixel / 8;
    	int width = image->bytes_per_line / bytes_per_pixel;
    	int height = image->height;

    	FILE *f = fopen("/tmp/snapshot.ppm", "wb");
    	fprintf(f, "P6\n%i %i 255\n", width, height);
    	for (int y = 0; y < height; ++y) {
    	    for (int x = 0; x < width; ++x) {
    	        int line_index = y * width * bytes_per_pixel;
    	        int column_index = x * bytes_per_pixel;
    	        int index = line_index + column_index;

    	        char red = image->data[index + 2];
    	        char green = image->data[index + 1];
    	        char blue = image->data[index + 0];

    	        fputc(red, f);
    	        fputc(green, f);
    	        fputc(blue, f);
    	    }
    	}
    	fclose(f);
        snapshot_rate = 0;
    }
}
