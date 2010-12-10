
node:outlet('output')

node:inlet('input').call = function(self, val)
  if val then
    print(string.format("[%s] %s <--- '%s'", self.name, 'input', val))
    -- receiving value
    self.val = val
    -- can we notify without explicitly adding this line ?
    self:notify('input', val)
    -- prepend name and send to outlet
    self:send('output', self.name .. ': ' .. val)
  end
  return self.val
end

-- other way to declare the same
node:inlet('input2')