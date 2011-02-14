--[[------------------------------------------------------

  wii.Remote
  ----------

  Access to a Wii Remote controller.

--]]------------------------------------------------------
require 'wii'
require 'wii.Remote_core'
require 'wii.Browser'
require 'worker'

local constr = wii.Remote
local worker = worker
local unconnected_remotes = {}
local connected_remotes = {}
function wii.Remote(name)
  local instance = nil
  if connected_remotes[name] then
    instance = connected_remotes[name]
    connected_remotes[name] = nil
  elseif connected_remotes[1] then
    instance = connected_remotes[1]
    table.remove(connected_remotes, 1)
  else
    instance = constr(worker)
    if name then
      unconnected_remotes[name] = instance
    else
      table.insert(unconnected_remotes, instance)
    end
    wii.Browser:find()
  end
  return instance
end

function wii.Browser.found(name)
  local instance = unconnected_remotes[name]
  if instance then
    unconnected_remotes[name] = nil
  elseif unconnected_remotes[1] then
    instance = unconnected_remotes[1]
    table.remove(unconnected_remotes, 1)
  else
    instance = constr(worker, name)
    if name then
      connected_remotes[name] = instance
    else
      table.insert(connected_remotes, instance)
    end
  end
  return instance
end
