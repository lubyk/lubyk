require 'rubyk'
require 'rk.Test'
require 'util'

local should = rk.Test.Suite('util.Dir')

function should.list_files_matching_pattern()
  local dir = util.Dir('modules/util')
  local pattern = '[.]lua$'
  for file in dir:glob(pattern) do
    assert_match(pattern, file)
  end
end

rk.Test.all()