#include <ruby.h>
#include <stdio.h>

#include "rbdraw.h"

void Init_rbdraw() {
  VALUE mDraw = rb_define_module("Draw");
    cDisplay = rb_define_class_under(mDraw, "Display", rb_cObject);
      rb_define_singleton_method(cDisplay, "new", display_new, -1);
      rb_define_method(cDisplay, "initialize", display_init, 2);
    cWindow = rb_define_class_under(mDraw, "Window", rb_cObject);
    cGraphics = rb_define_class_under(mDraw, "Graphics", rb_cObject);
  XInitThreads();
}

/* Class Heirarchy --
    -- Draw:Module
        -- Display: Class
            -- new(name = 'null') -- XOpenDisplay
            -- self.default -- (new null)
            -- close -- XCloseDisplay
            -- ul black_pixel -- XBlackPixel
            -- ul white_pixel -- XWhitePixel
            -- int height -- DisplayHeight
            -- int width -- DisplayWidth
            -- Graphics create_graphics -- XDefaultGC
        -- Window: Class
            -- new (display = Display.default) -- XCreateSimpleWindow
            -- close -- XDestroyWindow

        -- Graphics: Class -- GC
            -- new() -- raise error
*/

