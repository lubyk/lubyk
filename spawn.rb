=begin
----------------------------------------------------------

  Prepare skeleton for new modules
  --------------------------------



----------------------------------------------------------
=end
require 'yaml'
require 'pathname'
require 'fileutils'

if ARGV.size < 1
  puts "Usage: ruby spawn.rb MODULE_NAME [--vendor] [--core]"
  exit(0)
else
  name         = ARGV[0]
  needs_vendor = ARGV.include?('--vendor')
  needs_core   = ARGV.include?('--core')
end

#---------------------------------------------------------
#                                 name.lua               -
#---------------------------------------------------------
name_lua = <<-END_TXT
--[[------------------------------------------------------

  #{name}
  #{'-' * name.length}

  FIXME: description for module '#{name}'

--]]------------------------------------------------------
#{name} = Autoload('#{name}')
END_TXT

name_lua += "require '#{name}.vendor'\n" if needs_vendor
name_lua += "require '#{name}.core'\n"   if needs_core

#---------------------------------------------------------
#                                 name_test.lua          -
#---------------------------------------------------------
name_test_lua = <<-END_TXT
--[[------------------------------------------------------

  #{name} test
  #{'-' * name.length}-----

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('#{name}')

function should.auto_load()
  assert_true(#{name})
end

test.all()
END_TXT

#---------------------------------------------------------
#                                 folder hierarchy       -
#---------------------------------------------------------
definition =<<-END_TXT
modules:
  #{name}:
    include:
      #{name}:
    lua:
      #{name}.lua: #{name_lua.inspect}
      #{name}:
    sub:#{needs_core ? "\n      core:" : ''}#{needs_vendor ? "\n    vendor:" : ''}
    test:
      #{name}_test.lua: #{name_test_lua.inspect}
END_TXT

def build(path, definition)
  if definition.kind_of?(Hash)
    # build directory
    FileUtils::mkpath(path)
    definition.each do |k, v|
      build(path + k, v)
    end
  elsif definition.nil?
    # build empty directory
    FileUtils::mkpath(path)
  elsif definition.kind_of?(String)
    File.open(path, 'wb') do |f|
      f.puts definition
    end
  end
end

build(Pathname('modules'), YAML::load(definition)['modules'])
