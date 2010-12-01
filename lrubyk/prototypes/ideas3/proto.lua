function node:set(hash)
  for k,v in pairs(hash) do
    self[k] = v
  end
end

--node:outlet('output')

node:inlet('input1').call = function(self, val)
  if val then
    -- receiving value
    self.val = val
  end
  return self.val
end

-- other way to declare the same
node:inlet('input2')