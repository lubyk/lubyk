require 'rubyk'

local should = test.Suite('rk.Dir')

function should.list_files_matching_pattern()
  local dir = rk.Dir('modules/rubyk')
  local pattern = '[.]lua$'
  for file in dir:glob(pattern) do
    assert_match(pattern, file)
  end
end

test.all()