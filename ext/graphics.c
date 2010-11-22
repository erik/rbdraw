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
  XSetBackground(d, gc, WhitePixel(d, win->disp->screen_num));

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
 *    g = some_window.graphics
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
 * call-seq: line(bx, by, ex, ey) -> nil
 *
 * Creates a line from the point (bx, by) to (ex, ey)
 *
 *    g = some_window.graphics
 *    # draw a line from the top left to bottom right
 *    g.line(0,0, some_window.width, some_window.height
 */
VALUE draw_line(VALUE self, VALUE begx, VALUE begy, VALUE endx, VALUE endy) {
  int bx, by, ex, ey;
  bx = FIX2INT(begx);
  by = FIX2INT(begy);
  ex = FIX2INT(endx);
  ey = FIX2INT(endy);

  Graphics_t *g;
  Data_Get_Struct(self, Graphics_t, g);

  XDrawLine(g->disp->display, g->win->w, g->context, bx, by, ex, ey);
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
 *   g = some_window.graphics
 *   # draw 'hello world' in the top left corner
 *   g.text 'hello world', 0, 0
 */
VALUE draw_text(VALUE self,VALUE string, VALUE r, VALUE c)
{
  int x = FIX2INT(r);
  int y = FIX2INT(c);
  char* str = RSTRING_PTR(string);

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
 *    g = some_window.graphics
 *    # use a specific font
 *    g.font 'lucidasans-14'
 *    # use a wildcard font
 *    g.font '*-helvetica-*-12-*'
 */
VALUE select_font(VALUE self, VALUE font_name) {
  char *fontstr = RSTRING_PTR(font_name);
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
 *    g = some_window.graphics
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

/*
 * call-seq: color(r, g, b) -> RGB value
 *
 * Sets the RGB color value for subsequent drawing
 * functions. Returns a string containing the
 * compiled RGB value in the form #RRGGBB
 */
VALUE graphics_color(VALUE self, VALUE r, VALUE gr, VALUE b) {
  Graphics_t *g;
  Data_Get_Struct(self, Graphics_t, g);

  unsigned int red = FIX2INT(r);
  unsigned int green = FIX2INT(gr);
  unsigned int blue = FIX2INT(b);

#define OUTOFBOUNDS(color) if(color > 255 || color < 0) { \
    rb_raise(rb_eRuntimeError, "Invalid RGB value: %d, %d, %d", red, green, blue); }

  OUTOFBOUNDS(red);
  OUTOFBOUNDS(green);
  OUTOFBOUNDS(blue);

#undef OUTOFBOUNDS

  Colormap cmap;
  XColor c0, c1;
  cmap = DefaultColormap(g->disp->display, 0);

  char* colorfmt = malloc(20);
  sprintf(colorfmt, "#%02x%02x%02x", red, green, blue);

  XAllocNamedColor(g->disp->display, cmap, colorfmt, &c1, &c0);
  XSetForeground(g->disp->display, g->context, c1.pixel);

  return rb_str_new2(colorfmt);

}

/*
 * call-seq: clear() -> nil
 *
 * Clears the window, leaving just the background color
 */
VALUE graphics_clear(VALUE self) {
  Graphics_t *g;
  Data_Get_Struct(self, Graphics_t, g);

  XClearWindow(g->disp->display, g->win->w);
  XFlush(g->disp->display);
  return Qnil;
}

/*
 * call-seq: circle(x, y, diameter) -> nil
 *
 * Draws a circle with a center of (x, y) and
 * a diameter of diameter
 *
 *    g = some_window.graphics
 *    # draw a circle at 100, 100, with a diameter of 20
 *    g.circle(100, 100, 20)
 */
VALUE draw_circle(VALUE self, VALUE xpos, VALUE ypos, VALUE diam) {
  int x = FIX2INT(xpos);
  int y = FIX2INT(ypos);
  int d = FIX2INT(diam);

  Graphics_t *g;
  Data_Get_Struct(self, Graphics_t, g);

  XFillArc(g->disp->display, g->win->w, g->context,
	   x - (d / 2), y - (d / 2), d, d, 0, 360 * 64);

  if(g->sync_on_draw) {
    XSync(g->disp->display, False);
  }

  return Qnil;
}

/*
 * call-seq: buffer() { |obj| block } -> nil
 *
 * WARNING: BROKEN!
 * Expects a block parameter, performs draw
 * functions called in body on a separate
 * buffer which is drawn to the window
 * at the end of the block
 *
 *    g = some_window.graphics
 *    g.buffer {|buff|
 *      buff.point 10, 10
 *      # ...
 *    }
 */
VALUE buffer_do(VALUE self) {
  /* TODO: Fixme! */
  if(!rb_block_given_p()) {
    rb_raise(rb_eRuntimeError, "no block given");
    return self;
  }

  Graphics_t *g;
  Pixmap buffer;
  Data_Get_Struct(self, Graphics_t, g);

  Display *display = g->disp->display;
  int width = g->win->width;
  int height = g->win->height;
  Window win = g->win->w;

  buffer = XCreatePixmap(display, win, width, height,
			 DefaultDepth(display, g->disp->screen_num));

  Window_t *w = malloc(sizeof(Window_t));
  w->disp = g->disp;
  w->width = width;
  w->height = height;
  w->w = buffer;
  Graphics_t *buffg = GraphicsNew(w);

  rb_yield(Data_Wrap_Struct(cGraphics, 0, GraphicsDispose, buffg));

  XCopyArea(display, buffg->win->w, g->win->w, g->context,
   	    0, 0, width, height, 0, 0);
  XFreePixmap(display, buffer);

  XFlush(buffg->disp->display);
  return Qnil;
}

