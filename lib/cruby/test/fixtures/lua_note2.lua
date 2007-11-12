function update(sig)
	g = g or 0
	g = g + 1
	n = 20
	list = {30,n,30,n, 35,n,30,n, 30,n,30,n, 30,n,30,n}
	
	if (g % 2) == 0 then
		return list[1 + ((g/2) % #list)]
	else
		return nil
	end
end