#ifndef _RBDRAW_H_
#define _RBDRAW_H_

#include <X11/Xlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

#include <ruby.h>

#include <stdbool.h>

VALUE mDraw, cDisplay, cWindow, cGraphics;

/* Display */
typedef struct {
  Display *display;
  char * display_name;
  unsigned int width, height;
  int screen_num;
  bool open;
} Display_t;

/* Window */
typedef struct {
  Window w;
  Display_t* disp;
  unsigned int width, height;
  int x_pos, y_pos;
} Window_t;


/* Graphics */
typedef struct {
  GC context;
} Graphics_t;

/* prototypes */
Display_t *DisplayNew(char* name);
void DisplayDispose(Display_t *rec);
VALUE display_init(VALUE, VALUE, VALUE, VALUE, VALUE);
VALUE display_new(int, VALUE*, VALUE);

Window_t *WindowNew(Display_t* disp);
void WindowDispose(Window_t* win);
VALUE window_new(int, VALUE*, VALUE);
VALUE window_init(VALUE, VALUE, VALUE);
VALUE window_show(VALUE);

Graphics_t *GraphicsNew(Window_t win);
void GraphicsDispose(Window_t *win);
#endif /* _RBDRAW_H_ */

