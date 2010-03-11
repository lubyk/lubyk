require 'rubygems'
require 'dub'
require 'dub/lua'
require 'pathname'

XML_DOC_PATH  = (Pathname(__FILE__).dirname + '../doc/xml/').expand_path
BINDINGS_PATH = (Pathname(__FILE__).dirname)


# ================================= ADDITIONS TO cv, glu, gl
namespaces = [:rk, :glu, :gl]

namespaces.map! do |ns|
  Dub::Lua.bind Dub.parse(XML_DOC_PATH + "namespace#{ns}.xml")[ns]
end

namespaces.each do |ns|
  File.open(BINDINGS_PATH + "#{ns.name}/additions.cpp", 'wb') do |f|
    f.puts ns
  end
end

# ================================= OSCIT BINDINGS


# file is hand edited, do not blindly overwrite...
# cv_additions = Dub::Lua.bind Dub.parse(XML_DOC_PATH + "namespacecv.xml")[:cv]
#
# File.open(BINDINGS_PATH + "cv/additions.cpp", 'wb') do |f|
#   f.puts cv
# end