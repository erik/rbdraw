#include "rbdraw.h"

Graphics_t *GraphicsNew(Window_t *win) {
  Graphics_t *g = malloc(sizeof(Graphics_t));
  
  g->disp = win->disp;
  g->win = win;
  
  XGCValues values;
  unsigned long valuemask = 0;

  Display *d = win->disp->display;
  Window w = win->w;

  GC gc = XCreateGC(d, w, valuemask, &values);
  XSetFillStyle(d, gc, FillSolid);
  XSetLineAttributes(d, gc, 2, LineSolid, CapRound, JoinRound);
  XSetBackground(d, gc, BlackPixel(d, win->disp->screen_num));
  
  XSync(d, False);

  if(gc < 0) {
    rb_raise(rb_eRuntimeError, "error on creating graphics");
    return NULL;
  }

  g->context = gc;
  
  return g;
}

void GraphicsDispose(Graphics_t *g) {
  DisplayDispose(g->disp);
  WindowDispose(g->win);
  g = NULL;
}

VALUE graphics_new(int argc, VALUE* argv, VALUE self) {
  rb_raise(rb_eRuntimeError, "can't call new for Graphics class");
  return Qnil;
}

// TODO: allow calling without XSync (makes everything slow)
VALUE draw_point(VALUE self, VALUE r, VALUE c) {
  int x = FIX2INT(r);
  int y = FIX2INT(c);
  Graphics_t *g;
  Data_Get_Struct(self, Graphics_t, g);
  
  XDrawPoint(g->disp->display, g->win->w, g->context, x, y);  
  XSync(g->disp->display, False);
  
  return Qnil;
}

VALUE draw_text(VALUE self, VALUE string, VALUE r, VALUE c) {
  int x = FIX2INT(r);
  int y = FIX2INT(c);
  char* str = RSTRING(string)->ptr;
  
  Graphics_t *g;
  Data_Get_Struct(self, Graphics_t, g);

  XDrawString(g->disp->display, g->win->w,g->context, x, y, str, strlen(str));
  XSync(g->disp->display, False);
  
  return Qnil;
}

VALUE select_font(VALUE self, VALUE font_name) {
  char *fontstr = RSTRING(font_name)->ptr;
  Graphics_t* g;

  Data_Get_Struct(self, Graphics_t, g);

  XFontStruct* font_info = XLoadQueryFont(g->disp->display, fontstr);
  if(!font_info) {
    rb_raise(rb_eRuntimeError, "Couldn't load font: %s", fontstr);
    return Qnil;
  }
  XSetFont(g->disp->display, g->context, font_info->fid);
  g->font = font_info;

  return Qnil;
}

