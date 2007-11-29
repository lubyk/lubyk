old = 0
table = {}
table[49] = 48
table[50] = 50
table[51] = 51
function bang()
	---[[
	print(in1)
	if (in1 == 46) then
		old = in1
		return
	end
	if (in1 == old) then
		return
	end
	print(in1)
	old = in1
	return table[in1]
	--]]
end