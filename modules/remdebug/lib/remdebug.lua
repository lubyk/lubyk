--[[------------------------------------------------------

  remdebug
  --------


  RemDebug 1.0 Beta
  Copyright Kepler Project 2005 (http://www.keplerproject.org/remdebug)

  In Lubyk, we have done some slight modifications to let users use
  remdebug() instead of remdebug.engine.start() and to start the controller
  with remdebug.controller().

--]]------------------------------------------------------
remdebug = Autoload('remdebug')
local mt   = {}
mt.__index = Autoload.__index
setmetatable(remdebug, mt)

mt.__call = remdebug.engine.start
--function mt.__call(lib)
--  return lib.engine.start()
--end

