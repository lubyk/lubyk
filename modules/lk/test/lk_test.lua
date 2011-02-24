require 'lubyk'

local should = test.Suite('lk.path helpers')

function should.readall()
  assert_equal('Hello Lubyk!', lk.readall(fixture.path('io.txt')))
end

function should.change_dir()
  local current = lk.dir()
  lk.with_filepath(current .. '/fixtures/baz.lua', function()
    assert_match(current .. '/fixtures$', lfs.currentdir())
  end)
end

function should.change_dir_before_dofile()
  dofile(fixture.path('dofile.lua'))
  assert_equal(2, lk_test_dofile_val)
end

function should.split()
  local list = lk.split('cat,dog,mouse', ',')
  assert_equal('cat'  , list[1])
  assert_equal('dog'  , list[2])
  assert_equal('mouse', list[3])
  assert_equal(3, #list)
end

function should.return_empty_on_spit_starting_with_sep()
  local list = lk.split('/my/home', '/')
  assert_equal(''    , list[1])
  assert_equal('my'  , list[2])
  assert_equal('home', list[3])
  assert_equal(3, #list)
end

function should.provide_source()
  assert_match('modules/lk/test/lk_test.lua$', lk.source())
end

function should.provide_dir()
  assert_match('modules/lk/test$', lk.dir())
end

function should.test_file_existence()
  assert_equal('file', lk.file_type(fixture.path('simple.yml')))
  assert_equal(nil, lk.file_type(fixture.path('complex.yml')))
  assert_equal('directory', lk.file_type(lk.dir()))
  assert_equal(nil, lk.file_type(nil))
end

test.all()