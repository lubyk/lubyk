require 'debug'

local lib = {suites = {}, file_count = 0, TIMEOUT = 35}
lib.__index = lib
test = lib

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

function lib.Suite(name)
  local suite = {_info = {name = name, tests = {}, errors = {}}}
  setmetatable(suite, lib)
  table.insert(lib.suites, suite)
  -- default setup and teardown functions
  suite.setup    = function() end
  suite.teardown = function() end
  return suite
end

-- This is to execute the tests in creation order.
function lib:__newindex(key, value)
  rawset(self, key, value)
  if type(value) == 'function' and
    key ~= 'setup' and
    key ~= 'teardown' and
    key ~= 'should' then
    table.insert(self._info.tests, {key, value})
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
      lib.runSuite(suite)
      lib.reportSuite(suite)
      if test.abort then
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

function lib.runSuite(suite)
  local exec_count = 0
  local fail_count = 0
  local skip_count = 0
  local errors = suite._info.errors
  local test_var
  local test_func
  local function pass_args() return test_func(test_var) end
  lib.current_suite = suite
  suite._info.assert_count = 0
  -- list of objects protected from gc
  suite._info.gc_protect = {}
  local gc_protect = suite._info.gc_protect
  -- run all tests in the current file
  local skip = suite._info.user_suite and lib.file_count > 1
  for i,e in pairs(suite._info.tests) do
    local name, func = unpack(e)
    test_var = setmetatable({}, lib)
    test_var._name  = name
    test_var._suite = suite
    gc_protect[name] = test_var
    test_func = func
    if skip or (lib.only and lib.only ~= name) then
      -- skip user tests
      skip_count = skip_count + 1
    else
      exec_count = exec_count + 1
      suite.setup(gc_protect[name])
      if lib.verbose then
        printf("%-12s Run %s", '['..suite._info.name..']', name)
      end
      local ok, err = sched:pcall(pass_args)
      if lib.verbose then
        printf("%s %s", ok and 'OK' or 'FAIL', err or '')
      end
      collectgarbage('collect')
      if not ok then
        fail_count = fail_count + 1
        -- Get line and message for assertPass, assertError
        table.insert(errors, {i, name, err})
      end
      suite.teardown(gc_protect[name])
      if test.abort then
        break
      end
    end
  end

  suite._info.exec_count = exec_count
  suite._info.total_count = #suite._info.tests
  suite._info.fail_count = fail_count
  suite._info.skip_count = skip_count
end

function lib.reportSuite(suite)
  local ok_message, skip_message = '', ''
  if suite._info.fail_count == 0 then
    ok_message = 'OK'
  else
    ok_message = string.format('%i Failure(s)', suite._info.fail_count)
  end
  local exec_count = suite._info.exec_count
  if suite._info.skip_count > 0 then
    if exec_count == 0 then
      ok_message = '-- skip'
    else
      skip_message = string.format(' : skipped %i', suite._info.skip_count)
    end
  end
  print(string.format('==== %-28s (%2i test%s%s): %s', suite._info.name, exec_count, exec_count > 1 and 's' or ' ', skip_message, ok_message))
  lib.total_exec = lib.total_exec + suite._info.exec_count
  lib.total_count = lib.total_count + suite._info.total_count
  lib.total_asrt = lib.total_asrt + suite._info.assert_count
  if suite._info.fail_count > 0 then
    for _, e in ipairs(suite._info.errors) do
      local i, name, err = unpack(e)
      lib.total_fail = lib.total_fail + 1
      local hname = string.gsub(name, '([A-Z])', function(x) return ' '..string.lower(x) end)
      print(string.format('  %i. Should %s\n     %s\n', i, hname, string.gsub(err, '\n', '\n     ')))
    end
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

------------------------------------ ASSERTIONS ---------------------------

local function formatArg(arg)
  local argtype = type(arg)
  if argtype == "string" then
    return "'"..arg.."'"
  elseif argtype == "number" or argtype == "boolean" or argtype == "nil" then
    return tostring(arg)
  else
    return "["..tostring(arg).."]"
  end
end

function lib.assert(ok, msg, up_count)
  up_count = up_count or 2
  lib.current_suite._info.assert_count = lib.current_suite._info.assert_count + 1
  if not ok then
    error(msg, up_count + 1)
  end
end

function fail(msg)
  lib.assert(false, msg)
end

function assertFalse(ok)
  lib.assert(not ok, string.format('Should fail but passed.'))
end

function assertTrue(ok, msg)
  lib.assert(ok, msg or string.format('True expected but was false.'))
end

-- Test raw equality (same table)
function assertEqual(expected, value, resolution, up_count)
  up_count = up_count or 1
  if resolution and type(expected) == 'number' then
    local ok = (value >= expected - resolution) and (value <= expected + resolution)
    lib.assert(ok, string.format('Expected %s but found %s (resolution: %f).', formatArg(expected), formatArg(value), resolution), up_count + 1)
  else
    lib.assert(value == expected, string.format('Expected %s but found %s.', formatArg(expected), formatArg(value)), up_count + 1)
  end
end

-- Test value equality (same table content)
function assertValueEqual(expected, value, resolution, up_count)
  up_count = up_count or 1
  if type(expected) == 'table' then
    assertTableEqual(expected, value, resolution, up_count + 1)
  else
    assertEqual(expected, value, resolution, up_count + 1)
  end
end

function assertTableEqual(expected, value, resolution, up_count)
  up_count = up_count or 1
  assertEqual('table', type(value), up_count + 1)
  for i, v in ipairs(expected) do
    assertValueEqual(v, value[i], resolution, up_count + 1)
  end
  for k, v in pairs(expected) do
    assertValueEqual(v, value[k], resolution, up_count + 1)
  end
  for k, v in pairs(value) do
    if expected[k] == nil then
      lib.assert(false, string.format("Expected no '%s' key but found %s.", k, formatArg(v)), up_count + 1)
    end
    assertValueEqual(v, value[k], resolution, up_count + 1)
  end
  assertEqual(#expected, #value, up_count + 1)
end

function assertNotEqual(unexpected, value)
  lib.assert(value ~= unexpected, string.format('Should not equal %s.', formatArg(unexpected)))
end

function assertMatch(pattern, value)
  lib.assert(type(value) == 'string', string.format('Should be a string but was a %s.', type(value)))
  lib.assert(string.find(value, pattern), string.format('Expected to match %s but was %s.', formatArg(pattern), formatArg(value)))
end

function assertNotMatch(pattern, value, msg)
  lib.assert(type(value) == 'string', string.format('Should be a string but was a %s.', type(value)))
  lib.assert(not string.find(value, pattern), string.format('Expected to not match %s but was %s.', formatArg(pattern), formatArg(value)))
end

function assertError(pattern, func)
  local ok, err = pcall(func)
  lib.assert(not ok, string.format('Should raise an error but none found.'))
  lib.assert(string.find(err, pattern), string.format('Error expected to match %s but was %s.', formatArg(pattern), formatArg(err)))
end

function assertPass(func, teardown)
  local ok, err = pcall(func)
  if teardown then
    teardown()
  end
  if ok then
    lib.assert(true)
  else
    lib.assert(false, err) --string.format('Should not raise an error but %s found.', err))
  end
end

function assertLessThen(expected, value)
  lib.assert(value < expected, string.format('Should be less then %f but was %f.', expected, value))
end

function assertType(expected, value)
  lib.assert(type(value) == expected, string.format('Should be a %s but was %s.', expected, type(value)))
end

function assertNil(value)
  lib.assert(type(value) == 'nil', string.format('Should be a Nil but was %s.', type(value)))
end

function assertInRange(t1, t2, value)
  lib.assert(value >= t1 and value < t2, string.format('Should be in [%f, %f[ but was %f.', t1, t2, value))
end
