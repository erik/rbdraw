# this draws some pretty lines

require 'rbdraw'
include Draw

win = Window.new
win.width = 150
win.height = 150

win.show

red = win.graphics
green = win.graphics
blue = win.graphics

red.color 255, 0, 0
green.color 0, 255, 0
blue.color 0, 0, 255

red.line 0, 0, 150, 150
green.line 0, 150, 150, 0
blue.line 0, 75, 150, 75

gets
