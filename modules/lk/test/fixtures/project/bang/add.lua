outlet 'sum'

val1 = val1 or 0
val2 = val2 or 0

function inlet.val1(v)
  val1 = v
  sum(val1 + val2)
end

function inlet.val2(v)
  val2 = v
  sum(val1 + val2)
end
