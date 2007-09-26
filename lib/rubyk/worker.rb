require File.join(File.dirname(__FILE__), 'connection')

class Worker
  @@outlets = {}
  @@inlets  = {}
  include ConnectionMethods
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
            @input_cache[#{k.to_sym.inspect}] ||= get_inlet_value(#{k.to_sym.inspect})
          end"
      end
    end
    
    def outlet(hash)
      outlets = @@outlets[self] ||= {}
      outlets.merge!(hash)
    end
  end

  def initialize
    @output = {}
    @input_cache = {}
  end
  
  def update!(time, conn)
    # Breadth first not working if some elements do not have any inputs nor callbacks (fixed values)
    return unless inputs_current?(time) # only compute when all inputs are already updated
    @time = time
    @input_cache = {}
    self.class.output_methods.each do |method|
      update_output(method, time)
    end
    propagate!(time)
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
      return unless @_out_connections[method] # do not compute output if not connected
      @output[method] = send(method)
    end
    
    def get_inlet_value(name)
      conn = nil
      @_in_connections[name].each do |c|
        if c.time == @time
          conn = c
          break
        end
      end
      conn ? conn.value : nil
    end
    
end
