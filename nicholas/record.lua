old = 0
function bang()
 -- print(in1)
  if (in1 == 46) then
    return
  end
	if (old ~= in1) then
		print(in1)
		old = in1
		if (in1 == 50) then
			return 46
		else
			return in1
		end
	end
end