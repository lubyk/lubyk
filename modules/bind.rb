require 'rubygems'
require 'dub'
require 'dub/lua'
require 'pathname'
require 'htmlentities'
require 'fileutils'

modules_to_bind = ARGV

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

# Dub::Lua.function_generator.custom_type(/QString/) do |type_def, arg, stack_pos|
#   # Does this work for return types ?
# end

Dub::Lua.function_generator.custom_type(/int\s+lua_func_idx/) do |type_def, arg, stack_pos|
%Q{
luaL_checktype(L, #{stack_pos}, LUA_TFUNCTION);
// push on top
lua_pushvalue(L, #{stack_pos});
#{type_def} = luaL_ref(L, LUA_REGISTRYINDEX);
}
end

# Special bindings file for mimas: we load everything in the call for mimas.core
mimas_declare = []
mimas_load = []

modules = {
  'dummy' => %w{Dummy},
  'lk' => {
    'class' => %w{Socket Mutex Timer Thread},
    'const' => true,
  },
  'lubyk' => %w{Worker},
  'mdns'  => %w{Browser Registration},
  'midi'  => %w{In Out},
  'mimas' => {
    'class' => %w{Application Brush Callback Color FileObserver GLWidget HBoxLayout Label LineEdit ListView Path Painter Pen PushButton Slider TableView VBoxLayout Widget},
    'const' => true,
  },
  'zmq'   => {
    'class' => %w{Socket},
    'const' => true,
  },
  'wii'   => %w{Browser Remote},
}.each do |mod_name, opts|
  next unless modules_to_bind.nil? || modules_to_bind.include?(mod_name)
  puts "Binding #{mod_name}"
  if !opts.kind_of?(Hash)
    opts = {'class' => opts}
  end

  namespace = Dub.parse(XML_DOC_PATH + "namespace#{mod_name}.xml")[mod_name.to_sym]
  if mod_name == 'mimas'
    # ignore setHue, variantFromLua
    namespace.ignore %w{setHue variantFromLua}
  end
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

    dir = BINDINGS_PATH + "modules/#{mod_name}/sub"
    if !File.exist?(dir)
      FileUtils::mkdir(dir)
    end

    File.open(dir + "#{klass.opts[:filename] || klass.lib_name}.cpp", 'wb') do |f|
      f.puts klass
    end

    if mod_name == 'mimas'
      mimas_declare << "int luaload_mimas_#{klass.lib_name}(lua_State *L);"
      mimas_load    << "luaload_mimas_#{klass.lib_name}(L);"
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

    if mod_name == 'mimas'
      mimas_declare << "int luaload_mimas_constants(lua_State *L);"
      mimas_load    << "luaload_mimas_constants(L);"
    end
  end
end

if modules_to_bind.nil? || modules_to_bind.include?('mimas')
  # Update the mimas/core/mimas.cpp file
  mimas_file = BINDINGS_PATH + "modules/mimas/sub/core/mimas.cpp"
  file = File.read(mimas_file)
  File.open(mimas_file, 'wb') do |f|
    file.sub!(%r{//\s*\[\[DECLARE.*?\]\]}m,
  %Q{// [[DECLARE
  #{mimas_declare.join("\n")}
  // ]]})

    file.sub!(%r{//\s*\[\[LOAD.*?\]\]}m,
  %Q{// [[LOAD
    #{mimas_load.join("\n  ")}
    // ]]})

    f.puts file
  end
end