function update(sig)
	g = g or 0
	g = g + 1
	n = 20
	list = {60,62,n,n, 63,n,67,62, 60,n,60,n, n,n,60,n}
	return list[1 + g % #list]
end