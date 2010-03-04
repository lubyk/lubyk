require 'rubygems'
require 'dub'
require 'dub/lua'
require 'pathname'

XML_DOC_PATH  = (Pathname(__FILE__).dirname + 'doc/xml/').expand_path
BINDINGS_PATH = (Pathname(__FILE__).dirname)

rk = Dub.parse(XML_DOC_PATH + 'namespacerk.xml')[:rk]
Dub::Lua.bind(rk)

# %w{Mat Size Point}.each do |class_name|
#   File.open(BINDINGS_PATH + "lua/cv_#{class_name}.cpp", 'wb') do |f|
#     klass = cv[class_name]
#     klass.header = 'opencv/cv.h'
#     klass.ignore %w{findHomography}
#     f.puts klass
#   end
# end
#

File.open(BINDINGS_PATH + "rk/lua_bindings.cpp", 'wb') do |f|
  f.puts rk
end
