#!/usr/bin/env ruby

def test_from_file(path)
  path = path.split('/')
  name = path[-1].gsub('.lua', '')
  path[-1] = name.gsub(/(.)([A-Z])/,'\1_\2'.downcase) +  '_test.lua'
  path[-2] = nil
  path[-3] = 'test'
  return name, path.compact.join('/')
end
#puts test_from_file('/Users/gaspard/git/lubyk/modules/editor/lua/editor/NodeView.lua')
name, file_path = test_from_file(ENV['TM_FILEPATH'])

if true
  # html output
  puts %Q{<html><head><title>#{name} test</title>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <body>
    <pre>#{`lua #{file_path.inspect}`}</pre>
  </body>
  </html>}
else
  puts `lua #{file_path.inspect}`
end