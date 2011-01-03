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
  'rk'    => %w{Socket Timer Thread},
  'rubyk' => %w{Worker},
  'mdns'  => %w{Browser Registration},
  'mimas' => %w{Application Callback HBoxLayout Label PushButton Slider VBoxLayout Widget},
  'zmq'   => {
    'class' => %w{Socket},
    'const' => true,
  }
}.each do |mod_name, opts|
  if !opts.kind_of?(Hash)
    opts = {'class' => opts}
  end

  namespace = Dub.parse(XML_DOC_PATH + "namespace#{mod_name}.xml")[mod_name.to_sym]
  Dub::Lua.bind(namespace)
  # Dub::Lua.function_generator.template_path = (BINDINGS_PATH + 'lua_function.cpp.erb')

  # ==============================================================================
  # ================    Classes     ==============================================
  # ==============================================================================
  (opts['class'] || {}).each do |class_name, definitions|
    klass = namespace[class_name]

    if not klass
      puts "Could not find xml for #{class_name}"
      next
    end

    if dub_info = get_dub_info(klass.xml)
      # Set custom tostring formats and ignore methods
      klass.opts.merge!(dub_info)
    end
    klass.header = "#{mod_name}/#{class_name}.h"

    File.open(BINDINGS_PATH + "modules/#{mod_name}/sub/#{klass.lib_name}.cpp", 'wb') do |f|
      f.puts klass
    end
  end

  if opts['const']
    (opts['groups'] || {}).each do |name, filename|
      group = Dub.parse(XML_DOC_PATH + "group___#{filename}.xml")[name]
      Dub::Lua.bind(group)
      namespace.merge!(group)
    end
    namespace.header = "#{mod_name}/#{namespace.header}"
    namespace.lib_name = "#{mod_name}_constants"
    File.open(BINDINGS_PATH + "modules/#{mod_name}/sub/constants.cpp", 'wb') do |f|
      f.puts namespace
    end
  end
end
