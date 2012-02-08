--[[------------------------------------------------------

  editor.Settings
  ---------------

  Editor settings (preferences). User defined values are
  saved in '.lubyk/editor.lua'.

--]]------------------------------------------------------
require 'lubyk'

local defaults = {
  -- Which views to show on launch.
  show = {
    Patch = true,
  },
  -- Where the prototypes database lives
  prototypes_db = string.format('%s/Documents/Lubyk.db', os.getenv('HOME')),
  -- Paths relative to Lubyk.lib (should not be changed by user)
  prototypes_base_src = {
    lubyk = 'lubyk',
  },
  -- Absolute paths (user setings)
  prototypes_src = {},
  -- Number of files to keep in open recent list
  open_recent_max = 5,

  -- Default size and position for main window
  main_view = {
    x = 50,
    y = 50,
    w = 600,
    h = 400,
  },
}

editor.Settings = lk.Settings('editor', defaults)
