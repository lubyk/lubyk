if not Inlet_mt then
  Inlet_mt = {}
  Inlet_mt.__index = Inlet_mt
end

local mt = Inlet_mt

function Inlet(node, name, method)
  local inlet = {node = node, name = name, method = method}
  setmetatable(inlet, mt)
  return inlet
end

function mt:receive(val)
  self.method(self.node, val)
end

--[[
/a/b/c/get

/a/b/c/set

method params

get '/a/b/c' ==> get value
set '/a/b/c' ==> set value ==> notify change

metro = {}

function metro:set_tempo(value)
end

function metro:get_tempo()
  return self.tempo
end
--]]