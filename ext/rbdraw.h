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
  bool created;
} Window_t;


/* Graphics */
typedef struct {
  Display_t* disp;
  Window_t* win;
  XFontStruct* font;
  GC context;
  bool sync_on_draw;
} Graphics_t;

/* prototypes */
Display_t *DisplayNew(char* name);
void DisplayDispose(Display_t *rec);
VALUE display_init(VALUE, VALUE, VALUE, VALUE, VALUE);
VALUE display_new(int, VALUE*, VALUE);
VALUE display_close(VALUE);

Window_t *WindowNew(Display_t* disp);
void WindowDispose(Window_t* win);
VALUE window_new(int, VALUE*, VALUE);
VALUE window_init(VALUE, VALUE, VALUE);
VALUE window_show(VALUE);
VALUE window_hide(VALUE);
VALUE create_graphics(VALUE);

Graphics_t *GraphicsNew(Window_t *win);
void GraphicsDispose(Graphics_t *win);
VALUE graphics_new(int, VALUE*, VALUE);
VALUE draw_point(VALUE, VALUE, VALUE);
VALUE draw_text(VALUE, VALUE, VALUE, VALUE);
VALUE select_font(VALUE, VALUE);
VALUE sync_on_draw(VALUE, VALUE);
VALUE graphics_sync(VALUE);
#endif /* _RBDRAW_H_ */

