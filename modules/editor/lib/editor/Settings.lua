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
    Library = true,
    Patch = true,
    View = true,
    Controls = true,
  },
  -- Where the databases (prototypes, controls) lives
  db_path = Lubyk.lib .. '/Lubyk.db',
  -- Paths relative to Lubyk.lib (should not be changed by user)
  prototype_base_src = {
    '_prototype',
  },
  -- Paths relative to Lubyk.lib (should not be changed by user)
  control_base_src = {
    '_control',
  },
  -- Absolute paths (user setings)
  prototype_src = {},
  -- Number of files to keep in open recent list
  open_recent_max = 4,

  -- Default size and position for main window
  main_view = {
    x = 50,
    y = 50,
    w = 787,
    h = 611,
  },
  -- Default size and position for log window
  log_view = {
    x = 50,
    y = 50,
    w = 120,
    h = 200,
  },
  log_show = {
    info = true,
    warn = true,
    error = true,
  },
  logo = Lubyk.lib .. '/editor/img/logo.png',
}

editor.Settings = lk.Settings('editor', defaults)
