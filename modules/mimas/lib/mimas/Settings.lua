--[[------------------------------------------------------

  mimas.Settings
  --------------

  Default settings for mimas.

--]]------------------------------------------------------

local defaults = {
  -- Quit application when the last window is closed. The
  -- default value is set in Lubyk so that lk.Process can
  -- alter this value without loading mimas.
  quit_on_close = Lubyk.mimas_quit_on_close,
}

mimas.Settings = lk.Settings('mimas', defaults)
