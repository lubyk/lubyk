# we are in Rubyk context.
Connection.connect(@val1,  :value, @total, :value1)
Connection.connect(@val2,  :value, @total, :value2)

Connection.connect(@total, :total, @print, :print)

Connection.connect(self, :bang, @val1, :bang)
Connection.connect(self, :bang, @val2, :bang) # => needed or we wont have any output


Connection.connect(self, :bang, @play, :bang)