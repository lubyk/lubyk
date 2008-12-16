require 'osc'

class TestSatellite
  
  def initialize(host,inport,outport)
    @host = host
    @inport = inport
    @outport = outport
    
    @receiver = OSC::UDPServer.new
    @receiver.bind(@host, @inport)
    
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
    @sender.send( m, 0, @host, @outport)
  end
end

@sat = TestSatellite.new('localhost', 5000, 7000)

def send(url, *args)
  @sat.send(url, *args)
end
