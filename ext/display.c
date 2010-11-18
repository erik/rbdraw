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

  disp_t->display_name = name == NULL ? "default" : name;
  disp_t->display = display;
  disp_t->screen_num = screen_num;

  return disp_t;
  
}

void DisplayDispose(Display_t* disp) {
  XFree(disp->display);
  free(disp);
  disp = NULL;
}

VALUE display_new(int argc, VALUE *args, VALUE self) {
  char *name = argc == 1 ? RSTRING(args[0])->ptr : NULL;
  Display_t* disp = DisplayNew(name);
  VALUE tdata = Data_Wrap_Struct(self, 0, DisplayDispose, disp);

  char *dname = name != NULL ? name : "default";
  
  VALUE argv[2];
  argv[0] = rb_str_new2(dname);
  argv[1] = INT2FIX(disp->screen_num);

  rb_obj_call_init(tdata, 2, argv);
  return tdata;
}

/* init vars and such */
VALUE display_init(VALUE self, VALUE name, VALUE num) {
  rb_iv_set(self, "@name", name);
  rb_iv_set(self, "@screen_num", num);
  return self;
}
