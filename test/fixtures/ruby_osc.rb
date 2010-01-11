require 'osc'


# debug
class DebugServer < OSC::UDPSocket
  include OSC::Server
  def serve
   $stdout.send(:puts,"serving")
    loop do
      p, sender = recvfrom(MAX_MSG_SIZE)
       $stdout.send(:puts,"IN")
      debug_dispatch p
    end
  end

  # send msg2 as a reply to msg1
  def reply(msg1, msg2)
    domain, port, host, ip = msg2.source
    send(msg2, 0, host, port)
  end
  
  def debug_dispatch(mesg)
   puts "YO"
   case mesg
   when Bundle, Message
   else
     mesg = Packet.decode(mesg)
   end

   domain, port, host, ip = msg.source
   $stdout.send(:puts,"-> \#{msg.address} \#{msg.args.inspect} from \#{host}:\#{port}")
  end
  
  def dispatch_bundle(mesg)
   case mesg
   when Bundle, Message
   else
     mesg = Packet.decode(mesg)
   end

   domain, port, host, ip = msg.source
    $stdout.send(:puts,"-> \#{msg.address} \#{msg.args.inspect} from \#{host}:\#{port}")
  end
end

class TestSatellite
  
  def initialize(host,inport,outport)
    @host = host
    @inport = inport
    @outport = outport
    
    @receiver = DebugServer.new
    @receiver.bind(@host, @inport)
    
    @receiver.add_method '/n', nil do |msg|
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
