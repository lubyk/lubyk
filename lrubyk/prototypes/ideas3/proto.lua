
-- we need the 'or' to properly reload
node = node or {}

function node:set(hash)
  for k,v in pairs(hash) do
    self[k] = v
  end
end
