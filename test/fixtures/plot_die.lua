n = 0.0
function bang()
	n = n + math.pi / 70
	--if (n > 2 * math.pi) then n = 0 end
	send(1, {math.cos(n), math.sin(n * 1.2)})
end