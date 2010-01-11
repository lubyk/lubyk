g = 0 -- low pass filter
function update(sig)
	if (in1 > 0.2) then
		--print(in2,in1)
		g = 0
		return in2
		--if (in2 == 3 or in2 == 9) then
		--	if in1 > 0.1 then
		--		if (in2 == 9) then
		--			return 5
		--		else
		--			return 2
		--		end
		--	end
		--else
		--	if in2 == 1 then
		--		return 1
		--	end 
		--end
	end
	g = g + 1
end