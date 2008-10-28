x = 200
y = 200
n = 0.0
s = 10
in3 = in3 or {200,200}

function bang(sig)
  n = n + math.pi / 40
  x = in3[1] + math.cos(n) * 10
  y = in3[2] + math.sin(n) * 10
end

function draw(sig)
  gl.Translate(x,sig[2]-y, 0)
  gl.Color( {1, 1, 0, 0.8} )
  gl.Rect(-s,-s,s,s)
end