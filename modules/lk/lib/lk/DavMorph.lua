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
  self.quit     = private.quit
  self.join = function(self)
    self.dav_thread:join()
  end
  if filepath then
    self:openFile(filepath)
  end
  return self
end

--=============================================== PRIVATE

--=============================================== Morph

function private:openFile(filepath)
  -- super
  lk.Morph.openFile(self, filepath)
  -- setup server
  if self.dav then
    self.dav:setRoot(self.root)
  else
    self.dav = lk.DavServer(Lubyk.dav_port, self.root)
    self.dav_thread = lk.Thread(function()
      self.dav:listen()
    end)
  end
end

function private:quit()
  lk.Morph.quit(self)
  if self.dav_thread then
    self.dav_thread:kill()
  end
end
--=============================================== WebDAV
-- All callbacks are defined by Morph on the FileResource
-- nothing to do here.
