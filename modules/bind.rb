require 'rubygems'
require 'dub'
require 'dub/lua'
require 'pathname'
require 'htmlentities'
HtmlDecode = HTMLEntities.new

def get_dub_info(xml)
  (xml/'simplesect').each do |x|
    if (x/'title').inner_html == 'Bindings info:'
      (x/'title').remove()
      (x/'ref').each do |r|
        r.swap(r.inner_html)
      end
      code = HtmlDecode.decode((x/'para').inner_html)
      return eval(code)
    end
  end
  nil
end

XML_DOC_PATH  = (Pathname(__FILE__).dirname + '../build/doc/xml/').expand_path
BINDINGS_PATH = (Pathname(__FILE__).dirname + '..').expand_path

Dub.logger.level = Logger::WARN

Dub::Lua.function_generator.custom_type(/lua_State /) do |type_def, arg, stack_pos|
  if type_def =~ /lua_State\s*\*\s*L/
    ""
  else
    "#{type_def} = L;"
  end
end

Dub::Lua.function_generator.custom_type(/int\s+lua_func_idx/) do |type_def, arg, stack_pos|
%Q{
luaL_checktype(L, #{stack_pos}, LUA_TFUNCTION);
// push on top
lua_pushvalue(L, #{stack_pos});
#{type_def} = luaL_ref(L, LUA_REGISTRYINDEX);
lua_pop(L, 1);
}
end

{
  'rk'    => %w{Timer},
  'rubyk' => %w{Worker},
  'mdns'  => %w{Browser Registration},
  'zmq'   => %w{Send Receive}
}.each do |mod_name, classes|
  namespace = Dub.parse(XML_DOC_PATH + "namespace#{mod_name}.xml")[mod_name.to_sym]
  Dub::Lua.bind(namespace)
  # Dub::Lua.function_generator.template_path = (BINDINGS_PATH + 'lua_function.cpp.erb')

  # ==============================================================================
  # ================    Classes     ==============================================
  # ==============================================================================
  classes.each do |class_name, definitions|
    klass = namespace[class_name]
    if dub_info = get_dub_info(klass.xml)
      # Set custom tostring formats
      klass.opts.merge!(dub_info)
    end
    klass.header = "#{mod_name}/#{class_name}.h"

    File.open(BINDINGS_PATH + "modules/#{mod_name}/sub/#{klass.lib_name}.cpp", 'wb') do |f|
      puts klass.lib_name
      f.puts klass
    end
  end
end

