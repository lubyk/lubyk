--[[------------------------------------------------------

  sqlite3 test
  ------------

  SQLite3 database. The tests here are adapted from
  the LuaSQLite examples and tests.

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('sqlite3')

function should.setup(t)
  local db = sqlite3.open_memory()

  db:exec[[
    CREATE TABLE test (id INTEGER PRIMARY KEY, content);

    INSERT INTO test VALUES (NULL, 'Hello World');
    INSERT INTO test VALUES (NULL, 'Hello Lua');
    INSERT INTO test VALUES (NULL, 'Hello SQLite3')
  ]]
  t.db =db
end

function should.teardown(t)
  t.db = nil
end

function should.open(t)
  local db = sqlite3.open_memory()
  assert_type('userdata', db)
end

function should.prepare_insert(t)
  local db = sqlite3.open_memory()

  db:exec[[ CREATE TABLE test (id INTEGER PRIMARY KEY, content) ]]

  local stmt = db:prepare[[ INSERT INTO test VALUES (:key, :value) ]]

  stmt:bind_names{  key = 1,  value = "Hello World"    }
  stmt:step()
  assert_equal(1, db:last_insert_rowid())
  stmt:reset()
  stmt:bind_names{  key = 2,  value = "Hello Lua"      }
  stmt:step()
  assert_equal(2, db:last_insert_rowid())
  stmt:reset()
  stmt:bind_names{  key = 3,  value = "Hello SQLite3"  }
  stmt:step()
  assert_equal(3, db:last_insert_rowid())
  stmt:finalize()

  for row in db:nrows("SELECT * FROM test") do
    if row.id == 1 then
      assert_equal('Hello World', row.content)
    elseif row.id == 2 then
      assert_equal('Hello Lua', row.content)
    elseif row.id == 3 then
      assert_equal('Hello SQLite3', row.content)
    end
  end
end

function should.prepare_select(t)
  local db = t.db
  local stmt = db:prepare[[ SELECT content FROM test WHERE id = :id ]]
  stmt:bind_names{id = 2}
  assert_equal('Hello Lua', stmt:first_row()[1])
  assert_equal('Hello Lua', stmt:first_row()[1])
end

test.all()