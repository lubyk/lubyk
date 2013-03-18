--[[------------------------------------------------------

  # Directory helper

  This is a helper to access/glob directory trees. It requires
  'lfs' (lua filesystem).

--]]------------------------------------------------------
require 'lfs'

local lib = class('lk.Dir', {
  sep = '/',
  ignore_pattern = '^[.]'
})

-- # Class functions

-- Create a new directory helper pointing at @path.
function lib.new(path)
  local self = {path = path}
  return setmetatable(self, lib)
end

local function glob_list(base, pattern)
  for file in lfs.dir(base) do
    if not string.match(file, lib.ignore_pattern) then
      local fullpath = base..lib.sep..file
      local attrs = lfs.attributes(fullpath)
      if attrs and attrs.mode == 'file' then
        if not pattern or string.match(fullpath, pattern) then
          coroutine.yield(fullpath)
        end
      elseif attrs and attrs.mode == 'directory' then
        glob_list(fullpath, pattern)
      end
    end
  end
end

-- # Methods

-- Return an iterator to recursively find files matching @pattern@ in the
-- directory. The pattern syntax is the same as string.match.
--
--   -- Find files ending in ".lua".
--   for file in lk.Dir('lib'):glob '%.lua$' do
--     print(file)
--   end
--   --> lib/lk/Dir.lua
--   --> lib/lk/Doc.lua
--   --> ...
function lib:glob(pattern)
  local co = coroutine.create(glob_list)
  --glob_list(self.path, pattern, list)
  return function()
    local ok, value = coroutine.resume(co, self.path, pattern)
    if ok then
      return value
    else
      return nil
    end
  end
end

local function list_files(self)
  local base = self.path
  for file in lfs.dir(base) do
    if not string.match(file, self.ignore_pattern) then
      coroutine.yield(base..self.sep..file)
    end
	end
end

-- Return an iterator over the paths in the directory. The returned values are
-- paths, not just filenames.
--
--   for file in lk.Dir('lib'):list() do
--     print(file)
--   end
--   --> lib/lk
--   --> lib/lk.lua
function lib:list()
  local co = coroutine.create(list_files)
  return function()
    local ok, value = coroutine.resume(co, self)
    if ok then
      return value
    else
      return nil
    end
  end
end

-- Return true if there is at least one child in the directory that matches
-- @pattern@.
function lib:contains(pattern)
  for file in lfs.dir(self.path) do
    if string.match(file, pattern) then
      return true
    end
	end
  return false
end

