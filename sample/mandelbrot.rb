# awesome (but insanely slow) Mandelbrot set drawing
# feel free to mess around with various variables to
# find a better / faster solution

require 'rbdraw'
include Draw

# higher resolutions are *much* nicer, but 400, 300
# should complete in ~30 seconds
$w = 400
$h = 300

w = Window.new

w.height = $h
w.width = $w

w.show

g = w.graphics

MAX_ITER = 255

g.sync_on_draw false

# this strategy was shamelessly stolen from RosettaCode
# http://rosettacode.org/wiki/Mandelbrot_set#haXe

sx = 3.0 / ($w - 1)
sy = 2.0 / ($h - 1)

(0...$h).each {|y|
  ci = y * sy - 1.0
  (0...$w).each {|x|
    k = 0
    zr = 0
    zi = 0
    cr = x * sx - 2.0
    
    while k <= MAX_ITER and zr * zr + zi * zi <= 4
      temp = zr * zr - zi *zi + cr
      zi = 2 * zr * zi + ci
      zr = temp
      k += 1
    end
    
    g.color k > MAX_ITER ? 0 : k * 0x010
    g.point(x, y)
      
  }
  # Sync after every line, makes drawing less choppy
  g.sync
}
g.sync

gets
      
