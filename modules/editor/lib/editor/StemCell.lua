--[[------------------------------------------------------

  editor.StemCell
  ---------------

  Used as proxy for lk.StemCell.

--]]------------------------------------------------------

local lib       = {type='editor.StemCell'}
lib.__index     = lib
editor.StemCell = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, opts)
  local self = {
    stem_name = opts.stem_name,
    name      = opts.name,
    title     = '+',
    machine   = opts.machine,
  }

  setmetatable(self, lib)
  return self
end})


function lib:connect(remote)
  self.push = remote.push
  self.online = true
  -- noop
end

function lib:disconnect(remote)
  self.push = nil
  self.online = false
  -- noop
end

function lib:quit()
  if self.online then
    self.push:send(lubyk.quit_url)
  end
end
