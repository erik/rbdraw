require 'rbdraw.so'

d = Draw::Display.new
puts "#{d.name} #{d.screen_num} #{d.width}x#{d.height}"

w = Draw::Window.new

w.width = 400
w.height = 400

w.show

g = w.graphics

g.font "lucidasans-14"

g.text "hello world", w.width/2, w.height/2

(0..w.height).each {|y|
  (0..w.width). select {|x| x.even? }.each {|x|
    g.point x, y
  }
}

gets

w.hide
d.close
