require File.join(File.dirname(__FILE__), 'test_help')
require File.join(File.dirname(__FILE__), '../lib/rubyk/rubyk_server')

class RubykServerTest < Test::Unit::TestCase
  def test_load_class
    assert ! RubykServer.const_defined?('TestLoadClass')
    server.send(:load_class, 'TestLoadClass < Array', 'def hello; "hello #{size} times"; end')
    assert RubykServer.const_defined?('TestLoadClass')
    t = RubykServer::TestLoadClass.new
    assert_kind_of Array, t
    t << 'a' << 'b'
    assert_equal "hello 2 times", t.hello
  end
  
  def test_update_class
    server(:dummy)
    assert RubykServer.const_defined?('Dummy')
    server.send(:load_class, 'Dummy < Object', 'attr_accessor :name; def hello; "hello #{@name}!"; end')
    t = RubykServer::Dummy.new
    t.name = "Gaspard"
    assert_equal "hello Gaspard!", t.hello
  end
  
  def test_load_instance
    server(:dummy)
    assert_nil server.instance_variable_get('@instance1')
    server.send(:load_class, 'Dummy < Object', 'attr_accessor :name; def hello; "hello #{@name}!"; end')
    server.send(:load_instance, '@instance1 - Dummy', '@name = "John"')
    assert t = server.instance_variable_get('@instance1')
    assert_kind_of RubykServer::Dummy, t
    assert_equal "hello John!", t.hello
  end
  
  def test_update
    server(:dummy)
    preserving_files(server.basepath) do
      filepath = File.join(server.basepath, 'instances', '@bob - String.rb')
      File.open(filepath, 'wb') do |f|
        f.puts "replace 'my name is Bob.'"
      end
      assert_nil server.instance_variable_get('@bob')
      server.update(filepath)
      bob = server.instance_variable_get('@bob')
      assert_kind_of String, bob
      assert_equal 'my name is Bob.', bob  
    end
  end
  
  private
    def server(appname=:empty)
      @server ||= RubykServer.new(File.join(File.dirname(__FILE__), 'fixtures',appname.to_s), '127.0.0.1', 99989)
    end
end