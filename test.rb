require 'rbdraw.so'

d = Draw::Display.new
puts "#{d.name} #{d.screen_num} #{d.width}x#{d.height}"

w = Draw::Window.new

w.width = 400
w.height = 400

w.show

g = w.graphics

puts "Drawing 'hello world' to the screen in Lucidasans-14..."
g.font "lucidasans-14"
g.text "hello world", w.width/2, w.height/2
print ">"
gets

puts "Drawing 1000 random points"
1000.times {|_|
  x = rand w.width
  y = rand w.height
  g.point x, y
}
print ">"
gets

#faster drawing
g.sync_on_draw false

puts "Switching to color: #{ g.color 255, 0, 0}"

puts "Drawing some stripes"
(0..w.width). select {|x| x % 10 == 0 }.each {|x|
    g.line x, 0, x, w.height
}

g.sync

print ">"
gets

w.hide
d.close
