# draws a colorful fractal tree

require 'rbdraw'
include Draw

Deg_to_rad = Math::PI / 180

$w = 800
$h = 600

win = Window.new
win.width  = $w
win.height = $h

win.show

@g = win.graphics

@g.sync_on_draw false


@g.text "Fractal tree", 20, 20

def drawTree x1, y1, angle, depth
  @g.color rand(255), rand(255), rand(255)
  return if depth.zero? 
  x2 = x1 + (Math::cos(angle * Deg_to_rad) * depth * 10.0).to_i
  y2 = y1 + (Math::sin(angle * Deg_to_rad) * depth * 10.0).to_i
  @g.line x1, y1, x2, y2
  drawTree(x2, y2, angle - 20, depth - 1)
  drawTree(x2, y2, angle + 20, depth - 1)
end

drawTree(400, 600, -90, 9)

@g.sync

gets
