--[[------------------------------------------------------

  lk.util
  -------

  Loads utility functions inside 'lk'.

--]]------------------------------------------------------

--- Return the parent folder and filename from a filepath.
function lk.pathDir(filepath)
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

--- Read all the content from a given path (or basepath and path).
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

function lk.absolutizePath(path, base)
  if not string.match(path, '^/') then
    path = string.format('%s/%s', base or lfs.currentdir(), path)
  end
  -- resolve '/./' and '/../'
  local parts = lk.split(path, '/')
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
  return lk.join(path, '/')
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
lk.deepMerge = deepMerge

local function makePathPart(path, fullpath)
  local file_type = lk.fileType(path)
  if file_type == 'file' then
    error(string.format("Could not build path '%s' ('%s' is a file).", fullpath, path))
  elseif file_type == 'directory' then
    return -- done
  else
    local base = lk.pathDir(path)
    makePathPart(base, fullpath)
    -- base should exist or an error has been raised
    lfs.mkdir(path)
    -- done
  end
end

--- Build necessary folders to create the given path.
function lk.makePath(path)
  local fullpath = lk.absolutizePath(path)
  makePathPart(fullpath, fullpath)
end

local function shellQuote(str)
  str = string.gsub(str, '\\', '\\\\')
  return '"' .. string.gsub(str, '"', '\\"') .. '"'
end

lk.shellQuote = shellQuote

--- Remove a directory recursively
function lk.rmTree(path, recursive)
  local fullpath = lk.absolutizePath(path)
  if not lk.exist(fullpath) then
    return true
  end
  if fullpath ~= '/' and fullpath ~= '' then
    -- bug paranoia
    if not recursive then
      return lfs.rmdir(fullpath)
    else
      local code = string.format('rm -r %s', shellQuote(fullpath))
      if os.execute(code) == 0 then
        return true
      else
        return false
      end
    end
  end
end

-- Move a file or directory
function lk.move(path, new_path)
  local base = lk.pathDir(new_path)
  lk.makePath(base)
  return os.rename(path, new_path)
end

function lk.rmFile(path)
  if lk.fileType(path) == 'file' then
    os.remove(path)
  end
end

--- Write data to a filepath, creating path folder if necessary. If check_diff
-- is true, only write if the content has changed.
function lk.writeall(filepath, data, check_diff)
  -- get base directory and build components if necessary
  lk.makePath(lk.pathDir(filepath))
  if check_diff and lk.exist(filepath) then
    if data == lk.readall(filepath) then
      return true
    end
  end
  local f = assert(io.open(filepath, 'wb'))
  local s = f:write(data)
  f:close()
  return s
end

-------------------------------- lk.withDirectory(directory, func)

function lk.withDirectory(new_dir, func)
  local cur_path = lfs.currentdir()
  local abs_path = lk.absolutizePath(new_dir)
  -- change into the loaded file's directory before reading
  lfs.chdir(abs_path)
  -- load file
  local ok, err = pcall(func)
  -- change back to actual directory
  lfs.chdir(cur_path)
  assert(ok, string.format("%s (with current path '%s')", err or '', abs_path))
end

-------------------------------- lk.strip(string)
-- Removes white spaces at the beginning and end of the string.
function lk.strip(str)
  return string.match(str, '^[ \t\n\r]*(.-)[ \t\n\r]*$')
end

-------------------------------- lk.split(string, sep)
-- Split a string into elements.
-- Compatibility: Lua-5.1
function lk.split(str, pat)
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

-------------------------------- lk.join(list, sep)
-- Join elements of a table with a separator
function lk.join(list, sep)
  local res = nil
  for _, part in ipairs(list) do
    if not res then
      res = part
    else
      res = res .. sep .. part
    end
  end
  return res
end

-------------------------------- lk.insertSorted(table, string, [key])
-- Insert a string into a table, keeping entries sorted.
function lk.insertSorted(list, str, key)
  local pos = -1
  for i, n in ipairs(list) do
    local a, b = n, str
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
    table.insert(list, str)
  else
    table.insert(list, pos, str)
  end
end

-------------------------------- lk.absToRel(abs_string, base)
-- Transform an absolute url to a relative with given base
function lk.absToRel(abs_string, base)
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

-------------------------------- lk.scriptSource()
-- Find the source of the current file or the
-- file up x levels in the call chain (-1 = up
-- one level).
function lk.scriptSource(level)
  local level = level or 0
  return debug.getinfo(2 - level).source
end

-------------------------------- lk.scriptPath(level = 0)
-- Find the the current file or the
-- file up x levels in the call chain (-1 = up
-- one level).
function lk.scriptPath(level)
  local level = level or 0
  return string.match(lk.scriptSource(level - 1), '^@(.*)$')
end

-------------------------------- lk.scriptDir()
-- Find the directory of the current file or the
-- directory of the file up x levels in the call
-- chain (-1 = up one level).
function lk.scriptDir(level)
  local level = level or 0
  local file = lk.scriptPath(level - 1)
  if string.match(file, '/') then
    return string.gsub(lk.scriptPath(level - 1), '/[^/]+$', '')
  else
    return '.'
  end
end

-------------------------------- lk.fileType(path)
-- Test for file/directory existence and/or type.
function lk.fileType(filepath)
  if not filepath then return nil end
  local attrs = lfs.attributes(filepath)
  return attrs and attrs.mode
end

lk.exist = lk.fileType

-------------------------------- lk.dofile(path)
-- Load a file relative to the current file.
function lk.dofile(path)
  return dofile(lk.scriptDir(-1) .. '/' .. path, path)
end

-------------------------------- lk.findCode(class_name)
-- Find source code from a given class name of the
-- form 'lubyk.Metro' or 'my.super.complicated.Thing'. The
-- loader searches locally first and then into package.paths.
-- 
-- TODO: REMOVE
function lk.findCode(basedir, class_name)
  assert(false, "lk.findCode will be removed")
  if not class_name then
    class_name = basedir
    basedir = lfs.currentdir()
  end
  -- FIXME: Beware to only optimize this by storing fullpaths
  local path = string.gsub(class_name, '%.', '/') .. '.lua'
  local fullpath = basedir .. '/lib/' .. path
  if lk.fileType(fullpath) == 'file' then
    -- Found local file relative to patch
    return lk.readall(fullpath)
  else
    -- Search in lib paths
    local file = lk.findFile(class_name)
    if file then
      return lk.readall(file)
    else
      return nil
    end
  end
end

-------------------------------- printf(format, ...)
-- Print with formating
function printf(format, ...)
  print(string.format(format, ...))
end
