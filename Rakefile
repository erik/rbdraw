require 'rake'

task :doc do
  puts "Building documentation."
  system 'cat ext/*.[ch] > doc.c && markdown README.md > README.rdoc && rdoc doc.c README.rdoc -m README.rdoc --title rbdraw && rm doc.c README.rdoc'
end

task :sloc do
  system 'sloccount ext'
end

task :make do 
  puts "Creating Makefile."
  system 'make clean'
  system 'ruby ext/extconf.rb'
end

task :build do
  system 'make'
end

task :rebuild do
  system 'make clean all'
end

task :default => :build
