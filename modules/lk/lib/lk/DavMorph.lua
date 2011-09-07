--[[------------------------------------------------------

  lk.DavMorph
  -----------

  This is the same as lk.Morph but the service also starts
  a WebDAV server to allow live coding/editing of the
  patch.

--]]------------------------------------------------------

local private = {}

function lk.DavMorph(filepath)
  local self = lk.Morph()
  self.openFile = private.openFile
  self.delete   = private.delete
  if filepath then
    self:openFile(filepath)
  end
  return self
end

--=============================================== PRIVATE

--=============================================== Morph

function private:openFile(filepath)

  print("DAV")
  -- super
  lk.Morph.openFile(self, filepath)
  -- setup server
  if self.dav then
    self.dav:setRoot(self.root)
  else
    print("Creating DavServer")
    self.dav = lk.DavServer(Lubyk.dav_port, self.root)
    self.dav_thread = lk.Thread(function()
      print(string.format("Starting server on port %i...\nConnect with: http://localhost:%i", self.dav.port, self.dav.port))
      self.dav:listen()
      print('DONE?')
    end)
  end
  -- share cache
  self.dav.cache = self.resources
end

--=============================================== WebDAV
-- All callbacks are defined by Morph on the FileResource
-- nothing to do here.

function private:delete(parent, resource)
  -- refuse delete (or we loose Morph link)
  return nil, {status = '400'}
end
