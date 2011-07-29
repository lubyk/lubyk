--[[------------------------------------------------------

  editor.Library
  --------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor.Library')

function should.loadCode()
  assertTrue(editor.Library)
end

local function makeLib()
  local lib = editor.Library(sqlite3.open_memory())
  lib.sources = {
    lubyk = fixture.path('nodes')
  }
  lib:sync()
  return lib
end

function should.populateDatabase()
  local lib = makeLib()
  assertEqual(2, lib:nodeCount())
end

function should.findNodeByPosition()
  local lib = makeLib()
  assertValueEqual({
    name = 'lubyk.Bar',
    keywords = 'lubyk.Bar',
  }, lib:node(1))
  assertValueEqual({
    name = 'lubyk.Foo'
  }, lib:node(2))
end

function should.findWithFilter()
  local lib = makeLib()
  assertValueEqual({
    name = 'lubyk.Foo',
    keywords = 'lubyk.Foo',
  }, lib:node('oo'))
end

function should.notAddPercentInFilter()
  local lib = makeLib()
  assertNil(lib:node('Foo%'))
end

function should.paginateWithFilter()
  local lib = makeLib()
  assertValueEqual({
    name = 'lubyk.Bar',
    keywords = 'lubyk.Bar',
  }, lib:node('lubyk'))
  assertValueEqual({
    name = 'lubyk.Foo',
    keywords = 'lubyk.Foo',
  }, lib:node('lubyk', 2))
end

function should.returnNilOnNotFound()
  local lib = makeLib()
  assertEqual(nil, lib:node('xx'))
end

test.all()
