--[[------------------------------------------------------

  lk.StemCell
  -----------

  This is a very simple service whose sole purpose is to
  announce a machine and enable remote morph to start new
  processes.

--]]------------------------------------------------------
local lib = {type='lk.StemCell'}
lib.__index = lib
lk.StemCell = lib

local private = {}

setmetatable(lib, {
  -- new method
 __call = function(lib)
  local self = {
    -- Machine name
    name = Lubyk.host,
    -- List of pids in case we must restart a process.
    processes = {},
  }
  setmetatable(self, lib)

  private.start(self)
  return self
end})

--============================================= lk.Service delegate

local execute_url,        quit_url = 
      lubyk.execute_url,  lubyk.quit_url

function lib:callback(url, ...)
  if url == execute_url then
    return private.execute(self, ...)
  elseif url == quit_url then
    self:quit()
  else
    -- ignore
    self:error("Invalid url '%s'.", url)
  end
end

function lib:error(...)
  -- TODO: notify errors.
  printf(...)
end

-- The stem cell is the parent of the following processes, it cannot die
-- before them.
function lib:quit()
  for name, pid in pairs(self.processes) do
    private.kill(pid)
  end

  sched:quit()
end

--=============================================== PRIVATE
--- Start service.
private.actions = {}

function private:start()
  local srv_opts = {
    callback = function(...)
      return self:callback(...)
    end,
    registration_callback = function(req, service)
      private.registrationCallback(self, service)
    end,
    type = 'lk.StemCell',
    quit = function()
      self:quit()
    end,
  }
  self.service = lk.Service(Lubyk.zone .. ':@' .. self.name , srv_opts)
end

function private:registrationCallback(service)
  if (Lubyk.zone .. ':@' .. self.name) ~= service.name then
    -- We do not want to have two stem cells with the same name running
    printf("Another stem cell with name '%s' exists in zone '%s'. Quit.", self.name, Lubyk.zone)
    self:quit()
  end
end

function private:execute(action, ...)
  local func = private.actions[action]
  if func then
    func(self, ...)
  else
    printf("Unknown action '%s'.", action)
  end
end

--=============================================== ACTIONS

-- Spawn a new process that will callback to get data (yml definition, assets).
function private.actions:spawn(name)
  -- Spawn Process. If a process dies, the stem cell will restart it.
  self.processes[name] = worker:spawn([[
  require 'lubyk'
  process = lk.Process(%s)
  run()
  ]], name)
  -- the process will find the morph's ip/port by it's own service discovery
end

-- Restart a given process (kill and let morph restart).
function private.actions:kill(name)
  local pid = self.processes[name]
  -- kill
  if pid then
    private.kill(pid)
  end
  -- The process will be restarted.
end

function private.kill(pid)
  os.execute(string.format('kill -9 %i', pid))
end
