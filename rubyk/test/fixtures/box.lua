mode   = mode or 0
inputs = inputs or 0
if node == 0 then -- waiting for sync
  if sig == 255
		mode = 1
  end
	return -1
end

if mode == 1 then -- first sync received
	if sig == 255 then -- move into data mode
	  inputs = 0
	  mode = 2 -- receive 1st char mode
	else
		mode = 0 -- error, return to sync mode
	end
	return -1
end

if mode == 2 then
	high = sig
	mode = 3
	return -1
end

val   = (high * 256) + sig
val_g = (val/1024) * (3300 - 1650) / 200 -- 3300 = fullscale_mv, 1024 = fullscale_bin, 1650 = offset_mv, 200 = sensitivity_mv
inputs = inputs + 1

if inputs >= 12
	mode = 0
end