old = 0
function bang()
	if (in1 == 46) then
		old = in1
		return
	end
	if (in1 == old) then
		return
	end
	--print(in1)
	old = in1
	if (in1 == 49) then
		return 72
	end
	return 65
end
	