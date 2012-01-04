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
  self:start(opts)
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
  if self.dav then
    self.dav:setRoot(self.root)
  else
    -- try 5 times (now, 2s, 4s, 8s, 16s)
    self.start = sched:try(function()
      self.dav = lk.DavServer(Lubyk.dav_port, self.root)
      self.start = nil
    end, function(err)
      print("Could not start DavServer", err)
      sched:quit()
    end)
  end
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
