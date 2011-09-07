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
 __call = function(lib, path, rootpath)
  if not lk.exist(path) then
    return nil
  end
  local self = {path = path, rootpath = rootpath or path}
  setmetatable(self, lib)
  return self:sync()
end})

-- accessor for WebDAV properties
lib.davProperty = rawget

function lib:sync()
  local stat = lfs.attributes(self.path)
  if not stat then
    -- does not exist (removed)
    return nil
  end
  self.name = string.match(self.path, '/([^/]+)$')
  self.children_list = nil
  self.body_cache    = nil
  if self.path == self.rootpath then
    self.href = '/'
  else
    self.href = string.sub(self.path, string.len(self.rootpath) + 1)
  end
  if stat.mode == 'directory' then
    self.is_dir = true
    self.resourcetype = {xml = 'collection'}
    -- TODO: we could keep the getlastmodified in sync (must change
    -- on add/removal of children and/or subchildren)
  else
    self.is_dir = false
    self.resourcetype     = nil
    self.contenttype      = private.contentType(self)
    self.getcontentlength = stat.size
    self.getlastmodified  = stat.modification
    self.creationdate     = stat.change
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
    lk.writeall(self.path, content or '')
    self:sync()
    return true
  end
end

function lib:delete()
  if self.is_dir then
    lk.rmTree(self.path, true)
  else
    lk.rmFile(self.path)
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

--=============================================== Children
function lib:children()
  if not self.is_dir then
    return {}
  end
  local list = self.children_list
  if not list then
    -- build and store in cache
    list = {}
    for file in lfs.dir(self.path) do
      -- IGNORE
      if not string.match(file, self.IGNORE) then
        local f = self.path..'/'..file
        table.insert(list, lib(f, self.rootpath))
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
  return true
end

function lib:deleteChild(res_or_name)
  local rez 
  if type(res_or_name) == 'string' then
    local fullpath = self.path .. '/' .. res_or_name
    rez = lib(fullpath, self.rootpath)
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
