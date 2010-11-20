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
  g->sync_on_draw = true;
  return g;
}

void GraphicsDispose(Graphics_t *g) {
  DisplayDispose(g->disp);
  WindowDispose(g->win);
  g = NULL;
}

/*
 * can't be called
 */
VALUE graphics_new(int argc, VALUE* argv, VALUE self) {
  rb_raise(rb_eRuntimeError, "can't call new for Graphics class");
  return Qnil;
}

/*
 * call-seq: point(x, y) -> nil
 *
 * Draws a single pixel at the specified point.
 * 
 *    g = some_window.create_graphics
 *    # draws a point at 10, 20
 *    g.point(10, 20)
 */
VALUE draw_point(VALUE self, VALUE r, VALUE c) {
  int x = FIX2INT(r);
  int y = FIX2INT(c);
  Graphics_t *g;
  Data_Get_Struct(self, Graphics_t, g);
  
  XDrawPoint(g->disp->display, g->win->w, g->context, x, y);  
  if(g->sync_on_draw) {
    XSync(g->disp->display, False);
  }
  
  return Qnil;
}

/*
 * call-seq: text(string, x, y)     -> nil
 *
 * Draws a string at the given location.
 *
 *   g = some_window.create_graphics
 *   # draw 'hello world' in the top left corner
 *   g.text 'hello world', 0, 0
 */
VALUE draw_text(VALUE self,VALUE string, VALUE r, VALUE c)
{
  int x = FIX2INT(r);
  int y = FIX2INT(c);
  char* str = RSTRING(string)->ptr;
  
  Graphics_t *g;
  Data_Get_Struct(self, Graphics_t, g);

  XDrawString(g->disp->display, g->win->w,g->context, x, y, str, strlen(str));
  if(g->sync_on_draw) {
    XSync(g->disp->display, False);
  }
  
  return Qnil;
}


/*
 * call-seq: font(string) -> nil 
 *
 * Sets the font of the Graphics object to the given
 * name. The font name may contain wildcards, 
 * specified by '*'
 *
 *    g = some_window.create_graphics
 *    # use a specific font
 *    g.font 'lucidasans-14'
 *    # use a wildcard font
 *    g.font '*-helvetica-*-12-*'
 */
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

/*
 * call-seq: sync_on_draw(bool) -> true or false
 *
 * Sets whether the display will be updated each
 * time it a draw function is called. If set to 
 * false, the window will have to manually be updated
 * with #sync. Allowd drawing to be completed
 * faster for many draw functions.
 *
 *    g = some_window.create_graphics
 *    # turn off
 *    g.sync_on_draw(false)
 *    # do some drawing ...
 *    #manually sync
 *    g.sync
 *    # turn back on
 *    g.sync_on_draw(true)
 */
VALUE sync_on_draw(VALUE self, VALUE b) {
  bool flag;
  switch(TYPE(b)) {
    case T_FALSE:
    case T_NIL:
      flag = false;
      break;
    default:
      flag = true;
      break;
  }

  Graphics_t* g;
  Data_Get_Struct(self, Graphics_t, g);
  
  g->sync_on_draw = flag;
  
  return flag? Qtrue : Qfalse;
    
}

/*
 * call-seq: sync() -> nil
 * 
 * Manually updates display, flushing any
 * changes to the screen. This needs to 
 * be used if #sync_on_draw is false
 */
VALUE graphics_sync(VALUE self) {
  Graphics_t* g;
  Data_Get_Struct(self, Graphics_t, g);
  
  XSync(g->disp->display, False);
  return Qnil;
}
