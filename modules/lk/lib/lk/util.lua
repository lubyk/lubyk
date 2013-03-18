--[[------------------------------------------------------

  # Lubyk utilities

  Miscellanous utility functions in *lk* module.

--]]------------------------------------------------------
local lib     = lk
local private = {}

local spawn = lib.spawn

-- # Script

-- Find the source of the current file or the file up x levels in the call
-- chain (-1 = up one level).
function lib.scriptSource(level)
  local level = level or 0
  return debug.getinfo(2 - level).source
end

-- Find the path of the current script or the script up x levels in the call
-- chain (-1 = up one level). The path is relative to the current directory.
--
--   -- get path of running script
--   local my_path = lk.scriptPath()
--   --> 'test/lk_test.lua'
-- 
-- If you need an absolute path, use #absolutizePath.
function lib.scriptPath(level)
  local level = level or 0
  return string.match(lib.scriptSource(level - 1), '^@(.*)$')
end

-- Find the directory of the current script or the directory of the script up x
-- levels in the call chain (-1 = up one level). This is the same as #scriptPath
-- but it returns the parent directory.
function lib.scriptDir(level)
  local level = level or 0
  local file = lib.scriptPath(level - 1)
  assert(file, "Cannot use lk.scriptDir here because of a tail call optimization.")
  if string.match(file, '/') then
    return string.gsub(lib.scriptPath(level - 1), '/[^/]+$', '')
  else
    return '.'
  end
end

-- Declare a method as being deprecated. This should be used when method names
-- are changed to avoid breaking code without warnings. The syntax is:
--
--   function lib.badName(...)
--     return lib.deprecation('lk', 'badName', 'betterName', ...)
--   end
-- 
-- When someone uses the "badName" method, a deprecation warning is printed:
--
--   [DEPRECATION] {traceback}
--       'lk.badName' is deprecated. Please use 'lk.betterName' instead.
function lib.deprecation(lib_name, old, new, ...)
  local trace = lib.split(debug.traceback(), '\n\t')[4]
  local arg = ...
  if arg then
    print(string.format("[DEPRECATION] %s\n\t'%s.%s' is deprecated. Please use '%s.%s' instead.", trace, lib_name, old, lib_name, new))
    return _G[lib_name][new](...)
  else
    print(string.format("[DEPRECATION] %s\n\t'%s.%s' is deprecated and will be removed. Please use '%s' instead.", trace, lib_name, old, new))
  end
end

-- # Filesystem

-- Test for file/directory existence at @path@. If the element exists, returns
-- the file type such as *'directory'* or *'file'*. Returns nil if there is
-- nothing at the given path.
function lib.fileType(path)
  if not path then return nil end
  local attrs = lfs.attributes(path)
  return attrs and attrs.mode
end

-- Return true if the file or folder pointed by @path@ exists.
-- function lib.exist(path)
lib.exist = lib.fileType

-- nodoc
function lib.readAll(...)
  return lib.deprecation('lk', 'readAll', 'content', ...)
end

-- Return the content of a file as a lua string (not suitable for very long
-- content where parsing the file chunks by chunks is better). The method
-- accepts either a single @path@ argument or a @basepath@ and relative @path@.
function lib.content(basepath, path)
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

-- Write @data@ to @filepath@, creating path folders if necessary. If
-- @check_diff@ is true, only write if the content has changed to avoid
-- changing stat information on the file.
function lib.writeall(filepath, data, check_diff)
  -- get base directory and build components if necessary
  lib.makePath(lib.pathDir(filepath))
  if check_diff and lib.exist(filepath) then
    if data == lib.content(filepath) then
      return true
    end
  end
  local f = assert(io.open(filepath, 'wb'))
  local s = f:write(data)
  f:close()
  return s
end

-- Build necessary folders to create a given @path@.
function lib.makePath(path)
  local fullpath = lib.absolutizePath(path)
  private.makePathPart(fullpath, fullpath)
end

-- Move a file or directory from @path@ to @new_path@. This is like "os.rename"
-- but it also builds necessary path components in new_path.
function lib.move(path, new_path)
  local base = lib.pathDir(new_path)
  lib.makePath(base)
  return os.rename(path, new_path)
end

-- Delete the file located at @path@. Does nothing if the element at @path@ does
-- not exist (already removed).
function lib.rmFile(path)
  local typ = lib.fileType(path)
  if not typ then return end
  assert(typ == 'file', string.format("Cannot remove '%s': it is a %s.", path, typ))
  os.remove(path)
end

-- Remove the directory at @path@. If @recursive@ is true, remove recursively.
-- *This is a dangerous method* if the source path is not ensured to be exempt
-- of mistakes.
function lib.rmTree(path, recursive)
  local fullpath = lib.absolutizePath(path)
  if not lib.exist(fullpath) then
    return true
  end
  if fullpath ~= '/' and fullpath ~= '' then
    -- bug paranoia
    if not recursive then
      return lfs.rmdir(fullpath)
    else
      local code = string.format('rm -r %s', lib.shellQuote(fullpath))
      if os.execute(code) == 0 then
        return true
      else
        return false
      end
    end
  end
end

-- Return the parent folder and filename from a @filepath@. Usage:
--
--   local base, file = lk.pathDir(filepath)
-- 
-- This can also be used on urls.
function lib.pathDir(filepath)
  local base, file = string.match(filepath, '(.*)/(.*)$')
  if not base then
    return '.', filepath
  elseif base == '' then
    -- '/' == root ?
    return '/', file
  else
    return base, file
  end
end

-- Return an absolute path from a @path@ and optional @basepath@. If basepath is
-- not provided, the method uses the current directory. This method
-- resolves @/../@ and @/./@ in paths and returns a clean path. Can also be
-- used with urls.
--
-- For example:
--
--   local abs_path = lk.absolutizePath('foo/bar/../baz', '/home')
--   --> '/home/foo/baz'
function lib.absolutizePath(path, basepath)
  if not string.match(path, '^/') then
    path = string.format('%s/%s', basepath or lfs.currentdir(), path)
  end
  -- resolve '/./' and '/../'
  local parts = lib.split(path, '/')
  local path = {}
  for i, part in ipairs(parts) do
    if part == '.' then
      -- ignore
    elseif part == '..' then
      -- move back
      -- 1 = '', 2 = 'xxx', 3 = '..' ==> 1 = ''
      if i > 2 then
        table.remove(path, #path)
      end
    else
      table.insert(path, part)
    end
  end
  return lib.join(path, '/')
end

-- Transform an absolute path or url to a relative path with given base. For 
-- example:
--
--   local rel = lk.absToRel('/home/foo/bar', '/home/foo/baz')
--   --> '/home/foo/bar'
--   local rel = lk.absToRel('/home/foo/bar', '/home/foo')
--   --> 'bar'
function lib.absToRel(abs_string, base)
  local l = string.len(base)
  local s = string.sub(abs_string, 1, l)
  if s == base then
    if base == abs_string then
      return abs_string
    else
      return string.sub(abs_string, l+2, -1)
    end
  else
    return abs_string
  end
end

-- # Strings, Arrays

-- Removes white spaces at the beginning and end of the string @str@.
function lib.strip(str)
  return string.match(str, '^[ \t\n\r]*(.-)[ \t\n\r]*$')
end

-- Split a string @str@ into elements separated by the pattern @pat@. The
-- function returns a table.
--
--   local t = lk.split('foo/bar/baz', '/')
--   --> {'foo', 'bar', 'baz'}
function lib.split(str, pat)
  local t = {}  -- NOTE: use {n = 0} in Lua-5.0
  if not pat then
    local i = 1
    while true do
      local s = string.sub(str, i, i)
      if s == '' then
        break
      else
        table.insert(t, s)
      end
      i = i + 1
    end
  else
    local fpat = '(.-)' .. pat
    local last_end = 1
    local s, e, cap = string.find(str,fpat, 1)
    while s do
      table.insert(t,cap)
      last_end = e+1
      s, e, cap = str:find(fpat, last_end)
    end
    if last_end <= #str then
      cap = str:sub(last_end)
      table.insert(t, cap)
    end
  end
  return t
end

-- Join elements of a table @list@ with separator @sep@. Returns a string.
--
--   local x = lk.join({'foo', 'bar', 'baz'}, '.')
--   --> 'foo.bar.baz'
function lib.join(list, sep)
  local res = nil
  for _, part in ipairs(list) do
    if not res then
      res = part
    else
      res = res .. sep .. part
    end
  end
  return res or ''
end

-- Insert @elem@ into a @list@, keeping entries in "list" sorted. If elem is not
-- a string, the @elem[key]@ is used to get a string for sorting.
function lib.insertSorted(list, elem, key)
  local pos = -1
  for i, n in ipairs(list) do
    local a, b = n, elem
    if key then
      a = a[key]
      b = b[key]
    end
    if a > b then
      pos = i
      break
    end
  end
  if pos == -1 then
    table.insert(list, elem)
  else
    table.insert(list, pos, elem)
  end
end

local function deepMerge(base, key, value)
  local base_v = base[key]
  if type(value) == 'table' then
    if not base_v then
      base[key] = value
      return true
    else
      -- merge
      local changed = false
      for k, v in pairs(value) do
        changed = deepMerge(base_v, k, v) or changed
      end
      return changed
    end
  elseif base_v == value then
    -- nothing changed
    return false
  else
    base[key] = value
    return true
  end
end
lib.deepMerge = deepMerge

-- Deep merge @value@ into @base@ at @key@. Returns true if *base* has been
-- changed. For example:
--
--   local base = {a = { b = 4, c = 5 }}
--   lk.deepMerge(base, 'a', { c = 5 })
--   --> false (nothing changed)
--
--   lk.deepMerge(base, 'a', { c = 6 })
--   --> true, base = {a = { b = 4, c = 6 }}
-- function lib.deepMerge(base, key, value)
-- # Debugging

local orig_req = require
-- Print calls to @require@. This can be used to list all code dependencies.
-- Usage:
--
--   lk.traceRequire()
--   -- From now on, all require statements are printed.
-- 
-- If you simply want to make sure no require is not called to autoload code
-- after some point, you should use [Autoload.strict](Autoload.html#strict).
function lib.traceRequire()
  require = function(path)
    print("require '"..path.."'")
    return orig_req(path)
  end
end

-- Print a log message with the current file name and line. This is better then
-- using "print" because it gives us some information on the context of the 
-- print statement.
--
--   lk.log('foo')
--   --> lk/util.lua:426:  foo
function lib.log(...)
  local trace = lib.split(debug.traceback(), '\n\t')[3]
  local file, line = string.match(trace, '^([^:]+):([^:]+):')
  if file and line then
    print(string.format('%s:%i:', file, line), ...)
  else
    print(trace, ...)
  end
end


-- # Miscellanous

-- nodoc
function printf(format, ...)
  return lib.deprecation('lk', 'printf', 'print', string.format(format, ...))
end

lib.print = print

-- Start a new process with @code@, encoding arguments with yaml.
--
-- Here is an example that passes a table of arguments.
--   
--   local pid = lk.spawn([[
--     require 'lubyk'
--     lk.Doc.make(%s)
--   ]], {target = 'doc', sources = 'lib', format = 'html'})
function lib.spawn(code, ...)
  local dump = yaml.dump(...)
  local sep = ''
  -- Make sure the literal string in the yaml dump is not finished too soon by the
  -- text in the dump.
  while dump:match(string.format('%%]%s%%]', sep)) do
    sep = sep .. '='
  end

  local args = string.format("yaml.load [%s[\n%s\n]%s]", sep, dump, sep)
  return spawn(string.format(code, args))
end

-- Quote string @str@ so that it can be inserted in a shell command. Example:
--
--   os.execute(string.format('latex %s', lk.shellQuote(filepath)))
function lib.shellQuote(str)
  str = string.gsub(str, '\\', '\\\\')
  return '"' .. string.gsub(str, '"', '\\"') .. '"'
end

function private.makePathPart(path, fullpath)
  local file_type = lib.fileType(path)
  if file_type == 'file' then
    error(string.format("Could not build path '%s' ('%s' is a file).", fullpath, path))
  elseif file_type == 'directory' then
    return -- done
  else
    local base = lib.pathDir(path)
    private.makePathPart(base, fullpath)
    -- base should exist or an error has been raised
    lfs.mkdir(path)
    -- done
  end
end

