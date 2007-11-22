class Connection
  attr_reader :time, :value
  class << self
    def connect(src_obj, src_method, dest_obj, dest_method = nil)
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
    @dest.update!(time)
  end
  
end
