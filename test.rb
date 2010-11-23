$LOAD_PATH << '.' # because Ruby 1.9 is dumb 

require 'rbdraw'

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

puts "Clearing screen"
g.clear
print '>'
gets

#faster drawing
g.sync_on_draw false

puts "Drawing a pretty diamond thing"
(0..w.height).select {|y| y % 10 == 0 }.each {|y|
    g.color rand(255), rand(255), rand(255)
    g.line 0, w.height / 2, w.width, y
    g.line w.width, w.height / 2, 0, y
    g.line w.width / 2, 0, y, w.height
    g.line w.width / 2, w.height, y, 0
}

g.sync

print ">"
gets

g.clear

g.color 0, 0,0

dm = 1
while dm < w.width
  g.buffer {|b|
    b.color 0, 0, 0
    b.text "it's alive!", 10, 20
    b.color rand(255), rand(255), rand(255)
    b.circle w.width / 2, w.height / 2, dm
    dm += (2 + dm * 0.02).to_i
    sleep(0.1)
  }
end

g.sync

print '>'
gets

w.hide
d.close

