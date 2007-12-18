old  = 0
wait = 0
token = {}
token[46] = 0
token[49] = 1
token[50] = 2
token[51] = 3
token[52] = 4
token[53] = 5
token[54] = 6
token[55] = 7
token[56] = 8
token[57] = 9
token[48] = 10
token[114] = 11
token[119] = 12

t = {}
t[0] = -0.8
t[1] = 1.5
t[2] = 1.8
t[3] = 1.5
t[4] = 1.5
t[5] = 1.2
t[6] = 1.0
t[7] = 1.0
t[8]  = 1.5
t[9]  = 1.5
t[10] = 1.5
t[11] = 2.5
t[12] = 4.5


n = {}
n[0]  = 0
n[1]  = 52
n[2]  = 54
n[3]  = 55
n[4]  = 57
n[5]  = 59
n[6]  = 60
n[7]  = 62
n[8]  = 64
n[9]  = 66
n[10] = 67
n[11] = 69
n[12] = 71


function bang()
  tok = token[in1]
  if ((tok ~= old or wait > 100) and in2 < t[tok]) then
  print(tok, in2)
    wait = 0
    old = tok
    send(1, n[tok])
  else
    wait = wait + 1
  end
end