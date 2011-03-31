--[[------------------------------------------------------

  rk
  -----

  This file is loaded just after the load paths have been
  changed and loads Lubyk essentials.

--]]------------------------------------------------------
require 'Autoload'
lk = Autoload('lk')
-- autoload stuff in _G
Autoload.global()

lubyk = Autoload('lubyk')
-------------------------------- CONSTANTS
lubyk.service_type    = '_lubyk._tcp'
-- Get patch basic information
lubyk.info_url        = '/lk/info'
-- Get all patch information at once
lubyk.sync_url        = '/lk/sync'
-- Notification for new service (in-process)
lubyk.add_service_url = '/lk/add_service'
-- Notification for removed service (in-process)
lubyk.rem_service_url = '/lk/rem_service'

-------------------------------- lubyk.core
require 'lubyk.core'

-------------------------------- pack/unpack (msgpack.vendor)
require 'msgpack.vendor'

-------------------------------- lk.findfile
require 'lk.constants'

-------------------------------- lk.readall(path)

function lk.readall(basepath, path)
  if path then
    path = string.format('%s/%s', basepath, path)
  else
    path = basepath
  end
  local f = assert(io.open(path, 'rb'))
  local s = f:read('*all')
  f:close()
  return s
end

-------------------------------- lk.with_filepath(filepath, func)

function lk.with_filepath(filepath, func)
  local cur_path = lfs.currentdir()
  local abs_path = filepath
  if not string.match(abs_path, '^/') then
    abs_path = string.format('%s/%s', cur_path, filepath)
  end
  local new_dir = string.gsub(abs_path, '/[^/]+$', '')
  -- change into the loaded file's directory before reading
  lfs.chdir(new_dir)
  -- load file
  local ok, err = pcall(func)
  -- change back to actual directory
  lfs.chdir(cur_path)
  assert(ok, string.format("%s (with current path '%s')", err or '', new_dir))
end

-------------------------------- lk.split(string, sep)
-- Split a string into elements.
-- Compatibility: Lua-5.1
function lk.split(str, pat)
  local t = {}  -- NOTE: use {n = 0} in Lua-5.0
  local fpat = '(.-)' .. pat
  local last_end = 1
  local s, e, cap = str:find(fpat, 1)
  while s do
    table.insert(t,cap)
    last_end = e+1
    s, e, cap = str:find(fpat, last_end)
  end
  if last_end <= #str then
    cap = str:sub(last_end)
    table.insert(t, cap)
  end
  return t
end

-------------------------------- lk.source()
-- Find the source of the current file or the
-- file up x levels in the call chain (-1 = up
-- one level).
function lk.source(level)
  local level = level or 0
  return debug.getinfo(2 - level).source
end

-------------------------------- lk.file(level = 0)
-- Find the the current file or the
-- file up x levels in the call chain (-1 = up
-- one level).
function lk.file(level)
  local level = level or 0
  return string.match(lk.source(level - 1), '^@(.*)$')
end

-------------------------------- lk.dir()
-- Find the directory of the current file or the
-- direcotyr of the file up x levels in the call
-- chain (-1 = up one level).
function lk.dir(level)
  local level = level or 0
  return string.gsub(lk.file(level - 1), '/[^/]+$', '')
end

-------------------------------- lk.file_type(path)
-- Test for file/directory existence and/or type.
function lk.file_type(filepath)
  if not filepath then return nil end
  local attrs = lfs.attributes(filepath)
  return attrs and attrs.mode
end

lk.exist = lk.file_type

-------------------------------- lk.dofile(path)
-- Load a file relative to the current file.
function lk.dofile(path)
  return dofile(lk.dir(-1) .. '/' .. path, path)
end

-------------------------------- lk.findcode(class_name)
-- Find source code from a given class name of the
-- form 'lubyk.Metro' or 'my.super.complicated.Thing'. The
-- loader searches locally first and then into package.paths.
function lk.findcode(basedir, class_name)
  if not class_name then
    class_name = basedir
    basedir = lfs.currentdir()
  end
  -- FIXME: Beware to only optimize this by storing fullpaths
  local path = string.gsub(class_name, '%.', '/') .. '.lua'
  local fullpath = basedir .. '/' .. path
  if lk.file_type(fullpath) == 'file' then
    -- Found local file relative to patch
    return lk.readall(fullpath)
  else
    -- Search in lib paths
    local file = lk.findfile(class_name, 'path')
    if file then
      return lk.readall(file)
    else
      return nil
    end
  end
end