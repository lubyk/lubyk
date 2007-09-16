require File.join(File.dirname(__FILE__), 'test_help')
require File.join(File.dirname(__FILE__), '../lib/simple/dav_server')
require 'stringio'
Struct.new("DummyDavServer", :realm, :authenticator)
class DavServerTest < Test::Unit::TestCase
  
  def test_do_get
    assert_equal File.read(TEST_APP_DIRECTORY + 'dummy.rb'), response_body('GET /dummy.rb')
  end
  
  def test_do_put
    preserving_files(TEST_APP_DIRECTORY) do
      do_query("PUT /dummy.rb", :body => "This is the end.\n")
      assert_equal File.read(TEST_APP_DIRECTORY + 'dummy.rb'), "This is the end.\n"
    end
  end
  
  def test_do_put_new
    preserving_files(TEST_APP_DIRECTORY) do
      assert ! File.exist?(TEST_APP_DIRECTORY + 'new.rb')
      do_query("PUT /new.rb", :body => "This is a new file.\n")
      assert_equal File.read(TEST_APP_DIRECTORY + 'new.rb'), "This is a new file.\n"
    end
  end
  
  def test_do_delete
    preserving_files(TEST_APP_DIRECTORY) do
      assert File.exist?(TEST_APP_DIRECTORY + 'dummy.rb')
      do_query("DELETE /dummy.rb", :body => "This is the end.\n")
      assert ! File.exist?(TEST_APP_DIRECTORY + 'dummy.rb')
    end
  end
  
  def test_propfind
    do_query("PROPFIND /", :body => "<?xml version='1.0' encoding='utf-8'?>
    <D:propfind xmlns:D='DAV:'>
    <D:prop>
    <D:getlastmodified/>
    <D:getcontentlength/>
    <D:resourcetype/>
    </D:prop>
    </D:propfind>", 'Depth' => 1)
    assert_match %r{response.*response.*href.*lib}m, @response.body
  end
  
  private
    def response_body(query, body=nil)
      do_query(query, :body=>body)
      @response.body
    end
    
    def do_query(query, opts={})
      body = opts.delete(:body)
      
      headers = ''
      opts.merge('Content-Length' => (body ? body.length : 0)).each do |k,v|
        headers << "#{k}: #{v}\r\n"
      end
      server    = Struct::DummyDavServer.new('TestRealm', nil)
      query_str = StringIO.new("#{query} HTTP/1.1\r\n#{headers}\r\n#{body ? body : ''}")
      @session  = DavSession.new(server, query_str, TEST_APP_DIRECTORY)
      @request  = @session.request
      @response = @session.response
      
      @session.handle_request
    end
end