local i, o, p = lubyk.i, lubyk.o, lubyk.p

o.sum = {}

p {
  val1 = 0,
  val2 = 0,
}

function i.val1(v)
  -- notify
  p.val1 = v
  o.sum(val1 + val2)
end

function i.val2(v)
  -- notify
  p.val2 = v
  o.sum(val1 + val2)
end
