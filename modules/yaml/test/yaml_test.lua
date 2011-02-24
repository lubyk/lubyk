--[[------------------------------------------------------

  yaml test
  ---------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('yaml')

function should.auto_load()
  assert_true(yaml.loadpath)
end

function should.load_hash()
  local simple = yaml.load(fixture.readall('simple.yml'))
  assert_equal('hello', simple.hash.a)
  assert_equal('lubyk', simple.hash.b)
end

function should.load_list()
  local simple = yaml.load(fixture.readall('simple.yml'))
  assert_equal('first',  simple.list[1])
  assert_equal('second', simple.list[2])
end

function should.load_path()
  local simple = yaml.loadpath(fixture.path('simple.yml'))
  assert_equal('first',  simple.list[1])
  assert_equal('second', simple.list[2])
end

function should.use_refs_as_same_obj()
  local refs = yaml.load(fixture.readall('refs.yml'))
  assert_equal('Jane',  refs.roles.boss.name)
  -- same obj
  assert_true(refs.roles.wife == refs.roles.boss)
  assert_true(refs.all[1]     == refs.roles.boss)
end

test.all()
