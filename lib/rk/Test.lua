local lib = {suites = {}}
rk.Test = lib

function lib.run(tests)
  lunit.testcase(tests)
  lunit.main()
end

function lib.Suite(name)
  local suite = {_info = {name = name, tests = {}, errors = {}}}
  lib.suites[name] = suite
  -- default setup and teardown functions
  suite.setup    = function() end
  suite.teardown = function() end
  return suite
end

function lib.all()
  for name, suite in pairs(lib.suites) do
    lib.run_suite(suite)
  end
  lib.report()
end

function lib.run_suite(suite)
  local test_count = 0
  local fail_count = 0
  local errors = suite._info.errors
  lib.current_suite = suite
  -- run all tests in the current file
  for name,func in pairs(suite) do
    if type(func) == 'function' then
      -- make sure it's a test
      if name ~= '_info' and name ~= 'setup' and name ~= 'teardown' then
        test_count = test_count + 1

        suite.setup()
          local ok, err = pcall(func)
          if not ok then
            fail_count = fail_count + 1
            local file, line, message = string.match(err, "([^/\.]+\.lua):(%d+): (.+)")
            errors[name] = message
          end
        suite.teardown()
      end
    end
  end

  suite._info.test_count = test_count
  suite._info.fail_count = fail_count
end

function lib.report()
  local total_test  = 0
  local total_fail = 0
  local ok_message = ''
  print('Testing report')
  for name, suite in pairs(lib.suites) do
    if suite._info.fail_count == 0 then
      ok_message = 'OK'
    else
      ok_message = string.format('%i Failure(s)', suite._info.fail_count)
    end
    print(string.format('==== %s (%i tests): %s', suite._info.name, suite._info.test_count, ok_message))
    total_test = total_test + suite._info.test_count
    if suite._info.fail_count > 0 then
      for name, err in pairs(suite._info.errors) do
        total_fail = total_fail + 1
        local hname = string.gsub(name, '_', ' ')
        print(string.format('\n  %i. %s\n     %s', total_fail, hname, err))
      end
    end
  end

  print('\n')

  if total_test == 0 then
    print(string.format('No tests defined. Test functions must end with "_test"'))
  elseif total_fail == 0 then
    if total_test == 1 then
      print(string.format('Success! Your test passes.', total_test))
    else
      print(string.format('Success! %i tests pass.', total_test))
    end
  elseif total_test == 1 then
    if total_fail == 1 then
      print(string.format('Fail... %i failure / %i test', total_fail, total_test))
    else
      print(string.format('Fail... %i failures / %i test', total_fail, total_test))
    end
  else
    if total_fail == 1 then
      print(string.format('Fail... %i failure / %i tests', total_fail, total_test))
    else
      print(string.format('Fail... %i failures / %i tests', total_fail, total_test))
    end
  end
end

------------------------------------ ASSERTIONS ---------------------------
function lib.assert(ok, message)
  if not ok then
    error(message, 3)
  end
end

function assert_equal(a, b)
  lib.assert(a == b, string.format('Expected %s but found %s', a, b))
end




