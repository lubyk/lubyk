n = 0.0
function bang()
	n = n + math.pi / 120
	--if (n > 2 * math.pi) then n = 0 end
	
	send(1, {math.cos(n * 1.1), math.sin(n),
	   1.2 * math.cos(n * 1.1), 1.2 * math.sin(n),
	   1.4 * math.cos(n * 1.1) * (1.0 + 0.05 * math.cos(n *10)), 1.4 * math.sin(n) * (1.0 + 0.05 * math.sin(n *10)),
	   1.6 * math.cos(n), 1.6 * math.sin(n)})
end