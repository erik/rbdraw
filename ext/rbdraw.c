#include <stdio.h>

#include "rbdraw.h"

void Init_rbdraw() {
  /*
   * Module containing all of the classes and functions necessary
   * to create Displays, Windows and draw on the screen.
   *
   * Author:: Erik Price
   * Copyright:: Copyright (c) 2010 Erik Price
   * License:: Distributed under the MIT License
   */
  VALUE mDraw = rb_define_module("Draw");
  /* Display */
    cDisplay = rb_define_class_under(mDraw, "Display", rb_cObject);
      rb_define_singleton_method(cDisplay, "new", display_new, -1);
      rb_define_method(cDisplay, "initialize", display_init, 4);
      rb_define_method(cDisplay, "close", display_close, 0);
        rb_define_attr(cDisplay, "name", 1, 0);
	rb_define_attr(cDisplay, "screen_num", 1, 0);
	rb_define_attr(cDisplay, "width", 1, 0);
	rb_define_attr(cDisplay, "height", 1, 0);
  /* Window */
    cWindow = rb_define_class_under(mDraw, "Window", rb_cObject);
      rb_define_singleton_method(cWindow, "new", window_new, -1);
      rb_define_method(cWindow, "initialize", window_init, 2);
      rb_define_method(cWindow, "show", window_show, 0);
      rb_define_method(cWindow, "hide", window_hide, 0);
      rb_define_method(cWindow, "graphics", create_graphics, 0);
        rb_define_attr(cWindow, "width", 1, 1);
	rb_define_attr(cWindow, "height", 1, 1);
    /* Graphics */
    cGraphics = rb_define_class_under(mDraw, "Graphics", rb_cObject);
      rb_define_singleton_method(cGraphics, "new", graphics_new, -1);
      rb_define_method(cGraphics, "point", draw_point, 2);
      rb_define_method(cGraphics, "text", draw_text, 3);
      rb_define_method(cGraphics, "font", select_font, 1);
      rb_define_method(cGraphics, "sync_on_draw", sync_on_draw, 1);
      rb_define_method(cGraphics, "sync", graphics_sync, 0);

  XInitThreads();
}
