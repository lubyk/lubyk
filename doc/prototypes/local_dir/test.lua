require 'lfs'
local dofile_without_chdir = dofile

-- We rewrite 'dofile' to chdir into the file's directory before loading file
-- Since we have no control on where scripts and lib live, we must use relative
-- paths as much as possible.
function dofile(filepath)
  local cur_path = lfs.currentdir()
  local abs_path = string.format('%s/%s', cur_path, filepath)
  local new_dir  = string.gsub(abs_path, "/[^/]+$", "")

  -- change into the loaded file's directory before reading
  lfs.chdir(new_dir)
  -- load file
  dofile_without_chdir(abs_path)
  -- change back to actual directory
  lfs.chdir(cur_path)
end

print("local_dir/test.lua")
print("dofile('a.lua')")
dofile('a.lua')