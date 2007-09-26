# we are here in 'Value' class. Superclass is Worker.

outlet :value => :float

def value
  @value || 0.0
end