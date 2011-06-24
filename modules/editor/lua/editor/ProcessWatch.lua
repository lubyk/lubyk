--[[------------------------------------------------------

  editor.ProcessWatch
  -------------------

  Watches for remote lk.Processes and creates new
  editor.Process when they are found. When a process
  disappears: remove said Process.

  FIXME: merge with lk.ProcessWatch with a single API....
--]]------------------------------------------------------

local lib = {}
lib.__index         = lib
editor.ProcessWatch = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, delegate)
  local service_type = lubyk.service_type
  local self = {
    browser = lk.ServiceBrowser(service_type),
    found_processes   = {},
    pending_processes = {},
    delegate = delegate,
  }

  --======================================= SUB client

  self.sub = zmq.SimpleSub(function(...)
    -- receive message from remote server or local ServiceBrowser
    local url, service_name = ...
    if url == lubyk.add_service_url then
      -- resolve pending
      if self.found_processes[service_name] then
        -- allready found
        return
      end

      local remote_service = self.browser.services[service_name]
      local process = self.pending_processes[service_name]

      if process then
        -- pending process will be connected
        self.pending_processes[service_name] = nil
      else
        -- create new
        process = editor.Process(remote_service.name)
      end

      process:connect(remote_service, delegate, self)

      self.found_processes[service_name] = process
      self.delegate:addProcess(process)

    elseif url == lubyk.rem_service_url then
      local process = self.found_processes[service_name]
      -- remove connection
      if process then
        self.found_processes[service_name] = nil
        instance.pending_processes[service_name] = process
        process:disconnect()
        self.delegate:removeProcess(process)
      end
    else
      -- ???
    end
  end)
  -- so we can get connection commands from the service browser
  self.sub:connect(string.format('inproc://%s', service_type))

  setmetatable(self, lib)
  self:updateView()
  return self
end})

function lib:updateView()
  if not self.delegate.process_list_view then
    self.delegate.process_list_view = editor.ProcessList()
  end
end

function lib:findProcess(process_name)
  local process = self.found_processes[process_name] or
                  self.pending_processes[process_name]
  if not process then
    -- pending
    process = editor.Process(process_name)
    self.pending_processes[process_name] = process
  end
  return process
end
