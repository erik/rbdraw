#include "rbdraw.h"
#include <ruby.h>

Display_t *DisplayNew(char *name) {
  Display_t *disp_t = malloc(sizeof(Display_t));
  Display *display;
  if( (display = XOpenDisplay(name)) == NULL) {
    rb_raise(rb_eRuntimeError, "could not connect to X server at %s", name);
    return NULL;
  }
  int screen_num = DefaultScreen(display);
  
  disp_t->width = DisplayWidth(display, screen_num);
  disp_t->height = DisplayHeight(display, screen_num);
  disp_t->display_name = name == NULL ? "default" : name;
  disp_t->display = display;
  disp_t->screen_num = screen_num;
  disp_t->open = true;

  return disp_t;  
}

void DisplayDispose(Display_t* disp) {
  // TODO: Segfault on XCloseDisplay here...
  /* if(disp) {
    if(disp->open) {
      XCloseDisplay(disp->display);
    }
    disp = NULL;
    }*/
}

/*
 * call-seq: new(name = getenv("DISPLAY")) -> Display
 *
 * Creates a new Display object. Can be called with
 * either one or no arguments to represent the name
 * of the X server to connect to. With no args, it
 * will connect to the default display
 */
VALUE display_new(int argc, VALUE *args, VALUE self) {
  char *name = argc == 1 ? RSTRING(args[0])->ptr : getenv("DISPLAY");
  Display_t* disp = DisplayNew(name);
  VALUE tdata = Data_Wrap_Struct(self, 0, DisplayDispose, disp);

  char *dname = name != NULL ? name : getenv("DISPLAY");
  
  VALUE argv[4];
  argv[0] = rb_str_new2(dname);
  argv[1] = INT2FIX(disp->screen_num);
  argv[2] = INT2FIX(disp->width);
  argv[3] = INT2FIX(disp->height);

  rb_obj_call_init(tdata, 4, argv);
  return tdata;
}

/* :nodoc: */
VALUE display_init(VALUE self, VALUE name, VALUE num, VALUE w, VALUE h) {
  rb_ivar_set(self, rb_intern("@name"), name);
  rb_ivar_set(self, rb_intern("@screen_num"), num);
  rb_ivar_set(self, rb_intern("@width"), w);
  rb_ivar_set(self, rb_intern("@height"), h);
  return self;
}

/*
 * call-seq: close() -> true
 *
 * Closes the display, preventing further usage
 * and ending the connection to the X server.
 * if the display is already closed, this will
 * raise an error
 */
VALUE display_close(VALUE self) {
  Display_t* disp;
  Data_Get_Struct(self, Display_t, disp);
  
  if(!disp->open) {
    rb_raise(rb_eRuntimeError, "display already closed");
    return Qnil;
  }
  
  XCloseDisplay(disp->display);
  disp->open = false;

  return Qtrue;

}
