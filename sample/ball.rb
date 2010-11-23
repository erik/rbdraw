require 'rbdraw'
include Draw

class Ball
  @@diameter = 40
  @@gravity = -0.7
  @@xaccel = 0.0
  @@yaccel = 0.3

  attr_accessor :d, :x, :y, :xspeed, :yspeed, :life
  def initialize(x, y, speed)
    @x = x
    @y = y
    @xspeed = 0
    @yspeed = speed
    @d = @@diameter
    @life = 255
  end

  def alive?
    @life -= 2
    @life > 0
  end

  def move
    @xspeed += @@xaccel
    @yspeed += @@yaccel

    @x += @xspeed.to_i
    @y += @yspeed.to_i
  end

  def bounce
    @yspeed *= @@gravity
    @xspeed *= @@gravity
  end
end

$w = 300
$h = 300

$balls = []
10.times {|_|
  $balls << Ball.new(rand($w), rand(40) + 10, 0)
}

win = Window.new
win.width = $w
win.height = $h

win.show

g = win.graphics
#g.sync_on_draw false

loop do
  g.buffer {|buff|
    # about every 10 frames add a new ball
    if rand(10) == 2
      $balls << Ball.new(rand($w), rand(40) + 10, 0)
    end
    $balls = $balls.select {|ball|
      ball.alive?
    }
    $balls.collect {|ball|
      if ball.y > win.height - ball.d
        ball.bounce
        ball.move
      end
      l = (255 - ball.life).abs
      buff.color(l, l, l)
      ball.move
      buff.circle ball.x, ball.y, ball.d
      ball
    }
   # buff.sync
  }
  sleep(0.05)
end

win.hide

