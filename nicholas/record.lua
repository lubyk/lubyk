old = 0
cooled = false
out = 0
function bang()
	print(in1)
	if (in1 == 46 or old == in1) then
		old = in1
		return
	end
	old = in1
	if (in1 == 49) then
		return 72
	end
	return in1
end