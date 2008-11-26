require 'osc'

class TestSatellite
  
  def initialize(host,port)
    @host = host
    @port = port
    
    @receiver = OSC::UDPServer.new
    @receiver.bind(@host, @port)
    
    @receiver.add_method '/h*', nil do |msg|
      domain, port, host, ip = msg.source
      puts "#{msg.address} -> #{msg.args.inspect} from #{host}:#{port}"
    end
    
    Thread.new do
      @receiver.serve
    end
    
    @sender = OSC::UDPSocket.new
  end
  
  def send(url,*args)
    m = OSC::Message.new(url, nil, *args)
    @sender.send( m, 0, @host, @port)
  end
end

@sat = TestSatellite.new('localhost', 5000)
@sat.send('/hello/')

sleep 3
