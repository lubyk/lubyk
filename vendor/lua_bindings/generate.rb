require 'rubygems'
require 'dub'
require 'dub/lua'
require 'pathname'

# TODO Use CMake to run Doxygen and generate

XML_DOC_PATH  = (Pathname(__FILE__).dirname + 'doc/xml/').expand_path
BINDINGS_PATH = (Pathname(__FILE__).dirname)


# ================================= ADDITIONS TO glu, gl, rk
namespaces = [:rk, :glu, :gl]

namespaces.map! do |ns|
  Dub::Lua.bind Dub.parse(XML_DOC_PATH + "namespace#{ns}.xml")[ns]
end

namespaces.each do |ns|
  path = BINDINGS_PATH + "#{ns.name}/bindings.cpp"
  Dir.mkdir(path.dirname) unless File.exist?(path.dirname)

  File.open(path, 'wb') do |f|
    f.puts ns
  end
end

# ================================= OSCIT BINDINGS
# ?

# ================================= ADDITIONS TO cv
# file is hand edited, do not blindly overwrite...
# Just move code for cv_rk_mat_at into lua_modules/cv/additions.cpp
# Same for cv_LoadImage into cv.cpp
# FIXME: find a way to *append* new definitions. (Put markers in file to insert new functions + declarations ?)

# FIXME: find a way to transform a method into a class method:
# cv = Dub::Namespace.new('cv')
# mat = Dub::Matrix.new(cv, 'Mat')
# mat.add_method('at', cv_additions[:rk_mat_at]) # first argument = this

cv_additions = Dub::Lua.bind Dub.parse(XML_DOC_PATH + "namespacecv.xml")[:cv]

path = BINDINGS_PATH + "cv/additions.cpp"
Dir.mkdir(path.dirname) unless File.exist?(path.dirname)

File.open(path, 'wb') do |f|
#  f.puts cv_additions
end