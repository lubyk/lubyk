old = 0
function bang()
	---[[
	--print(in1)
	if (in1 == 46) then
		old = in1
		return
	end
	if (in1 == old) then
		return
	end
	print(in1, in2)
	old = in1
	return in1
	--]]
end
	