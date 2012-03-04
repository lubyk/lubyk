--[[------------------------------------------------------

  mdns.Browser
  ------------

  This class calls a callback whenever new devices for a
  given service type appear or are removed on the network
  (this is an implementation of the ZeroConf or Bonjour
  browser).

--]]------------------------------------------------------
require 'mdns.Browser_core'
-- mdns.Service mt must be loaded
require 'mdns.Service'
local lib = mdns.Browser_
local constr = mdns.Browser
local private = {}

local ctx = mdns.Context()

function mdns.Browser(service_type, func)
  local self = constr(ctx, service_type)
  if func then
    self.addDevice = func
    self.removeDevice = func
  end

  self.found = {}

  local fd = self:fd()
  if fd then
    -- File descriptor based browser (macosx)
    self.thread = lk.Thread(function()
      while true do
        sched:waitRead(fd)
        local service = self:getService()
        while service do
          self:browseCallback(service)
          service = self:getService()
        end
      end
    end)
  else
    -- Without a filedescriptor, 'browseCallback' will be directly
    -- called by lk.SelectCallback created by the mdns.Context.
  end
  return self
end

--=============================================== Dummy implementations
function lib:addDevice(service)
end

function lib:removeDevice(service)
end

--=============================================== PRIVATE
function lib:browseCallback(service)
  local name = service:name()
  if service:isAdd() then
    if not self.found[name] then
      -- only add once
      self.found[name] = service
      local fd = service:fd()
      if fd then
        -- File descriptor based service: needs resolution.
        service.thread = lk.Thread(function()
          -- Resolve the service name
          while true do
            sched:waitRead(fd)
            if self.found[name] then
              -- Hasn't been removed before resolution.
              local info = service:info()
              if info then
                service.info = info
                -- only notify once for now
                self:addDevice(info)
                break
              end
            end
          end
          -- The mdns.Service should be deleted as soon as possible now so that we free the
          -- socket filedescriptor.
          service:__gc()
          self.found[name] = true
        end)
      else
        -- No file descriptor means the service is already
        -- resolved.
        local info = service:info()
        if info then
          service.info = info
          self:addDevice(info)
        end
        self.found[name] = true
      end
    end
  elseif self.found[name] then
    -- only remove once
    self.found[name] = nil
    self:removeDevice({name = name})
  end
end

