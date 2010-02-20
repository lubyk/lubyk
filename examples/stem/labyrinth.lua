math.randomseed(os.time())

function grow(stem)
  if stem.next then
    -- once we have grown, we do not move
    return
  end
  -- we randomly turn left right or go forward
  try  = table.shuffle({1,2,3})
  for i=1,3 do
    if try[i] == 1 then
      -- left
      if entropy(-1, 0) < 0.8 then
        append(-1, 0)
        break
      end
    elseif try[i] == 2 then
      -- front
      if entropy(0, 1) < 0.8 then
        append(0, 1)
        break
      end
    else
      -- right
      if entropy(1, 0) < 0.8 then
        append(1, 0)
        break
      end
    end
  end
end



--[[

-- Find the best direction to grow
best = {
  chaos = 1.0,
  x = 0,
  y = 0
}
for x=-1.0,1.0,0.5 do
  for y=-1.0,1.0,0.5 do

  end
end

--]]

function table.shuffle(t)
  local n = #t -- n is the last pertinent index (loop invariant)
  while n > 1 do
    local k = math.random(n) -- 1 <= k <= n
    -- Quick swap
    t[n], t[k] = t[k], t[n]
    n = n - 1
  end

  return t
end

