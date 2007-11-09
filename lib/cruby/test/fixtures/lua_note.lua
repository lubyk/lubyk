g = g or 0
g = g + 1
n = 20
list = {60,62,63,n, 72,74,75,n, 60,n,60,67, n,n,60,n}
return list[1 + g % #list]