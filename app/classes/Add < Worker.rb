# we are here in 'Add' class. Superclass is Worker.

input  :value1 => :float, :value2 => :float
output :total  => :float

def total
  value1 + value2
end