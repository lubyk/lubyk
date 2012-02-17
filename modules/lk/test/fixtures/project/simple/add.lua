outlet 'sum'

defaults {
  val1 = 0,
  val2 = 0,
}

function inlet.val1(v)
  val1 = v
  sum(val1 + val2)
end

function inlet.val2(v)
  val2 = v
  sum(val1 + val2)
end
