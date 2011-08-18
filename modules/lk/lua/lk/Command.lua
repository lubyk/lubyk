--[[------------------------------------------------------

  lk.Command
  ----------

  The Command executes single commands or opens a console
  to interact with running lubyk morph servers and process.
  This is like a simplified (or scriptable) GUI.

--]]------------------------------------------------------

local lib = {type='lk.Command'}
lib.__index = lib
lk.Command  = lib

--private
local getTarget

setmetatable(lib, {
  -- new method
 __call = function(lib, url, ...)
  local zone = 'default' --lk.Setting('zone', 'default')

  local self = {
    zone  = zone,
    zones = {},
    -- private flag to find more processes
    find_more = true,
  }
  setmetatable(self, lib)
  self.zones[zone] = {}
  self.current = self.zones[zone]

  if url then
    url = self:specialCommand(url, ...)
    if not url then
      -- done
    else
      local target, cmd = getTarget(url)
      if target ~= '*' then
        -- just find this target, execute command and quit
        self.target = target
        self.cmd = {url, ...}
      elseif cmd then
        -- star based url (run on many processes)
        self.on_load = {url, ...}
      end
    end
  end

  if self.find_more then
    self.process_watch = lk.ProcessWatch():addDelegate(self)
    while(self.find_more) do
      self.find_more = nil
      if self.abort then
        break
      else
        sleep(1000) -- let the processes resolve
      end
    end
  end

  if self.on_load then
    self:call(unpack(self.on_load))
    self.on_load = nil
  end

  return self
end})

function getTarget(url)
  return string.match(url, '^/([^/]*)(.*)$')
end

-- TODO: Transform this to a command line that we could use to do everything that
-- we can do with the GUI.
--
-- For now simply enable the "-e" (exec) mode with the command to halt all local processes related to
-- lubyk:
-- > lubyk --zone default '/*/lk/quit'
-- Which is the same as
-- > lubyk --zone default quit
-- Or (by using .lubykrc for default zone)
-- > lubyk quit
--
-- To stop only process "a"
-- > lubyk --zone default /a/lk/quit
-- > lubyk --zone default quit a
--
-- Command format:
-- /[URL] {arg, ...}
--
-- CMD {target (default=*)} {arg, ...}
-- ==
-- /target/lk/CMD {arg, ...}
function lib:call(url, ...)
  local target_name, cmd = getTarget(url)
  if target_name == '*' then
    for _, service in pairs(self.current) do
      print(service.req:send(cmd, ...))
    end
  elseif target_name then
    local service = self.current[target_name]
    if service then
      print(service.req:send(cmd, ...))
    else
      print(string.format("Process '%s' not found.", target_name))
    end
  else
    print(string.format("Bad url '%s' (could not find process name)."))
  end
end

--================================================== lk.ProcessWatch delegate

function lib:addService(service)
  self.find_more = true
  local zone = self.zones[service.zone]
  if not zone then
    self.zones[service.zone] = {}
    zone = {}
  end
  zone[service.name] = service
  if self.target and service.zone == self.zone and service.name == self.target then
    self:call(unpack(self.cmd))
    self.abort = true
  end
end

function lib:removeService(service)
  local zone = self.zones[service.zone]
  if zone then
    zone[service.name] = nil
  end
end

--================================================== Special commands
function lib:specialCommand(cmd, ...)
  if cmd == '--test' then
    local all = test.all
    test.all = function() end -- dummy
    test.loadAll(...)
    all()
    return nil
  elseif cmd == '--testInstall' then
    app = mimas.Application()
    dlg = editor.SimpleDialog {
      message = 'Lubyk works!',
--      cancel = 'Test more',
      ok = 'Ok',
    }
    function dlg.ok()
      dlg:close()
      self.find_more = false
    end
--    function dlg.cancel()
--      self:specialCommand('--test')
--    end
    dlg:resize(200,200)
    dlg:move(10, 10)
    dlg:show()
    app:exec()
    return nil
  elseif action == 'halt' then
    return '/*/lk/quit'
  else
    return cmd
  end
end
