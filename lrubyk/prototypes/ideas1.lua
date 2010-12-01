-- full patch definition

--[[
the patch
---------


+--------+            +-----+
| Cabox  | (y)--(val1)| Sum |
| captor |            | sum | (sum)--+
| captor | (x)--(val2)|-----+        |
+--------+                           |
                   +-----------------+
                   |             
                   |         +--------+
                   +-- (rot) | Patch  |
                             | cube   |
                   +-- (draw)|        |
+-------+          |         +--------+
| GLWin |          |
| win   | (draw)---+---(draw)+-------+
+-------+                    | Lua   |
                             | face  |
                             +-------+
]]

-- /captor
captor = Cabox()
captor.x:link('cube.sum')
captor.y:link('cube.sum')

-- /sum
sum = Sum()
sum.sum:link('cube.rot')

-- /win
win = GLWin()
win.draw:link('cube.draw')
win.draw:link('face.draw')

-- /cube
cube = Patch('cube.lua')

-- /face
face = GLLua('face.lua')

--[[
Sum.lua is a class definition

================ Sum.lua ===============
]]

local mt = Class('Sum')

-- this is what Class('Sum') does:
-- it defines the 'Sum' constructor
--[[
function Sum(...)
  local sum = Object()
  setmetatable(sum, mt)
  sum:init(...)
  return sum
end
]]

function mt:val1(val)
  self.val1 = val
  self:update_sum()
end

function mt:val2(val)
  self.val2 = val
  self:update_sum()
end

function mt:update_sum()
  self:send(self.val1 + self.val2)
end

function mt:init(...)
  self:inlet('val1', 0).method = mt.val1
  self:inlet('val2', 0).method = mt.val2
end

return mt

--[[
cube.lua is another patch with inlets and outlets abstracted at the patch level
================ cube.lua ===============
]]

local mt = Object()

patch:inlet('rot', nil).method = function(self, val)
  

--[[
face.lua is a Lua script (singleton)

================ face.lua ===============
]]

local obj = Object()

-- variant 1
obj:inlet('draw', nil, function(self, val)
  -- draw face with openGL
end)

-- variant 2
obj:inlet('draw', nil).method = function(self, val)
  -- draw face with openGL
end

-- variant 3
function draw(self, val)
  -- draw face with openGL
end
obj:inlet('draw', nil, draw)

