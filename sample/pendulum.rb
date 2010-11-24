#draws a simple swinging pendulum

require 'rbdraw'
include Draw

$width = 450
$height = 300

win = Window.new

win.height = $height
win.width = $width

win.show

g = win.graphics

angle = Math::PI / 2
length = 200
angle_vel = 0
dt = 0.1

loop do
  angle_accel = -9.81 / length * Math::sin(angle)
  angle_vel += angle_accel * dt
  angle += angle_vel * dt
  g.buffer do |buff|
    buff.color 25, 0, 255
    buff.line 0, $height / 4, $width, $height /4
    buff.color 0, 0, 0
    anchor_x = $width / 2
    anchor_y = $height / 4
    bx = anchor_x + (Math::sin(angle) * length).to_i
    by = anchor_y + (Math::cos(angle) * length).to_i
    buff.line(anchor_x, anchor_y, bx, by)
    buff.circle(anchor_x, anchor_y , 7)
    buff.circle bx, by, 14
  end
  sleep(0.05)
end
  
