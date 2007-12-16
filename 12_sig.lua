n = 0.0
m = math.pi / 3
function bang()
	n = n + math.pi / 150
	m = m + math.pi / 150
	--if (n > 2 * math.pi) then n = 0 end
	
	send(1, {
	   math.cos(n) * math.cos(2.3 * m),
	   math.cos(n*1.5)  * math.cos(2.3 * m),
 	   math.cos(n*1.7) * math.cos(m * 2.2),
 	   
 	   math.cos((n + math.pi/5)),
 	   math.cos((n + math.pi/5)*1.5),
 	   math.cos((n + math.pi/5)*1.7),
 	   
 	   math.cos((n + 2*math.pi/5)),
 	   math.cos((n + 2*math.pi/5)*1.5),
 	   math.cos((n + 2*math.pi/5)*1.7),

	   math.cos((n + 3*math.pi/5)),
	   math.cos((n + 3*math.pi/5)*1.5),
	   math.cos((n + 3*math.pi/5)*1.7),
	   })
end