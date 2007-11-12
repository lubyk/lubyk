read_count = -2 -- start by waiting for 255 255
offset_needed = true
read_offset   = 0
max_direction = 0
offset = {}
values = {}

for i=1,12 do
	offset[i] = 0
	values[i] = 0
end

function update(sig)
  local has_data = false
  local i, c, high, value
	high = 0
	c = read_char()
	while(c) do
	
		if (read_count == -2) then
			if (c == 255) then
				read_count = read_count + 1
			end
		elseif (read_count == -1) then
		  if (c == 255) then
				read_count = read_count + 1
			else
				read_count = -2 -- go back to sync wait
			end
		elseif (read_count % 2 == 0) then
			read_count = read_count + 1
			high = c
		else
			read_count = read_count + 1
			value = ((256*high) + c)/1024 * (3300 - 1650) / 200
			i = (read_count/2)
			values[i] = value - offset[i]
			if max_direction == 0 or value > values[max_direction] then
				max_direction = i
			end
			if (read_count == 24) then
				max_direction = 0
				write(values)   -- write to buffer
				has_data   = true
				read_count = -2 -- goto sync mode again
				if (offset_needed) then
					offset_needed = false
					for i=1,12 do
						offset[i] =  values[i] + offset[i]
					end
				end
			end
		end
		c = read_char()
	end
	--if (has_data and max_direction > 0) then
	if has_data then
		--if values[max_direction] > 0.3 or values[max_direction] < -0.3 then
		--	return max_direction
		--end
		return true
	end
end
print("ok")
