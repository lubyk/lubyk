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
  }
  setmetatable(self, lib)

  private.start(self)
  return self
end})

function lib:openFile(filepath)
  self:close()
  local base, name = lk.directory(filepath)
  if not lk.exist(filepath) then
    lk.makePath(base)
    lk.writeall(filepath, '')
  end
  self.root     = lk.FileResource(base)
  self.lkp_file = lk.FileResource('/' .. name, self.root)
  private.readFile(self)
end

function lib:close()
  -- TODO: close all processes in same zone
  -- clear
  self.processes = {}
  self.root = nil
end

--- Return the content of the file at the given path in the
-- current project.
function lib:get(url)
  local resource = lk.FileResource(url, self.root)
  if not resource then
    return nil
  end
  return resource:body()
end
--============================================= lk.Service delegate

--- Answering requests to Morph.
function lib:callback(url, ...)
  if url == lubyk.execute_url then
    return private.execute(self, ...)
  elseif url == lubyk.quit_url then
    sched:quit()
  else
    -- ignore
    print('Bad message to lk.Morph', url)
  end
end

--=============================================== PRIVATE
--- Start service.
function private:start()
  local srv_opts = {
    callback = function(...)
      return self:callback(...)
    end,
    registration_callback = function(service)
      private.registrationCallback(self, service)
    end,
    type = 'lk.StemCell',
  }
  self.service = lk.Service(Lubyk.zone .. ':@' .. self.name , srv_opts)
end

function private:registrationCallback(service)
  if (Lubyk.zone .. ':@' .. self.name) ~= service.name then
    -- We do not want to have two stem cells with the same name running
    printf("There is already a stem cell with name '%s' in zone '%s'.", self.name, Lubyk.zone)
    sched:quit()
  end
end

function private:execute(action, ...)
  if action == 'spawn' then
    -- start a new process
    private.spawn(self, ...)
  else
    print('Unknown action', action)
  end
end

-- Spawn a new process that will callback to get data (yml definition, assets).
function private:spawn(name)
  -- spawn Process
  local pid = worker:spawn([[
  require 'lubyk'
  process = lk.Process(%s)
  run()
  ]], name)
  -- the process will find the morph's ip/port by it's own service discovery
end

