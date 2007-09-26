# we are here in 'Add' class. Superclass is Worker.

inlet  :value1 => :float, :value2 => :float
outlet :total  => :float

def total
  value1 + value2
end