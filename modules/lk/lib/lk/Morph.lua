--[[------------------------------------------------------

  lk.Morph
  --------

  This Service announces itself with "morph" type and
  "Morpheus" name. It's role is to provide save and restore
  functionality to a processing network.

  The Morph server relies on the Lubyk.zone global setting.

  TODO: automatic versioning of saved changes (git)

  Usage: a morph process is started with a path to a lubyk
  project file (*.lkp, yaml content). The lkp file contains
  information on machine assignement of the processes.

--]]------------------------------------------------------

local lib = {type='lk.Morph'}
lib.__index = lib
lk.Morph    = lib

local private = {
  -- Actions triggered on a 'set' operation (while reading project).
  set     = {},
  -- Actions triggered on an 'update' operation (update from GUI).
  update = {},
  -- Actions triggered on a (partial) 'dump' operation.
  dump    = {},
  -- Actions related to process handling.
  process = {},
  -- Actions related to node handling.
  node    = {},
  -- Actions related to view handling.
  view    = {},
}

setmetatable(lib, {
  -- new method
 __call = function(lib, opts)
  local self = {
    -- Version of lubyk used to create project
    lubyk      = {version = Lubyk.version},
    -- Holds the list of all the processes that need to be running
    -- for the project to work (not just the ones actually running).
    processes  = {},
    -- Found stem cells (used to create new processes)
    stem_cells = {},
    -- Holds the list of views declared for this project.
    views      = {},
  }
  setmetatable(self, lib)

  if opts then
    self:start(opts)
  end
  return self
end})

function lib:start(opts)
  local srv_opts = {
    callback = function(...)
      return self:callback(...)
    end,
    registration_callback = function(reg, service)
      if (Lubyk.zone .. ':') ~= service.name then
        -- We do not want to have two morph servers on the same zone.
        printf("Another morph service is running in zone '%s'. Quit.", Lubyk.zone)
        sched:quit()
      else
        private.start(self, opts)
      end
    end,
    type = 'lk.Morph',
    info = opts.info,
    quit = function()
      self:quit()
    end,
  }
  -- Setup ports and publish service
  self.service = lk.Service(Lubyk.zone .. ':', srv_opts)
end

function lib:openFile(filepath)
  self:close()
  local base, name = lk.pathDir(filepath)
  if not lk.exist(filepath) then
    -- New project
    lk.makePath(base)
    lk.writeall(filepath, '')
  end
  local views_path = base .. '/_views'
  if not lk.exist(base..'/_views') then
    -- Default view
    lk.makePath(views_path)
  end

  local dir = lk.Dir(views_path)
  if not dir:contains('%.lkv$') then
    lk.writeall(views_path .. '/base.lkv', '')
  end
  self.root     = lk.FileResource(base)
  self.lkp_file = lk.FileResource('/' .. name, self.root)
  private.readFile(self)
end

function lib:close()
  -- TODO: notify changes (mark old processes with 'false')
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

function lib:update(definitions)
  local upd = private.update
  for k, v in pairs(definitions) do
    local func = upd[k]
    if func then
      func(self, v)
    end
  end
end

function lib:dump()
  return self:partialDump {
    -- This is to give the future host on unconnected processes in
    -- the editor.
    lubyk     = true,
    host      = true,
    name      = true,
    processes = true,
    _views    = true,
  }
end

function lib:partialDump(data)
  local dump = {}
  local d = private.dump
  for k, v in pairs(data) do
    local func = d[k]
    if func then
      dump[k] = func(self, v)
    end
  end
  return dump
end

--============================================= lk.Service delegate

local dump_url,        update_url,       get_url,       quit_url =
      lubyk.dump_url,  lubyk.update_url, lubyk.get_url, lubyk.quit_url

--- Answering requests to Morph.
function lib:callback(url, ...)
  if url == dump_url then
    return self:dump()
  elseif url == update_url then
    -- async call, no return value
    self:update(...)
    local p = self:partialDump(...)
    self.service:notify(p)
  elseif url == get_url then
    return self:get(...)
  elseif url == quit_url then
    self:quit()
  else
    -- ignore
    self:error("Invalid url '%s'.", url)
  end
end

function lib:error(...)
  -- TODO: notify errors.
  print(msg)
end

function lib:quit()
  self.quitting = true
  for k, process in pairs(self.processes) do
    if process.online then
      process.push:send(lubyk.quit_url)
    end
  end
  if self.stem_fd then
    -- Kill the stem cell that we created.
    local stem = self.stem_cells[Lubyk.host]
    if stem then
      stem.push:send(lubyk.quit_url)
    end
  end
  sched:quit()
end

--=============================================== lk.ProcessWatch delegate

--- When services are brought online
function lib:processConnected(remote_process)
  -- We need to receive notifications from this process so that
  -- we can write the changes to file.
  local process = self.processes[remote_process.name]
  if process then
    -- we are interested in this process
    private.process.connect(self, process, remote_process)
  else
    -- Is this a stem cell ?
    local stem_name = string.match(remote_process.name, '^@(.*)$')
    if stem_name then
      -- stem cell
      self.stem_cells[stem_name] = remote_process
      -- start pending processes
      for name, process in pairs(self.processes) do
        if not process.online and process.host == stem_name then
          if process.on_stem == 'restart' then
            process.on_stem = nil
            private.process.restart(self, process)
          else
            -- START
            private.process.start(self, process)
          end
        end
      end
    elseif remote_process.name ~= '' then
      printf("Invalid process '%s'. Kill", remote_process.name)
      -- Invalid process. Kill.
      remote_process.push:send(lubyk.quit_url)
    end
  end
end

function lib:processDisconnected(service)
  local name    = service.name
  local process = self.processes[name]
  if process and not self.quitting then
    -- Restart process
    process.online = false
    private.process.start(self, process)
  elseif self.stem_cells[name] then
    self.stem_cells[name] = nil
  end
end

--=============================================== PRIVATE

--- Start service and launch processes.
function private:start(opts)
  self.process_watch = opts.process_watch or lk.ProcessWatch()
  self.process_watch:addDelegate(self)
  if opts.path then
    self:openFile(opts.path)
  end
  if opts.start_stem then
    private.startStemCell(self)
  end
end


-- Reads and parses the content of the lkp file.
function private:readFile()
  -- Safe yaml loading (no alias = no recursion).
  local def = yaml.load(self.lkp_file:body(), true)
  if type(def) ~= 'table' then
    def = {}
  end
  local h = private.set
  for k, func in pairs(private.set) do
    -- private.set.lubyk(self, def.lubyk)
    func(self, def[k])
  end
  self.views_dir = private.findOrMakeResource(self, '/_views', true)
  local dir = lk.Dir(self.views_dir.path)
  for file in dir:glob('%.lkv') do
    local name = string.match(file, '/([^/]+)%.lkv$')
    private.view.add(self, name, {}, true)
  end
end

function private:writeFile()
  local dump = self:dump()
  -- not saved in this file
  dump._views = nil
  dump.name   = nil
  dump.lubyk  = self.lubyk
  self.lkp_file:update(yaml.dump(dump))
end

--- Helper Used while parsing/updating patch definitions.
function private.findOrMakeResource(self, url, is_dir)
  local resource = self.root.cache[url]
  if not resource then
    local fullpath = self.root.path .. url
    if not lk.exist(fullpath) then
      if is_dir then
        lk.makePath(fullpath)
      else
        lk.makePath(lk.pathDir(fullpath))
        lk.writeall(fullpath, '')
      end
    end
    resource = lk.FileResource(url, self.root)
  end
  return resource
end

function private:startStemCell()
  -- This is ugly, but we want to make sure this stem cell is not
  -- started before we see if there is any on the network.
  if not self.stem_cells[Lubyk.host] then
    self.stem_fd = lk.spawn([[
    require 'lubyk'
    stem = lk.StemCell()
    run()
    ]])
  end
end

--=============================================== SET

-- Parse Lubyk version information.
function private.set:lubyk(lubyk)
  -- We could check for version compatibility here.
  if lubyk and lubyk.version then
    self.lubyk = lubyk
  else  
    self.lubyk = {version = Lubyk.version}
  end
end

--- Parse processes machine assignments.
function private.set:processes(processes)
  for name, info in pairs(processes or {}) do
    if type(info) == 'table' then
      -- ok
    elseif info == '' then
      -- localhost
      info = {host = Lubyk.host}
    else
      info = {host = info}
    end
    private.process.add(self, name, info, true)
  end
end

--=============================================== UPDATE

--- Change processes: create, delete or change machine assignment.
function private.update:processes(data)
  for name, def in pairs(data) do
    if def == false then
      private.removeProcess(self, name)
    elseif not self.processes[name] then
      private.createProcess(self, def)
    elseif def == 'restart' then
      private.process.restart(self, self.processes[name])
    else
      -- Change machine assignation. Other changes are notified by
      -- process.
      -- TODO
    end
  end
end

function private:createProcess(definition)
  local processes = self.processes
  local name = definition.name
  if name == '_views' then
    -- ERROR, not allowed
    printf("Cannot create a process named '_views' (reserved name).")
  elseif processes[name] then
    -- ERROR
    printf("Cannot create existing process '%s'.", definition.name)
  else
    private.process.add(self, name, definition)
  end
end

function private:removeProcess(name)
  local process = self.processes[name]
  if process then
    if process.online then
      process.push:send(lubyk.quit_url)
      self.processes[name] = nil
      private.writeFile(self)
      process.dir:delete()
    end
  else
    -- ERROR
    printf("Cannot remove unknown process '%s'.", name)
  end
end


--=============================================== DUMP

--- Dump name
-- default: "tmp/bob.lkp" @ gaspard
function private.dump:name()
  local path = self.root.name .. '/' .. self.lkp_file.name 
  return string.format('"%s" @ %s', path, Lubyk.host)
end

function private.dump:host()
  return Lubyk.host
end

--- Dump Lubyk version information.
function private.dump:lubyk(partial)
  return self.lubyk
end

--- Dump information on processes machine assignments.
function private.dump:processes(partial)
  local to_dump
  if partial == true then
    to_dump = self.processes
  else
    to_dump = partial
  end
  local dump = {}
  for name, def in pairs(to_dump) do
    if def == false then
      -- Removal information
      dump[name] = false
    else
      local process = self.processes[name]
      local p, empty = {}, true
      if process.host ~= Lubyk.host then
        p.host = process.host
        empty = false
      end
      if process.dir.name ~= name then
        p.dir = process.dir.name
        empty = false
      end
      if empty then
        dump[name] = ''
      else
        dump[name] = p
      end
    end
  end
  return dump
end

--- Dump information on views
function private.dump:_views(partial)
  local to_dump
  if partial == true then
    to_dump = self.views
  else
    to_dump = partial
  end
  local dump = {}
  for name, def in pairs(to_dump) do
    if def == false then
      -- Removal information
      dump[name] = false
    elseif partial == true then
      -- Full dump
      dump[name] = def.cache
    else
      -- Partial dump: just echo changes back
      dump[name] = def
    end
  end
  return dump
end

--=============================================== PROCESS

--- A process appeared on the network, we connect to this process to receive
-- notifications.
function private.process:connect(process, remote_process)
  process.sub = zmq.SimpleSub(function(changes)
    -- we receive notifications, update content
    -- FIXME: filter control events ?
    private.process.changed(self, process, changes)
  end)
  process.sub:connect(remote_process.sub_url)
end

--- A process died, disconnect.
function private.process:disconnect(process)
  process.sub = nil
end

--- We receive notifications from processes.
function private.process.changed(self, process, changes)
  -- write changes to file    
  local cache = process.cache
  for base_k, base_v in pairs(changes) do
    if base_k == 'nodes' then
      local nodes = base_v
      local cache_nodes = cache[base_k]
      if not cache_nodes then
        cache_nodes = {}
        cache[base_k] = cache_nodes
      end
      for name, node in pairs(nodes) do
        local cache_node = cache_nodes[name]
        local links
        if not node then
          if not cache_node then
            -- ignore
          else
            -- removed node
            cache_nodes[name] = nil
            patch_changed = true
            -- remove file
            local url = process.dir.url .. '/' .. name .. '.lua'
            local resource = self.root.cache[url] 
            if resource then
              -- delete file
              resource:delete()
            end
          end
        else
          if not cache_node then
            -- new node
            local resource = process.dir:createChild(name .. '.lua', node.code or '')
            resource:addCallback('update', private.node.updateCallback, process, name)
            cache_node = {}
            cache_nodes[name] = cache_node
          end

          for k, v in pairs(node) do
            if k == 'code' then
              -- ignore code change notifications (we sent the code)
            elseif k == 'inlets' or k == 'has_all_slots' then
              -- ignore (not in patch)
            elseif k == 'outlets' then
              -- extract links
              for _, outlet in ipairs(v) do
                print('MORPH FUCK', name, yaml.dump(outlet))
                local l = outlet.links
                if l then
                  if not links then
                    links = {}
                  end
                  if type(l) == 'table' then
                    local lnks
                    if node.has_all_slots then
                      lnks = {}
                    else
                      lnks = links[outlet.name] or {}
                    end
                    for target_url, on in pairs(l) do
                      if on then
                        lnks[target_url] = true
                      else
                        lnks[target_url] = nil
                      end
                    end
                    links[outlet.name] = lnks
                  else
                    links[outlet.name] = l
                  end
                end
              end
            else
              patch_changed = lk.deepMerge(cache_node, k, v) or patch_changed
            end
          end
          
          if links then
            patch_changed = true
            cache_node.links = links
          end
        end
      end
    elseif base_k == 'log' then
      -- ignore
    else
      patch_changed = lk.deepMerge(cache, base_k, base_v) or patch_changed
    end
  end
  if patch_changed then
    private.process.writeFile(process)
  end                                         
end

--- Read and parse process patch definition file.
function private.process.readFile(self, process)
  process.cache = yaml.load(process.patch:body(), true) or {}
  if type(process.cache) ~= 'table' then
    process.cache = {}
  end
  local nodes = process.cache.nodes or {}
  for name, def in pairs(nodes) do
    local resource = private.findOrMakeResource(self, process.dir.url .. '/' .. name .. '.lua')
    resource:addCallback('update', private.node.updateCallback, process, name)
  end
end

--- Write patch definition to file.
function private.process.writeFile(process)
  process.patch:update(yaml.dump(process.cache))
end

-- When reading a file 'reading_lkp' is set so we know that we must not
-- write to lkp file.
function private.process.add(self, name, info, reading_lkp)
  -- Find or create process.
  local process = self.process_watch:process(name)
  self.processes[name] = process
  process.host = info.host
  -- set resource
  process.dir = private.findOrMakeResource(self, '/' .. (info.dir or name), true)
  process.patch = private.findOrMakeResource(self, process.dir.url .. '/_patch.yml')

  private.process.readFile(self, process)
  if not reading_lkp then
    -- This is a new process
    lk.deepMerge(process, 'cache', info)
    -- Host is not stored inside the patch
    process.cache.host = nil
    private.writeFile(self)
    private.process.writeFile(process)
  end
  private.process.start(self, process)
end

--- Try to start a process by calling the corresponding stem cell.
function private.process:start(process)
  local stem = self.stem_cells[process.host]
  if not stem or not stem.push then
    -- will start as soon as the stem cell appears on the network
    return
  end
  stem.push:send(lubyk.execute_url, 'spawn', process.name)
end

--- Kill and restart process.
function private.process:restart(process)
  if not process.online then
    private.process.start(self, process)
  else
    local stem = self.stem_cells[process.host]
    if not stem then
      -- will restart as soon as the stem cell appears on the network
      process.on_stem = 'restart'
    else
      stem.push:send(lubyk.execute_url, 'kill', process.name)
    end
  end
end

function private:restartProcess(name)
  local process = self.processes[name]
  if process then
    if process.online then
      process.push:send(lubyk.quit_url)
      -- The process will restart automatically.
    end
  else
    -- ERROR
    printf("Cannot restart unknown process '%s'.", name)
  end
end

--=============================================== NODE

-- This is called when we do an update on the resource (file save).
function private.node.updateCallback(process, node_name, resource)
  -- We launch a new thread to make sure that we do not hang the server
  -- if this fails.
  process.update_thread = lk.Thread(function()
    if process.online then
      process.push:send(lubyk.update_url, {
        nodes = {
          [node_name] = { code = resource:body()}
        }
      })
    end
  end)
end

--=============================================== VIEW

--- Change views (we do a deep parsing to detect what to create/delete/update).
function private.update:_views(data)
  local views = self.views
  for name, def in pairs(data) do
    local view = views[name]
    if def == false then
      if view then
        view.file:delete()
      end
    elseif not view then
      private.view.add(self, name, def)
    else
      -- update
      for id, opt in pairs(def) do
        if not opt then
          view.cache[id] = nil
          view[id] = nil
        else
          lk.deepMerge(view.cache, id, opt)
        end
      end
      -- write view to filesystem
      private.view.writeFile(view)
    end
  end
end

-- When reading a view file 'reading_lkv' is set so we know that we must not
-- write to lkv file.
function private.view.add(self, name, info, reading_lkv)
  local view = {}
  self.views[name] = view
  -- set resource
  view.file = private.findOrMakeResource(self, self.views_dir.url .. '/' .. name .. '.lkv')

  private.view.readFile(self, view)
  if not reading_lkv then
    -- This is a new view
    lk.deepMerge(view, 'cache', info)
    private.view.writeFile(view)
  end
end

--- Read and parse process view definition file.
function private.view.readFile(self, view)
  view.cache = yaml.load(view.file:body(), true) or {}
  if type(view.cache) ~= 'table' then
    view.cache = {}
  end
end

--- Write patch definition to file.
function private.view.writeFile(view)
  view.file:update(yaml.dump(view.cache))
end

-- We need this for testing
lib.private = private
