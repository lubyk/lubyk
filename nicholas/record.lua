old = 0
function bang()
 -- print(in1)
  if (in1 == 56) then
    return
  end
	if (old ~= in1) then
		print(in1)
		old = in1
		return in1 + 20
	end
end