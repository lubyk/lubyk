g = 0
function update(sig)
  c = read_char()
	if (c) then
		g = g + 1
		write(c)
	end		
	if (g % 2) == 1 then
		set_read_offset(1)
	else
		set_read_offset(0)
	end
	return true
end