--[[------------------------------------------------------

  lk.SelectCallback
  -----------------

  Main event loop management. When we use a graphical
  interface, the scheduler automatically passes the
  loop to the GUI.

--]]------------------------------------------------------
require 'lk.core'
local constr = lk.SelectCallback_core.new
local lib    = lk.SelectCallback_core
lk.SelectCallback = lib

lib.type = 'lk.SelectCallback'

-- Callback from C++ when the fd flags change.
-- <update> <self> <read> <write> <timeout>
function lib:update(read, write, timeout)
  local fd = self:fd()
  if read then
    if not self.read_thread then
      self.read_thread = lk.Thread(function()
        while true do
          sched:waitRead(fd)
          local ok, err = pcall(
            self.callback, self, true, false, false)
          if not ok then
            sched:log('error', 'lk.SelectCallback.update: '.. err)
          end
        end
      end)
    end
  elseif self.read_thread then
    self.read_thread:kill()
    self.read_thread = nil
  end
  if write then
    if not self.write_thread then
      self.write_thread = lk.Thread(function()
        while true do
          sched:waitWrite(fd)
          local ok, err = pcall(
            self.callback, self, false, true, false)
          if not ok then
            sched:log('error', 'lk.SelectCallback.update: '.. err)
          end
        end
      end)
    end
  elseif self.write_thread then
    self.write_thread:kill()
    self.write_thread = nil
  end
  if timeout >= 0 then
    if self.timeout_thread then
      self.timeout_thread:kill()
    end
    self.timeout_thread = lk.Thread(function()
      sleep(timeout)
      local ok, err = pcall(
      self.callback, self, false, false, true)
      if not ok then
        sched:log('error', 'lk.SelectCallback.update: '.. err)
      end
      self.timeout_thread = nil
    end)
  elseif self.timeout_thread then
    self.timeout_thread:kill()
    self.timeout_thread = nil
  end
end

-- Remove from scheduler, call finalizer if there exists any.
function lib:remove()
  if self.read_thread then
    self.read_thread:kill()
    self.read_thread = nil
  end
  if self.write_thread then
    self.write_thread:kill()
    self.write_thread = nil
  end
  if self.timeout_thread then
    self.timeout_thread:kill()
    self.timeout_thread = nil
  end
  if self.finalize then
    self.finalize()
  end
end
