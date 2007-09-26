module ConnectionMethods
  def bang
    :bang
  end
  
  def output
    @output || {}
  end
  
  def connect_outlet(out_name, conn)
    @_out_connections ||= {}
    @_out_connections[out_name] ||= []
    @_out_connections[out_name] << conn
  end
  
  def connect_inlet(in_name, conn)
    @_in_connections ||= {}
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
    bang
  end
end

class Connection
  attr_reader :time, :value
  class << self
    def connect(src_obj, src_method, dest_obj, dest_method)
      conn = new(src_obj, src_method, dest_obj, dest_method)
      src_obj.connect_outlet(src_method, conn)
      dest_obj.connect_inlet(dest_method, conn)
      conn
    end
  end
  
  def initialize(src_obj, src_method, dest_obj, dest_method)
    @src = src_obj
    @src_method = src_method
    @dest = dest_obj
    @dest_method = dest_method
    @time = nil
  end
  
  def propagate!(time)
    return if @time == time
    @value = @src.output[@src_method]
    @time  = time
    @dest.update!(time,self)
  end
  
end
