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
  /* 
   * Document-class: Draw::Display
   *
   * This class handles the connection to the X server.
   * Chances are you won't need to touch this at all, 
   * since Draw::Window.new can be called without a 
   * Display parameter, and will default to the one you'll
   * most likely want to use anyway.
   */
    cDisplay = rb_define_class_under(mDraw, "Display", rb_cObject);
      rb_define_singleton_method(cDisplay, "new", display_new, -1);
      rb_define_method(cDisplay, "initialize", display_init, 4);
      rb_define_method(cDisplay, "close", display_close, 0);
        rb_define_attr(cDisplay, "name", 1, 0);
	rb_define_attr(cDisplay, "screen_num", 1, 0);
	rb_define_attr(cDisplay, "width", 1, 0);
	rb_define_attr(cDisplay, "height", 1, 0);
  /* 
   * Document-class: Draw::Window
   *
   * This class handles the actual, visual window that
   * appears on the screen. There isn't really all
   * that much to do here, besides create the graphics
   * and resize the window
   */
    cWindow = rb_define_class_under(mDraw, "Window", rb_cObject);
      rb_define_singleton_method(cWindow, "new", window_new, -1);
      rb_define_method(cWindow, "initialize", window_init, 2);
      rb_define_method(cWindow, "show", window_show, 0);
      rb_define_method(cWindow, "hide", window_hide, 0);
      rb_define_method(cWindow, "graphics", create_graphics, 0);
        rb_define_attr(cWindow, "width", 1, 1);
	rb_define_attr(cWindow, "height", 1, 1);
    /* 
     * Document-class: Draw::Graphics
     *
     * This is the most important part of the library,
     * as it is what makes drawing on the window possible.
     * A Graphics object cannot be instantiated directly,
     * but must instead be created by a Draw::Window's 
     * #graphics method.
     */
    cGraphics = rb_define_class_under(mDraw, "Graphics", rb_cObject);
      rb_define_singleton_method(cGraphics, "new", graphics_new, -1);
      rb_define_method(cGraphics, "point", draw_point, 2);
      rb_define_method(cGraphics, "line", draw_line, 4);
      rb_define_method(cGraphics, "circle", draw_circle, 3);
      rb_define_method(cGraphics, "rect", draw_rect, 4);
      rb_define_method(cGraphics, "text", draw_text, 3);
      rb_define_method(cGraphics, "font", select_font, 1);
      rb_define_method(cGraphics, "sync_on_draw", sync_on_draw, 1);
      rb_define_method(cGraphics, "sync", graphics_sync, 0);
      rb_define_method(cGraphics, "color", graphics_color, -1);
      rb_define_method(cGraphics, "clear", graphics_clear, 0);
      rb_define_method(cGraphics, "buffer", buffer_do, 0);

  XInitThreads();
}
