#include "user_code.h"
#include "snes9x.h"
#include "controls.h"

#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>

static bool loop_state = false;

void S9xRunUserDefinedExtraCode()
{
    // Keep pressing <enter> forever
    int ENTER_CODE = 36;
    S9xReportButton(ENTER_CODE, loop_state);
    loop_state = !loop_state;
}
