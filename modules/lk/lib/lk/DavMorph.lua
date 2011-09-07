--[[------------------------------------------------------

  lk.DavMorph
  -----------

  This is the same as lk.Morph but the service also starts
  a WebDAV server to allow live coding/editing of the
  patch.

--]]------------------------------------------------------

local private = {}

function lk.DavMorph(...)
  local self = lk.Morph(...)
  private.setupDav(self)
  return self
end

--=============================================== PRIVATE

function private.setupDav(self)
  self.dav = lk.DavServer(Lubyk.dav_port)
  self.dav.morph = self
  for _, k in ipairs({'find', 'findChildren', 'create', 'update', 'delete'}) do
    self.dav[k]= private[k]
  end
  self.openFile = private.openFile
end

--=============================================== WebDAV
function private:find(path)
  error("'find(path)' callback not implemented for DAVServer")
end

function private:findChildren(resource)
  error("'findChildren(resource)' callback not implemented for DAVServer")
end

function private:create(path, content)
  -- forbidden
  return nil, {status = "403"}
end

function pribate:update(resource, content)
  -- forbidden
  return nil, {status = "403"}
end

function private:delete(resource)
  error("'delete(resource)' callback not implemented for DAVServer")
end

--=============================================== Morph

function private:openFile(path)
  -- super
  lk.Morph.open(self, path)
  -- setup server
  self.dav.dir = self.dir
  -- share cache
  self.dav.cache = self.cache
end
