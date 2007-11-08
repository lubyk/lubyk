g = g or 0
g = g + 1
n = 20
list = {60,72,71,71, n,n,60,n, 60,n,60,n, n,72,60,n}
return list[1 + g % #list]