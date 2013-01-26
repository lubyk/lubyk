--[[------------------------------------------------------

  lk.DavMorph
  -----------

  This is the same as lk.Morph but the service also starts
  a WebDAV server to allow live coding/editing of the
  patch.

--]]------------------------------------------------------

local private = {}

function lk.DavMorph(opts)
  local self = lk.Morph()
  self.openFile = private.openFile
  self.delete   = private.delete
  self.quit     = private.quit
  self.join = function(self)
    self.dav_thread:join()
  end
  opts.info = {}
  self.dav = lk.DavServer()
  opts.info.davport = self.dav.port
  -- Register WebDAV service when it has finished starting
  self.dav:onReady(function()
    self.dav_registration = mdns.Registration('_webdav._tcp', Lubyk.zone .. '.lubyk', self.dav.port)
    -- Start lubyk service and announce Morph.
    self:start(opts)
  end)
  return self
end

--=============================================== PRIVATE
function private:delete()
end

function private:quit()
  self.dav_thread:quit()
  lk.Morph.quit(self)
end

--=============================================== Morph

function private:openFile(filepath)
  -- super
  lk.Morph.openFile(self, filepath)
  -- setup server
  self.dav:setRoot(self.root)
end

function private:quit()
  lk.Morph.quit(self)
  if self.thread then
    self.thread:kill()
  end
end
--=============================================== WebDAV
-- All callbacks are defined by Morph on the FileResource
-- nothing to do here.
