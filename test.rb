require 'rbdraw.so'

d = Draw::Display.new
puts "#{d.name} #{d.screen_num} #{d.width}x#{d.height}"

w = Draw::Window.new

w.width = 800
w.height = 800

w.show

loop do
  sleep(1)
end
