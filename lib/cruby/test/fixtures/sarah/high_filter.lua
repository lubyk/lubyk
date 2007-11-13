g = 0 -- low pass filter
function update(sig)
	--print(in2,in1)
	if (in1 > 0.2) then
		g = 0
		if (in2 == 2 or in2 == 5) then
			if in1 > 0.7 then
				return in2
			end
		else
			return 1
		end
	end
	g = g + 1
end