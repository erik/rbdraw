#include "rbdraw.h"

Window_t *WindowNew(Display_t* disp) {
  Window_t* win = malloc(sizeof(Window_t));

  win->disp = disp;
  win->width = disp->width;
  win->height = disp->height;
  win->x_pos = disp->width / 2;
  win->y_pos = disp->height / 2;
  win->created = false;
  return win;
}

void WindowDispose(Window_t* win) {
  DisplayDispose(win->disp);
  win = NULL;
}
/*
 * call-seq: 
 *   new (display)    -> Window,
 *   new ()           -> Window
 *
 * Creates a new Window object. Can be called with 
 * either one or zero arguments. The argument is 
 * expected to be an object of the Display class.
 * If not provided, it will default to the default
 * Display.
 */
VALUE window_new(int argc, VALUE* argv, VALUE self) {
  Window_t* win;
  Display_t* d;
  switch(argc) {
    case 0:
      win = WindowNew(DisplayNew(getenv("DISPLAY")));
      break;
    case 1:
      Data_Get_Struct(argv[0], Display_t, d);
      win = WindowNew(d);
      break;
    default:
      rb_raise(rb_eRuntimeError, "wrong number of arguments, %d for 0 or 1", argc);
      return self;
  }
  VALUE tdata = Data_Wrap_Struct(self, 0, WindowDispose, win);

  VALUE args[2];
  args[0] = INT2FIX(800);
  args[1] = INT2FIX(600);

  rb_obj_call_init(tdata, 2, args);
  return tdata;
}
/* :nodoc: */
VALUE window_init(VALUE self, VALUE width, VALUE height) {
  rb_ivar_set(self, rb_intern("@width"), width);
  rb_ivar_set(self, rb_intern("@height"), height);
  return self;
}

/*
 * call-seq: show() -> nil
 *
 * Draws the window to the screen, raising an
 * error if the window's Display has already
 * been closed.
 */
VALUE window_show(VALUE self) {
  Window_t* win;
  Data_Get_Struct(self, Window_t, win);
  win->width = FIX2INT(rb_ivar_get(self, rb_intern("@width")));
  win->height = FIX2INT(rb_ivar_get(self, rb_intern("@height")));
  if(!win->disp->open) {
    rb_raise(rb_eRuntimeError, "display closed");
    return self;
  }
  
  Display_t* disp = win->disp;
  
  if(!win->created) {
    win->w = XCreateSimpleWindow(disp->display, RootWindow(disp->display, \
							   disp->screen_num),
				 win->x_pos, win->y_pos, win->width, win->height,
				 0, BlackPixel(disp->display, disp->screen_num),
				 WhitePixel(disp->display, disp->screen_num)); 
    win->created = true;
  }
  XMapWindow(disp->display, win->w);
  XFlush(disp->display);

  return Qnil;
}

/*
 * call-seq: hide() -> false or true
 * 
 * Hides the window. Doesn't close anything, just
 * removes it from the screen. Returns true if
 * removed from the screen and false if the 
 * window's Display has been closed
 */
VALUE window_hide(VALUE self) {
  Window_t *win;
  Data_Get_Struct(self, Window_t, win);
  if(!win->disp->open) {
    return Qfalse;
  }
  
  XUnmapWindow(win->disp->display, win->w);
  XFlush(win->disp->display);
  
  return Qtrue;
}

/*
 * call-seq: create_graphics() -> Graphics
 *
 * Creates a Graphics object for the Window, 
 * which can be used for drawing on the 
 * window
 */
VALUE create_graphics(VALUE self) {
  Window_t *win;
  Graphics_t* g;
  Data_Get_Struct(self, Window_t, win);
  
  g = GraphicsNew(win);
  return Data_Wrap_Struct(cGraphics, 0, GraphicsDispose, g);
}
