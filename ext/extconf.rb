require 'mkmf'

CONFIG['CC'] = 'clang'

extension_name = 'rbdraw'
dir_config(extension_name)

have_library('X11')

create_makefile(extension_name)

