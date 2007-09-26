# we are here in 'Add' class. Superclass is Worker.

inlet  :tempo => :float
outlet :out_bang  => :bang

def init
  @tempo = 60
end

def bang
  # restart metro
  Worker.remove_from_queue(self)
end

def out_bang
  Worker.add_to_queue(@time + (60.0 / @tempo), self, :bang)
  return :bang
end