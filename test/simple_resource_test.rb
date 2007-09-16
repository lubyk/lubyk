require File.join(File.dirname(__FILE__), 'test_help')
require File.join(File.dirname(__FILE__), '../lib/simple/resource')

class SimpleResourceTest < Test::Unit::TestCase
  
  def test_get
    assert_kind_of SimpleResource, SimpleResource.get(TEST_APP_DIRECTORY, '/', 0)
  end
  
  def test_get_no_file
    assert_nil SimpleResource.get(TEST_APP_DIRECTORY, '/not_found.txt', 0)
  end
  
  def test_create_resource
    preserving_files(TEST_APP_DIRECTORY) do
      base = TEST_APP_DIRECTORY
      ['simple.txt', 'lib/sublib/any/file.rb'].each do |path|
        fullpath = File.join(base, path)
        assert ! File.exist?(fullpath)
        assert_nil SimpleResource.get(base, path, 0)
        assert_kind_of SimpleResource, SimpleResource.get_or_new(base, path, 0)
        assert File.exist?(fullpath)
      end
    end
  end
  
  def test_delete_resource
    preserving_files(TEST_APP_DIRECTORY) do
      base = TEST_APP_DIRECTORY
      path = 'dummy.rb'
      fullpath = File.join(base, path)
      assert File.exist?(fullpath)
      assert_kind_of SimpleResource, s = SimpleResource.get(base, path, 0)
      assert s.delete!
      assert_nil SimpleResource.get(base, path, 0)
      assert ! File.exist?(fullpath)
    end
  end
  def test_children
    res = SimpleResource.get(TEST_APP_DIRECTORY, '/', 0)
    assert_equal [], res.children
    
    res = SimpleResource.get(TEST_APP_DIRECTORY, '/', 1)
    assert_equal ['dummy.rb', 'lib'], res.children.map{|r| r.name}
  end
  
  def test_all_children
    res = SimpleResource.get(TEST_APP_DIRECTORY, '/', 0)
    assert_equal [], res.children
    
    res = SimpleResource.get(TEST_APP_DIRECTORY, '/', 1)
    assert_equal ['dummy.rb', 'lib'], res.all_children.map{|r| r.name}
    
    res = SimpleResource.get(TEST_APP_DIRECTORY, '/', 2)
    assert_equal ['dummy.rb', 'lib', 'funky.rb'], res.all_children.map{|r| r.name}
  end
end