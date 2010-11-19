require 'mkmf'

CONFIG['CC'] = 'clang'
$CFLAGS << " -Wall" << " -std=c99"

extension_name = 'rbdraw'
dir_config(extension_name)

have_library('X11')

create_makefile(extension_name)

