old  = 0
wait = 0
t = {}
t[46] = -0.8
t[49] = 0.8
t[50] = 0.8
t[51] = 0.8
t[52] = 0.8

n = {}
n[46] = 0
n[49] = 49
n[50] = 50
n[51] = 48
n[52] = 47
function bang()
  --print(in1, in2)
  if ((in1 ~= old or wait > 100) and in2 < t[in1]) then
    wait = 0
    old = in1
    send(1, n[in1])
  else
    wait = wait + 1
  end
end