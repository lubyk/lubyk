function bang(sig)
	g = g or 0
	g = g + 1
	n = 20
	list = {60,n,63,n, n,n,65,67, 67,62,n,n, 72,n,n,73}
	return list[1 + g % #list]
end