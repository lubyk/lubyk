require 'debug'

local lib = {suites = {}}
test = lib

function lib.run(tests)
  lunit.testcase(tests)
  lunit.main()
end

function lib.Suite(name)
  local suite = {_info = {name = name, tests = {}, errors = {}}}
  table.insert(lib.suites, suite)
  -- default setup and teardown functions
  suite.setup    = function() end
  suite.teardown = function() end
  return suite
end

function lib.all()
  lib.total_test = 0
  lib.total_asrt = 0
  lib.total_fail = 0
  testing_gui = false
  for i, suite in ipairs(lib.suites) do
    if string.match(suite._info.name, 'mimas') then
      testing_gui = true
      break
    end
  end

  if testing_gui then
    lib.gui()
  else
    for i, suite in ipairs(lib.suites) do
      lib.run_suite(suite)
      lib.report_suite(suite)
    end
    lib.report()
  end
end

function lib.gui()
  lib.total_test = 0
  lib.total_asrt = 0
  lib.total_fail = 0
  if lib.suites[1] then
    -- test threads first (or they fail because of mimas widgets creation)
    for i, suite in ipairs(lib.suites) do
      if string.match('lk.Thread', suite._info.name) then
        lib.run_suite(suite)
        lib.report_suite(suite)
        table.remove(lib.suites, i)
        break
      end
    end

    app = mimas.Application()
    app:post(function()
      for i, suite in ipairs(lib.suites) do
        lib.run_suite(suite)
        lib.report_suite(suite)
      end
    end)

    app:exec()
  end
  -- get report when all threads have finished (all windows closed)
  lib.report()
end

function lib.loadAll(...)
  local arg = {...}
  if not arg[1] then
    -- load all
    for mod in lk.Dir('modules'):list() do
      print('Loading tests for', mod)
      if lk.fileType(mod) == 'directory' then
        for file in lk.Dir(mod):glob('test/.+_test[.]lua$') do
          dofile(file)
        end
      end
    end
  else
    for _, mod in ipairs(arg) do
      print('Loading tests for', mod)
      for file in lk.Dir('modules'..lk.Dir.sep..mod):glob('test/.+_test[.]lua$') do
        dofile(file)
      end
    end
  end
end

-- FIXME: traceback not working good enough
local function error_handler(err)
  local tb = lk.split(debug.traceback(), '\n')
  local max_i = 5
  local message = err
  for i = 4,#tb do
    if string.find(tb[i], 'lubyk/lib/test.lua') then
      max_i = i - 2
      break
    end
  end
  for i = 4,max_i do
    message = message .. '\n' .. tb[i]
  end
  return message
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

function lib.run_suite(suite)
  local test_count = 0
  local fail_count = 0
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
  for name,func in pairs(suite) do
    if type(func) == 'function' then
      -- make sure it's a test
      if name ~= '_info' and name ~= 'setup' and name ~= 'teardown' then
        test_count = test_count + 1
        gc_protect[name] = {}
        test_var  = gc_protect[name]
        test_func = func
        suite.setup(gc_protect[name])
          local ok, err = xpcall(pass_args, error_handler)
          --local ok, err = pcall(pass_args)
          if not ok then
            fail_count = fail_count + 1
            --local file, line, message = string.match(err, "([^/\.]+\.lua):(%d+): (.+)")
            --if message then
            --  errors[name] = message
            --else
              errors[name] = err
            --end
          end
        suite.teardown(gc_protect[name])
      end
    end
  end

  suite._info.test_count = test_count
  suite._info.fail_count = fail_count
end

function lib.report_suite(suite)
  lib.ok_message = ''
  if suite._info.fail_count == 0 then
    ok_message = 'OK'
  else
    ok_message = string.format('%i Failure(s)', suite._info.fail_count)
  end
  print(string.format('==== %-18s (%2i tests): %s', suite._info.name, suite._info.test_count, ok_message))
  lib.total_test = lib.total_test + suite._info.test_count
  lib.total_asrt = lib.total_asrt + suite._info.assert_count
  if suite._info.fail_count > 0 then
    for name, err in pairs(suite._info.errors) do
      lib.total_fail = lib.total_fail + 1
      local hname = string.gsub(name, '([A-Z])', function(x) return ' '..string.lower(x) end)
      print(string.format('  %i. Should %s\n     %s\n', lib.total_fail, hname, string.gsub(err, '\n', '\n     ')))
    end
  end
end

function lib.report()
  print('\n')

  if lib.total_test == 0 then
    print(string.format('No tests defined. Test functions must end with "_test.lua"'))
  elseif lib.total_fail == 0 then
    if lib.total_test == 1 then
      print(string.format('Success! %i test passes (%i assertions).', lib.total_test, lib.total_asrt))
    else
      print(string.format('Success! %i tests pass (%i assertions).', lib.total_test, lib.total_asrt))
    end
  elseif lib.total_test == 1 then
    if lib.total_fail == 1 then
      print(string.format('Fail... %i failure / %i test', lib.total_fail, lib.total_test))
    else
      print(string.format('Fail... %i failures / %i test', lib.total_fail, lib.total_test))
    end
  else
    if lib.total_fail == 1 then
      print(string.format('Fail... %i failure / %i tests', lib.total_fail, lib.total_test))
    else
      print(string.format('Fail... %i failures / %i tests', lib.total_fail, lib.total_test))
    end
  end
  print('')
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

function lib.assert(ok, msg)
  lib.current_suite._info.assert_count = lib.current_suite._info.assert_count + 1
  if not ok then
    error(msg, 3)
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
function assertEqual(expected, value)
  lib.assert(value == expected, string.format('Expected %s but found %s.', formatArg(expected), formatArg(value)))
end

-- Test value equality (same table content)
function assertValueEqual(expected, value)
  if type(expected) == 'table' then
    assertTableEqual(expected, value)
  else
    assertEqual(expected, value)
  end
end

function assertTableEqual(expected, value)
  assertEqual('table', type(value))
  for i, v in ipairs(expected) do
    assertValueEqual(v, value[i])
  end
  for k, v in pairs(expected) do
    assertValueEqual(v, value[k])
  end
end

function assertNotEqual(unexpected, value)
  lib.assert(value ~= unexpected, string.format('Should not equal %s.', formatArg(unexpected)))
end

function assertMatch(pattern, value)
  lib.assert(type(value) == 'string', string.format('Should be a string but was a %s.', type(value)))
  lib.assert(string.find(value, pattern), string.format('Expected to match %s but was %s.', formatArg(pattern), formatArg(value)))
end

function assertNotMatch(pattern, actual, msg)
  lib.assert(type(value) == 'string', string.format('Should be a string but was a %s.', type(value)))
  lib.assert(not string.find(value, pattern), string.format('Expected to not match %s but was %s.', formatArg(pattern), formatArg(value)))
end

function assertError(pattern, func)
  local ok, err = pcall(func)
  lib.assert(not ok, string.format('Should raise an error but none found.'))
  lib.assert(string.find(err, pattern), string.format('Error expected to match %s but was %s.', formatArg(pattern), formatArg(err)))
end

function assertPass(func)
  local ok, err = pcall(func)
  if ok then
    lib.assert(true)
  else
    lib.assert(false, string.format('Should not raise an error but %s found.', err))
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
