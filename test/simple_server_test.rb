require File.join(File.dirname(__FILE__), 'test_help')
require File.join(File.dirname(__FILE__), '../lib/simple/dav_server')
require 'stringio'
Struct.new("DummySimpleServer", :realm, :authenticator)

class SimpleServerTest < Test::Unit::TestCase
  
  def test_get_no_auth
    do_query "GET /dummy.rb"
    assert_equal 'Basic realm="TestRealm"', @response['WWW-Authenticate']
    assert_equal 401, @response.status
    
    do_query "GET /dummy.rb", 'Authorization' => "Basic #{Base64.encode64("gaspard:secret")}".chomp
    assert_equal 'Get not implemented in DummyServer.', @response.body
  end
  
  def test_encode_decode
    assert_equal ['gaspard', 'secret'], Base64.decode64("Basic #{Base64.encode64("gaspard:secret")}".chomp.split[1]).split(':')[0..1]
  end
  
  private
    
    def do_query(query, opts={})
      body = opts.delete(:body)
      headers = ''
      opts.merge('Content-Length' => (body ? body.length : 0)).each do |k,v|
        headers << "#{k}: #{v}\r\n"
      end
      server    = Struct::DummySimpleServer.new('TestRealm', Hash['gaspard', 'secret', 'joe', 'lazy'])
      query_str = StringIO.new("#{query} HTTP/1.1\r\n#{headers}\r\n#{body ? body : ''}")
      @session  = SimpleSession.new(server, query_str)
      @request  = @session.request
      @response = @session.response
      
      @session.handle_request
    end
end