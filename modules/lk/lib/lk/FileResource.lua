--[[------------------------------------------------------

  lk.FileResource
  ---------------

  Represents the content of a file or directory in the
  filesystem in a format directly useable by WebDAV.

--]]------------------------------------------------------

local lib = {type='lk.FileResource', IGNORE = '^%.'}
lib.__index = lib
lk.FileResource = lib
-- Only store in cache if the body is less then this
local CACHE_MAX_SIZE = 30000
local private = {}

setmetatable(lib, {
  -- new method
  -- Create a FileResource. The path *MUST* be relative to root
  -- if root is provided and must start with a '/'.
 __call = function(lib, url, root)
  local self
  if root then
    self = root.cache[url]
    if self then
      return self
    end
    self = {
      root = root,
      path = root.path .. url,
      url = url,
   }
  else
    self = {
      path = url,
      url = '',
    }
    self.root = self
    -- keep all resources cached
    self.cache = {}
  end
  if not lk.exist(self.path) then
    return nil
  else
    self.root.cache[self.url] = self
  end
  self.callbacks = {}
  setmetatable(self, lib)
  return self:sync()
end})

-- accessor for WebDAV properties
lib.davProperty = rawget

function lib:sync()
  local stat, err = lfs.attributes(self.path)
  if not stat then
    -- does not exist (removed)
    return nil
  end
  self.name = string.match(self.path, '/([^/]+)$')
  self.children_list = nil
  self.body_cache    = nil
  local url = self.url
  if self.root == self then
    self.url = ''
    self.cache[self.url] = self
  else
    self.url = string.sub(self.path, string.len(self.root.path) + 1)
    if url ~= self.url then
      if self.root.cache[url] == self then
        self.root.cache[url] = nil
      end
      self.root.cache[self.url] = self
    end
  end
  if stat.mode == 'directory' then
    self.is_dir = true
    self.resourcetype = {xml = 'collection'}
    -- TODO: we could keep the getlastmodified in sync (must change
    -- on add/removal of children and/or subchildren)
    self.getlastmodified  = stat.modification
    -- FIXME: find creation date through lfs ?
    self.creationdate     = self.getlastmodified
  else
    self.is_dir = false
    self.resourcetype     = nil
    self.contenttype      = private.contentType(self)
    self.getcontentlength = stat.size
    self.getlastmodified  = stat.modification
    -- FIXME: find creation date through lfs ?
    self.creationdate     = self.getlastmodified
  end
  return self
end

function lib:body()
  if self.body_cache then
    return self.body_cache
  elseif self.getcontentlength < CACHE_MAX_SIZE then
    self.body_cache = lk.readall(self.path)
    return self.body_cache
  else
    -- TODO: something that reads by chunks... (io.read func wrapper?)
    return lk.readall(self.path)
  end
end

function lib:update(content)
  if self.is_dir then
    return false
  else
    --- FIXME:
    -- Disable write during testing
    -- lk.writeall(self.path, content or '')
    self:sync()
    self:triggerCallbacks('update')
    return true
  end
end

function lib:delete()
  if self.is_dir then
    lk.rmTree(self.path, true)
    local url = self.url
    local len = string.len(url)
    for surl, _ in pairs(self.root.cache) do
      if string.sub(surl, 1, len) == url then
        self.root.cache[surl] = nil
      end
    end
  else
    lk.rmFile(self.path)
    self.root.cache[self.url] = nil
  end
  return true
end

function lib:move(new_path)
  if lk.move(self.path, new_path) then
    self.path = new_path
    self:sync()
    return true
  end
  return false
end

--- LOCK the resource. The lock has a default timeout of
-- FileResource.LOCK_TIMEOUT in ms and is linked to a user. 
--
-- @return 'nil' if the lock cannot be granted (already locked, parent lock)
--          a new lock token on succes
function lib:lock()
end

function lib:unlock()
end

--=============================================== Callbacks
-- These are manually triggered by the controllers (typically
-- the WebDAV controller when it updates files.

function lib:addCallback(op, func, ...)
  local clb = {...}
  table.insert(clb, self)
  local callbacks = self.callbacks[op]
  if not callbacks then
    callbacks = {}
    self.callbacks[op] = callbacks
  end
  table.insert(callbacks, {func, clb})
end

function lib:triggerCallbacks(op)
  local callbacks = self.callbacks[op]
  if callbacks then
    for _, clbk in ipairs(callbacks) do
      clbk[1](unpack(clbk[2]))
    end
  end
end

--=============================================== Children
function lib:children()
  if not self.is_dir or not lk.exist(self.path) then
    return {}
  end
  local list = self.children_list
  if not list then
    -- build and store in cache
    list = {}
    self.children_list = list
    for file in lfs.dir(self.path) do
      -- IGNORE
      if not string.match(file, self.IGNORE) then
        local f = self.url..'/'..file
        table.insert(list, lib(f, self.root))
      end
    end
  end
  return list
end

function lib:createChild(name, body)
  local fullpath = self.path .. '/' .. name
  if body then
    lk.writeall(fullpath, body)
  else
    lk.makePath(fullpath)
  end
  self:sync()
  return lk.FileResource(self.url .. '/' .. name, self.root)
end

function lib:deleteChild(res_or_name)
  local rez 
  if type(res_or_name) == 'string' then
    local url = self.url.. '/' .. res_or_name
    rez = lib(url, self.root)
  else
    rez = res_or_name
  end
  if not rez or rez:delete() then
    self:sync()
    return true
  end
  return false
end

function lib:moveChild(child, dest_parent, dest_name)
  local new_path = dest_parent.path .. '/' .. dest_name
  if child:move(new_path) then
    self:sync()
    dest_parent:sync()
    return true
  end
  return false
end

--=============================================== PRIVATE

local EXT_TO_CONTENT_TYPE = {
  lkp = 'text/yaml',
  yml = 'text/yaml',
  lua = 'text/plain',
}

function private:contentType()
  self.ext = string.match(self.name, '%.([^.]+)$')
  -- stupid ext to type for the moment
  return EXT_TO_CONTENT_TYPE[self.ext] or 'application/binary'
end
