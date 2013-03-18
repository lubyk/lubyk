require 'lubyk'
require 'debug'

test = {
  suites = {},
  file_count = 0,
  TIMEOUT    = 35,
  only       = false,
  verbose    = false,
  abort      = false,
  coverage   = false,
}
-- We do not use Autoload because the "test" global is used to store global
-- values like "test.memory" or whatnot. Maybe this should be fixed in projects
-- using testing.
require 'test.Suite'

local lib = test

function lib:testWithUser()
  return lib.UserSuite(self._info.name)
end

function lib:timeout(timeout, func)
  -- if not self._suite._info.user_suite then
  --   printf("Using timeout without user in %s (%s).", self._suite._info.name, self._name)
  -- end
  if not func then
    func = timeout
    timeout = self.TIMEOUT
  end
  local start = elapsed()
  while not func(elapsed() - start) and elapsed() < start + timeout do
    sleep(0.3)
  end
end

--- Test suite requiring user interaction/visual feedback. These
-- tests are turned off when running more then a single file.
function lib.UserSuite(name)
  local suite = {_info = {name = name .. '[ux]', tests = {}, errors = {}, user_suite = true}}
  -- this is to enable syntax like: withUser.should.receiveClick()
  suite.should = suite
  setmetatable(suite, lib)
  table.insert(lib.suites, suite)
  -- default setup and teardown functions
  suite.setup    = function() end
  suite.teardown = function() end
  return suite
end

function lib.all()
  lib.parseArgs()
  lib.total_exec = 0
  lib.total_count = 0
  lib.total_asrt = 0
  lib.total_fail = 0
  sched:run(function()
    for i, suite in ipairs(lib.suites) do
      suite:test(true) 
      if lib.abort then
        break
      end
    end
    lib.report()
    sched:quit()
  end)
end

function lib.files(list_or_path, pattern, reject)
  lib.parseArgs()
  -- first disable test.all()
  local all = lib.all
  lib.all = function() end
  -- load all files
  if type(list_or_path) == 'string' then
    pattern = pattern or '%_test.lua$'
  end
  local list = {}
  if pattern then
    local dir = lk.Dir(list_or_path)
    for file in dir:glob(pattern) do
      if reject and string.match(file, reject) then
        -- skip
      else
        table.insert(list, file)
      end
    end
  else
    list = list_or_path
  end
  for _, file in ipairs(list) do
    lib.file_count = lib.file_count + 1
    dofile(file)
  end
  all()
end

--- FIXME This is too dependent on lubyk !
function lib.loadAll(...)
  local arg = {...}
  if not arg[1] then
    -- load all
    for mod in lk.Dir('modules'):list() do
      if lk.fileType(mod) == 'directory' then
        for file in lk.Dir(mod):glob('test/.+_test[.]lua$') do
          lib.file_count = lib.file_count + 1
          dofile(file)
        end
      end
    end
  else
    for _, mod in ipairs(arg) do
      if not string.match(mod, '^modules/') then
        mod = 'modules' .. lk.Dir.sep .. mod
      end
      if lk.fileType(mod) == 'directory' then
        for file in lk.Dir(mod):glob('test/.+_test[.]lua$') do
          lib.file_count = lib.file_count + 1
          dofile(file)
        end
      else
        print(string.format("Module '%s' is not a directory.", mod))
      end
    end
  end
end

-- Prints traceback on error
function lib.trace(fun, ...)
  local some_args = {...}
  local n = select("#", ...)
  local function runner()
    return fun(unpack(some_args, 1, n))
  end
  local res = { xpcall(runner, debug.traceback) }
  if res[1] == true then
    table.remove(res, 1)
    return unpack(res)
  else
    print(res[2])
    return nil
  end
end

function lib.report()
  print('\n')
  if test.only then
    print('Only testing \''..test.only..'\'.')
  end

  if lib.total_exec == 0 then
    print(string.format('No tests defined. Test files must end with "_test.lua"'))
  elseif lib.abort then
    print(string.format('Abort after %i / %i tests', lib.total_exec, lib.total_count))
  elseif lib.total_fail == 0 then
    if lib.total_exec == 1 then
      print(string.format('Success! %i test passes (%i assertions).', lib.total_exec, lib.total_asrt))
    else
      print(string.format('Success! %i tests pass (%i assertions).', lib.total_exec, lib.total_asrt))
    end
  elseif lib.total_exec == 1 then
    if lib.total_fail == 1 then
      print(string.format('Fail... %i failure / %i test', lib.total_fail, lib.total_exec))
    else
      print(string.format('Fail... %i failures / %i test', lib.total_fail, lib.total_exec))
    end
  else
    if lib.total_fail == 1 then
      print(string.format('Fail... %i failure / %i tests', lib.total_fail, lib.total_exec))
    else
      print(string.format('Fail... %i failures / %i tests', lib.total_fail, lib.total_exec))
    end
  end
  print('')
end

function lib.parseArgs()
  for _, val in pairs(arg) do
    if val == '--verbose' then
      lib.verbose = true
    else
      local key,value = string.match(val, '%-%-(.-)=(.*)')
      if key == 'only' then
        lib.only = value
      end
    end
  end
end

