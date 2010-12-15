require 'rubygems'
require 'dub'
require 'dub/lua'
require 'pathname'

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
  'rk' => {
    'Timer' => {
      :string_format => '%li',
      :string_args   => '(*userdata)->interval()',
      :lib_name      => 'Timer_core'
    },
  },
  'rubyk' => {
    'Worker' => {
      :string_format => '%f',
      :string_args   => '(*userdata)->now()',
    },
  },
  'mdns' => {
    'Browser' => {
      :string_format => '%s',
      :string_args   => '(*userdata)->service_type()',
      :lib_name      => 'Browser_core'
    },
    'Registration' => {
      :string_format => '%s',
      :string_args   => '(*userdata)->name()',
      :lib_name      => 'Registration_core'
    },
  },
}.each do |mod_name, classes|
  namespace = Dub.parse(XML_DOC_PATH + "namespace#{mod_name}.xml")[mod_name.to_sym]
  Dub::Lua.bind(namespace)
  # Dub::Lua.function_generator.template_path = (BINDINGS_PATH + 'lua_function.cpp.erb')

  # ==============================================================================
  # ================    Classes     ==============================================
  # ==============================================================================
  classes.each do |class_name, definitions|
    klass = namespace[class_name]
    # Set custom tostring formats
    klass.opts.merge!(definitions)
    klass.header = "#{mod_name}/#{class_name}.h"

    File.open(BINDINGS_PATH + "modules/#{mod_name}/sub/#{klass.lib_name}.cpp", 'wb') do |f|
      f.puts klass
    end
  end
end

