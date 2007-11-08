g = g or 0
g = g + 1
n = 20
list = {30,n,30,n, 85,n,30,n, 30,n,n,n, 65,n,65,n}

if (g % 2) == 0 then
	return list[1 + (g/2) % #list]
else
	return 20
end