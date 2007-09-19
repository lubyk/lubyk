# we are in Rubyk context.
@total.in[:value1] = @val1.out[:value]
@total.in[:value2] = @val2.out[:value]

@rubyk.in[:print] << @total.out[:total]