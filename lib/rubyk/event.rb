class Event
  attr_accessor :time, :dest
  def initialize(*args)
    @time = args.shift
    @dest = args.shift
    @method = args.shift
    @args = args
  end
  
  def trigger!
    @dest.time = @time
    if @method == :bang
      @dest.update!(@time)
    else
      @dest.send(@method, *@args)
    end
  end
end