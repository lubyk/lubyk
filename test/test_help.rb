require 'test/unit'
require 'fileutils'

TEST_APP_DIRECTORY = File.join(File.dirname(__FILE__), 'fixtures/app') + '/'

def preserving_files(aPath, &block)
  path = aPath[-1..-1] == '/' ? aPath[0..-2] : aPath
  if File.exist?(path)
    FileUtils::cp_r(path,"#{path}.bak")
    move_back = true
  else
    move_back = false
  end
  begin
    yield
  ensure
    FileUtils::rmtree(path)
    if move_back
      FileUtils::mv("#{path}.bak",path)
    end
  end
end
