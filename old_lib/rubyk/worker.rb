require File.join(File.dirname(__FILE__), 'connection')
require File.join(File.dirname(__FILE__), 'event')

class Worker
  attr_accessor :time
  @@outlets = {}
  @@inlets  = {}
  @@queue   = []
  class << self
    def output_methods
      (@@outlets[self] || {}).keys
    end
    
    def inlet(hash)
      inlets = @@inlets[self] ||= {}
      inlets.merge!(hash)
      hash.each do |k,v|
        self.class_eval "
          def #{k}
            @_input_cache[#{k.to_sym.inspect}] ||= get_inlet_value(#{k.to_sym.inspect})
          end"
      end
    end
    
    def outlet(hash)
      outlets = @@outlets[self] ||= {}
      outlets.merge!(hash)
    end
    
    def add_to_queue(*args)
      time  = args[0]
      index = 0
      while(@@queue[index] && @@queue[index].time <= time) do
        index += 1
      end
      @@queue[index..index] = [Event.new(*args),@@queue[index]].compact
    end
    
    def remove_from_queue(obj)
      @@queue.reject! { |e| e.dest == obj}
    end
    
    def run_queue(time)
      while @@queue[0] && @@queue[0].time <= time
        event = @@queue.shift
        event.trigger!
      end
    end
  end
  
  def initialize
    @output = {}
    @_input_cache = {}
    @_out_connections = {}
    @_in_connections  = {}
  end
  
  def init
  end
  
  def destroy
  end
  
  def bang
    :bang
  end
  
  def update!(time)
    # Breadth first not working if some elements do not have any inputs nor callbacks (fixed values)
    # return unless inputs_current?(time) # only compute when all inputs are already updated
    @time = time
    @_input_cache = {}
    bang
    self.class.output_methods.each do |method|
      update_output(method, time)
    end
    propagate!(time)
  end
  
  def output
    @output || {}
  end
  
  def connect_outlet(out_name, conn)
    @_out_connections[out_name] ||= []
    @_out_connections[out_name] << conn
  end
  
  def connect_inlet(in_name, conn)
    @_in_connections[in_name] ||= []
    @_in_connections[in_name] << conn
  end
  
  def propagate!(time)
    @_out_connections ||= {}
    @_out_connections.each do |k,list|
      list.each do |c|
        c.propagate!(time)
      end
    end
  end
  
  private
    def inputs_current?(time)
      @_in_connections.each do |name, list|
        next if list.empty?
        ok = false
        list.each do |c|
          if c.time == time
            ok = true
            break # ok
          end
        end
        return false unless ok
      end
      true
    end
    
    def update_output(method, time)
      @output[method] = send(method)
    end
    
    def get_inlet_value(name)
      return nil unless inlets = @_in_connections[name]
      conn = nil
      inlets.each do |c|
        if c.time == @time
          conn = c
          break
        end
      end
      conn ? conn.value : nil
    end
    
end
