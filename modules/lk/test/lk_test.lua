require 'lubyk'

local should = test.Suite('lk.path helpers')

function should.readall()
  assertEqual('Hello Lubyk!', lk.readall(fixture.path('io.txt')))
end

function should.change_dir()
  local current = lk.dir()
  lk.with_filepath(current .. '/fixtures/baz.lua', function()
    assertMatch(current .. '/fixtures$', lfs.currentdir())
  end)
end

function should.change_dir_before_dofile()
  dofile(fixture.path('dofile.lua'))
  assertEqual(2, lk_test_dofile_val)
end

function should.split()
  local list = lk.split('cat,dog,mouse', ',')
  assertEqual('cat'  , list[1])
  assertEqual('dog'  , list[2])
  assertEqual('mouse', list[3])
  assertEqual(3, #list)
end

function should.return_empty_on_spit_starting_with_sep()
  local list = lk.split('/my/home', '/')
  assertEqual(''    , list[1])
  assertEqual('my'  , list[2])
  assertEqual('home', list[3])
  assertEqual(3, #list)
end

function should.provide_source()
  assertMatch('modules/lk/test/lk_test.lua$', lk.source())
end

function should.provide_dir()
  assertMatch('modules/lk/test$', lk.dir())
end

function should.provide_file()
  assertMatch('modules/lk/test/lk_test.lua$', lk.file())
end

function should.test_file_existence()
  assertEqual('file', lk.file_type(fixture.path('simple.yml')))
  assertEqual(nil, lk.file_type(fixture.path('complex.yml')))
  assertEqual('directory', lk.file_type(lk.dir()))
  assertEqual(nil, lk.file_type(nil))
end

function should.findcode_locally()
  assertMatch('Passes input values to output', lk.findcode(lk.dir(), 'fixtures.pass'))
end

function should.findcode_in_libs()
  assertMatch('Triggers regular bangs', lk.findcode(lk.dir(), 'lubyk.Metro'))
end

test.all()