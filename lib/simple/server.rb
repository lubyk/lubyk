require 'socket'
require 'cgi'
require 'base64'

class SimpleRequest
  attr_accessor :headers, :body, :method, :path
  def initialize(session)
    @session = session
    @headers = {}
    request = session.gets
    if request =~ /\A([A-Z]+) (\S+) (.*)\Z/
      @method = $1
      @path   = CGI.unescape($2)
    end
    while ((line = session.gets) != "\r\n")
      if line =~ /\A([a-zA-Z-]+): (.*)\r\n\Z/
        @headers[$1] = $2
      end
    end
    if (len = @headers['Content-Length']) && len.to_i > 0
      @body = session.read(len.to_i)
      #@body = CGI.unescape(.sub(/\Amessage=/,''))
    end
  end
  
  def [](key)
    @headers[key]
  end
end

class SimpleResponse
  attr_accessor :headers, :body, :status, :message
  CRLF = "\r\n"
  
  def initialize
    @headers = {}
    @body = ''
    @status = 200
    @message = 'OK'
  end
  
  def set_status(st,m)
    @status  = st
    @message = m
  end
  
  def []=(key,value)
    @headers[key] = value
  end
  
  def [](key)
    @headers[key]
  end
  
  def out(str)
    @body << str << "\n"
  end
  
  def to_s
    s = "HTTP/1.1 #{@status} #{@message}#{CRLF}" 
    @headers['Content-Length'] = @body ? @body.length : 0
    @headers.each do |key,value|
      s += "#{key}: #{value}\n"
    end
    s += CRLF
    s += @body || ''
    s
  end
end

class SimpleSession
  attr_reader :response, :request, :request_method
  def initialize(server, socket)
    @server   = server
    @socket   = socket
    @request  = SimpleRequest.new(socket)
    @response = SimpleResponse.new
    @request_method   = @request.method
  end
  
  def do_get
    response.body = "Get not implemented in DummyServer."
  end
  
  def before_do
    if $debug
      puts "\n" + request_method + ' ' + request.path.inspect
      request.headers.each do |k,v|
        printf "  %-15s : %s\n", k, v
      end
      puts "---------------"
      print request.body if request.body
      puts "---------------"
    end
    authenticate
  end
  
  def after_do
    if $debug
      puts "RESPONSE: #{response.status} #{response.message}"
      response.headers.each do |k,v|
        printf "  %-15s : %s\n", k, v
      end
      puts "==============="
      print response.body if response.body
      puts "==============="
    end
  end
  
  def writeout
    @socket.print @response
    if @request['Connection'] == "close"
      @socket.close
      return :close
    else
      @socket.flush
      return :keep
    end
  end
  
  def handle_request
    meth = request_method.downcase
    unless before_do == false
      method    = 'do_' + meth
      if self.class.method_defined?(method)
        self.send(method)
      else
        # puts "#{@request.method} not implemented"
        # error
      end
    end
  
    if @socket.kind_of?(TCPSocket)
      result = writeout
    else
      result = :close
    end
    after_do
    return result
  end
  
  private
    def authenticate
      return true unless @server.authenticator
      user, pass = get_auth_data
      
      if @server.authenticator[user] == pass
        true
      else
        response["WWW-Authenticate"] = "Basic realm=\"#{@server.realm}\""
        response.set_status(401, 'Unauthorized')
        false
      end
    end
    
    def get_auth_data
      if authdata = request.headers['Authorization']
        authdata = authdata.split
        if authdata[0] == 'Basic' 
          Base64.decode64(authdata[1]).split(':')[0..1] 
        else
          # other authentication not supported
          ['','']
        end
      else
        # no authentication
        ['','']
      end
    end
    
end

class SimpleServer
  attr_reader :realm, :authenticator
  
  def initialize(ip, port, opts={})
    @realm         = opts[:realm] || 'SimpleServer'
    @authenticator = opts[:authenticator]

    @ip    = ip
    @port  = port
  end
  
  def create_session(socket)
    SimpleSession.new(self, socket)
  end
  
  def run
    server = TCPServer.new(@ip,@port)
    while(true)
      socket  = server.accept
      # Thread.new...
      while(create_session(socket).handle_request != :close)
        ;
      end
    end
  end
end