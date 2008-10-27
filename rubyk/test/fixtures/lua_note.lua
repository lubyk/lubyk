function bang(sig)
	g = g or 0
	g = g + 1
	n = 20
	list = {n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n}
	return list[1 + g % #list]
end