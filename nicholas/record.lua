old = 0
cooled = false
out = 0
function bang()
	if (in1 == 46 or old == in1) then
		old = in1
		return
	end
	old = in1
	print(in1)
	return in1
end