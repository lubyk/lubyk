function bang(sig)
	g = g or 0
	g = g + 1
	n = 20
	list = {60,62,63,n, 63,n,65,n, 67,62,n,n, 72,n,n,n}
	return list[1 + g % #list]
end