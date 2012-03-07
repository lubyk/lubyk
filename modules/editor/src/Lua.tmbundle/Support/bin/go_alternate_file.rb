#!/usr/bin/env ruby
require 'uri'

def open_url(url)
  `open "#{url}"`
end

def open(filepath, line_number = nil, column_number = nil)
  options = []
  options << "url=file://#{URI.escape(filepath)}"
  options << "line=#{line_number + 1}" if line_number
  options << "column=#{column_number + 1}" if column_number
  open_url "txmt://open?" + options.join("&")
end

def test_from_file(path)
  path = path.split('/')
  name = path[-1].gsub('.lua', '')
  path[-1] = name.gsub(/(.)([A-Z])/,'\1_\2').downcase +  '_test.lua'
  path[-2] = nil
  path[-3] = 'test'
  return name, path.compact.join('/')
end

def file_from_test(path)
  path = path.split('/')
  name = path[-1].gsub('_test.lua', '').capitalize
  name.gsub!(/_(.)/) {$1.upcase}
  path[-1] = name +  '.lua'
  path[-2] = "lua/#{path[-3]}"
  return name, path.join('/')
end

filepath = ENV['TM_FILEPATH']

if filepath =~ /_test.lua/
  name, file_path = file_from_test(filepath)
else
  name, file_path = test_from_file(filepath)
end

#open filepath
puts `mate #{file_path.inspect}`