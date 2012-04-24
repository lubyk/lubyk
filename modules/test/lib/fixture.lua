local lib = {}
fixture = lib

function lib.readall(test_file)
  return lk.readall(lib.path(test_file, lk.scriptDir(-1)))
end

function lib.path(test_file, basepath)
  if test_file then
    return string.format('%s/fixtures/%s', basepath or lk.scriptDir(-1), test_file)
  else
    return string.format('%s/fixtures', basepath or lk.scriptDir(-1))
  end
end