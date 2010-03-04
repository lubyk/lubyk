require 'rubygems'
require 'dub'
require 'dub/lua'
require 'pathname'

XML_DOC_PATH  = (Pathname(__FILE__).dirname + 'doc/xml/').expand_path
BINDINGS_PATH = (Pathname(__FILE__).dirname)

namespaces = [:rk, :glu, :gl]

namespaces.map! do |ns|
  Dub::Lua.bind Dub.parse(XML_DOC_PATH + "namespace#{ns}.xml")[ns]
end

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
  namespaces.each do |ns|
    f.puts "\n\n"
    f.puts ns
  end
end
