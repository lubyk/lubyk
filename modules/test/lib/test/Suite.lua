--[[------------------------------------------------------
  # Test Suite

  The test suite is used to declare tests and group them
  by class or module.

--]]------------------------------------------------------
local lib = class 'test.Suite'
local private = {}
local test = test
-- Test is used as meta-table for the test var "t".
test.__index = test

-- Create a new test suite. It is good practice to use 'should' as
-- local name for this suite since it makes tests more readable. The
-- @name@ should be the exact name of the class or module being tested
-- in case the coverage option is activated in test (it is ON by default).
--
-- Usage example:
--
--   local should = test.Suite 'lk.Dir'
--
--   function should.listFilesInDirectory()
--     -- ...
--   end
-- 
-- The optional `opt` table can contain the following keys to alter testing
-- behavior:
--
-- + `coverage`: if set to false, untested functions will not be reported. If
--               `test.missing` is set to 'mock', mock test functions will be
--               printed for missing tests.
function lib.new(name, options)
  options = options or {}
  local self = {
    _info = {
      name = name,
      tests = {},
      errors = {},
      user_suite = options.user_suite,
    },
  }
  if options.coverage == false then
    self._info.coverage = false
  else
    self._info.coverage = true
  end
  setmetatable(self, lib)
  table.insert(test.suites, self)
  -- default setup and teardown functions
  return self
end

-- Return tests to only run in single mode (not in batch) because they require
-- user interaction. Usage:
--
--   local should   = test.Suite 'lk.Dir'
--   local withUser = should:testWithUser()
--
--   function withUser.should.displayTable()
--   end
--
function lib:testWithUser()
  return test.UserSuite(self._info.name)
end


-- # Setup, teardown
--
-- If you need to run some code before or after every test, you can define
-- the @setup@ (executed before each test) and/or @teardown@ (executed after
-- each test independantly of success or failure) functions:
--
--   local should = test.Suite 'lk.Dir'
--   
--   function should.teardown()
--     -- cleanup
--   end

-- Dummy setup function (does nothing).
function lib.setup() end

-- Dummy teardown function (does nothing).
function lib.teardown() end

-- # Declare tests

-- Tests are declared by adding functions to the test suite. Note that these
-- functions do not have access to 'self'.
--
-- Example test declaration:
--
--   functions should.beHappy()
--     assertTrue(true)
--   end
function lib:__newindex(key, value)
  rawset(self, key, value)
  if type(value) == 'function' and
    key ~= 'setup' and
    key ~= 'teardown' and
    key ~= 'should' then
    table.insert(self._info.tests, {key, value})
  end
end

-- # Run tests
--
-- Use @test.all()@ to run the tests in all suites at once. You can append this
-- call at the end of the file to run the test when running the file:
--
--   function should.testLast()
--   end
--
--   -- Run all tests now.
--   test.all()

-- nodoc
function lib:test(batch)
  if not batch then
    test.parseArgs()
    test.total_exec  = 0
    test.total_count = 0
    test.total_asrt  = 0
    test.total_fail  = 0
    sched:run(function()
      private.runSuite(self)
      private.reportSuite(self)
      test.report()
      sched:quit()
    end)
  else
    private.runSuite(self)
    private.reportSuite(self)
  end
end

------------------------------------ ASSERTIONS ---------------------------

-- # Assertions

local function formatArg(arg)
  local argtype = type(arg)
  if argtype == "string" then
    return "'"..arg.."'"
  elseif argtype == "number" or argtype == "boolean" or argtype == "nil" then
    return tostring(arg)
  else
    return tostring(arg)
  end
end

local function assert(ok, msg, up_count)
  up_count = up_count or 2
  test._info.assert_count = test._info.assert_count + 1
  if not ok then
    error(msg, up_count + 1)
  end
end

-- Force a test to fail with a given @msg@.
function fail(msg)
  assert(false, msg)
end

-- Assert that @ok@ is false.
function assertFalse(ok)
  assert(not ok, string.format('Should fail but passed.'))
end

-- Assert that @ok@ is true. If @msg@ exists, it is used in case of failure
-- in place of the default fail message.
function assertTrue(ok, msg)
  assert(ok, msg or string.format('True expected but was false.'))
end

-- Assert that @value@ is equal to @expected@. If @expected@ is a number,
-- the @resolution@ parameter can be used to cope with numerical errors.
-- The actual test for numbers is:
--
--   local ok = (value >= expected - resolution) and (value <= expected + resolution)
-- 
-- For other types, this tests raw equality (same object). To compare table
-- contents, use #assertValueEqual.
function assertEqual(expected, value, resolution, up_count)
  up_count = up_count or 1
  if resolution and type(expected) == 'number' then
    local ok = (value >= expected - resolution) and (value <= expected + resolution)
    assert(ok, string.format('Expected %s but found %s (resolution: %f).', formatArg(expected), formatArg(value), resolution), up_count + 1)
  else
    assert(value == expected, string.format('Expected %s but found %s.', formatArg(expected), formatArg(value)), up_count + 1)
  end
end

-- For tables, recursively test that all keys contain the same values.
function assertValueEqual(expected, value, resolution, up_count)
  up_count = up_count or 1
  if type(expected) == 'table' then
    assertTableEqual(expected, value, resolution, up_count + 1)
  else
    assertEqual(expected, value, resolution, up_count + 1)
  end
end

-- This is like #assertValueEqual but does not check for table type.
function assertTableEqual(expected, value, resolution, up_count)
  up_count = up_count or 1
  assertEqual('table', type(value), up_count + 1)
  for k, v in pairs(expected) do
    assertValueEqual(v, value[k], resolution, up_count + 1)
  end
  for k, v in pairs(value) do
    if expected[k] == nil then
      assert(false, string.format("Expected no '%s' key but found %s.", k, formatArg(v)), up_count + 1)
    end
    assertValueEqual(v, value[k], resolution, up_count + 1)
  end
  assertEqual(#expected, #value, up_count + 1)
end

-- Asserts that @value@ is not equal to @unexpected@.
function assertNotEqual(unexpected, value)
  assert(value ~= unexpected, string.format('Should not equal %s.', formatArg(unexpected)))
end

-- Assert that @value@ matches @pattern@ using @string.match@.
function assertMatch(pattern, value)
  assert(type(value) == 'string', string.format('Should be a string but was a %s.', type(value)))
  assert(string.find(value, pattern), string.format('Expected to match %s but was %s.', formatArg(pattern), formatArg(value)))
end

-- Assert that @value@ does not match @pattern@. If @msg@ is provided, use this
-- in case of failure.
function assertNotMatch(pattern, value, msg)
  assert(type(value) == 'string', string.format('Should be a string but was a %s.', type(value)))
  assert(not string.find(value, pattern), string.format('Expected to not match %s but was %s.', formatArg(pattern), formatArg(value)))
end

-- Assert that calling @func@ generates an error message that matches @pattern@.
function assertError(pattern, func)
  local ok, err = pcall(func)
  assert(not ok, string.format('Should raise an error but none found.'))
  assert(string.find(err, pattern), string.format('Error expected to match %s but was %s.', formatArg(pattern), formatArg(err)))
end

-- Assert that calling @func@ passes without errors. The optional @teardown@ 
-- function can be used to cleanup after the function call whether it passes
-- or fails.
function assertPass(func, teardown)
  local ok, err = pcall(func)
  if teardown then
    teardown()
  end
  if ok then
    assert(true)
  else
    assert(false, err)
  end
end

-- Assert that @value@ is less then @expected@.
function assertLessThen(expected, value)
  assert(value < expected, string.format('Should be less then %f but was %f.', expected, value))
end

-- Assert that the Lua type of @value@ is @expected@ ('number', 'table',
-- 'function', etc).
function assertType(expected, value)
  assert(type(value) == expected, string.format('Should be a %s but was %s.', expected, type(value)))
end

-- Assert that @value@ is nil.
function assertNil(value)
  assert(type(value) == 'nil', string.format('Should be a Nil but was %s.', type(value)))
end

-- Assert that @value@ is in the range defined by [@t1@, @t2@[.
function assertInRange(t1, t2, value)
  assert(value >= t1 and value < t2, string.format('Should be in [%f, %f[ but was %f.', t1, t2, value))
end

--=============================================== PRIVATE

function private:runSuite()
  if self._info.coverage then
    -- Make sure all functions are called at least once.
    local parent, meta = _G, _G
    for _, part in ipairs(lk.split(self._info.name, '%.')) do
      parent = meta
      meta   = meta[part]
      if not meta then break end
    end
    _G.assert(meta, string.format("Testing coverage but '%s' metatable not found.", self._info.name))

    local coverage = {}
    for k, v in pairs(meta) do
      if type(v) == 'function' then
        coverage[k] = v
        -- Dummy function to catch first call without using debug hook.
        meta[k] = function(...)
          coverage[k] = true
          meta[k] = v
          return v(...)
        end
      end
    end

    function self.testAllFunctions()
      local all_ok = true
      local not_tested = {}
      for k, info in pairs(coverage) do
        if info ~= true then
          if test.mock then
            k = string.upper(string.sub(k, 1, 1))..string.sub(k, 2, -1)
            lk.insertSorted(not_tested, {
              text = 'function should.respondTo'..k..'()\n'..test.mock..'end',
              line = tonumber(debug.getinfo(info).linedefined),
            }, 'line')
          else
            lk.insertSorted(not_tested, "'"..k.."'")
          end
          all_ok = false
        end
      end
      local list
      if test.mock then
        list = '\n\n'
        for _, info in ipairs(not_tested) do
          list = list .. '\n\n' .. info.text .. ' --' .. info.line
        end
      else
        list = lk.join(not_tested, ', ')
      end
      assertTrue(all_ok, string.format("Missing tests for %s", list))
    end
  end

  local exec_count = 0
  local fail_count = 0
  local skip_count = 0
  local errors = self._info.errors
  local test_var
  local test_func
  local function pass_args() return test_func(test_var) end
  test._info = self._info
  self._info.assert_count = 0
  -- list of objects protected from gc
  self._info.gc_protect = {}
  local gc_protect = self._info.gc_protect
  -- run all tests in the current file
  local skip = self._info.user_suite and test.file_count > 1
  for i,e in pairs(self._info.tests) do
    local name, func = unpack(e)
    test_var = setmetatable({}, test)
    test_var._name  = name
    test_var._suite = self
    gc_protect[name] = test_var
    test_func = func
    if skip or (test.only and test.only ~= name) then
      -- skip user tests
      skip_count = skip_count + 1
    else
      exec_count = exec_count + 1
      self.setup(gc_protect[name])
      if test.verbose then
        printf("%-12s Run %s", '['..self._info.name..']', name)
      end
      local ok, err = sched:pcall(pass_args)
      if test.verbose then
        printf("%s %s", ok and 'OK' or 'FAIL', err or '')
      end
      collectgarbage('collect')
      if not ok then
        fail_count = fail_count + 1
        -- Get line and message for assertPass, assertError
        table.insert(errors, {i, name, err})
      end
      self.teardown(gc_protect[name])
      if test.abort then
        break
      end
    end
  end

  self._info.exec_count = exec_count
  self._info.total_count = #self._info.tests
  self._info.fail_count = fail_count
  self._info.skip_count = skip_count
end

function private:reportSuite()
  local ok_message, skip_message = '', ''
  if self._info.fail_count == 0 then
    ok_message = 'OK'
  else
    ok_message = string.format('%i Failure(s)', self._info.fail_count)
  end
  local exec_count = self._info.exec_count
  if self._info.skip_count > 0 then
    if exec_count == 0 then
      ok_message = '-- skip'
    else
      skip_message = string.format(' : skipped %i', self._info.skip_count)
    end
  end
  print(string.format('==== %-28s (%2i test%s%s): %s', self._info.name, exec_count, exec_count > 1 and 's' or ' ', skip_message, ok_message))
  test.total_exec = test.total_exec + self._info.exec_count
  test.total_count = test.total_count + self._info.total_count
  test.total_asrt = test.total_asrt + self._info.assert_count
  if self._info.fail_count > 0 then
    for _, e in ipairs(self._info.errors) do
      local i, name, err = unpack(e)
      test.total_fail = test.total_fail + 1
      local hname = string.gsub(name, '([A-Z])', function(x) return ' '..string.lower(x) end)
      print(string.format('  %i. Should %s\n     %s\n', i, hname, string.gsub(err, '\n', '\n     ')))
    end
  end
end

