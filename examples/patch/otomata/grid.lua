--[[------------------------------------------------------

  grid
  ----

  Implements the automaton.

--]]------------------------------------------------------
inlet('advance', 'Receive clock animate [bang].')
inlet('paint', 'Draw cells [p, w, h]')
inlet('click', 'Mouse event [float, float, bool]')
hit     = outlet('hit', 'Sends hit position [int, int]')
updated = outlet('updated', 'Sends bang when all cells have been updated [bang].')

local UP = '^'
local RIGHT = '>'
local DOWN = 'v'
local LEFT = '<'
local TURN = {
 ['^'] = '>',
 ['>'] = 'v',
 ['v'] = '<',
 ['<'] = '^',
}
local GRID_COLOR = mimas.Color(0, 0, 0.2)
local CELL_COLOR = mimas.Color(0.1, 0.5, 0.5)
local ROW_COUNT = 9
local COL_COUNT = 9

local function make_grid()
  local rows = {}
  for i = 1, ROW_COUNT do
    rows[i] = {}
  end
  return rows
end

local function make_cells(definition)
  local cells = {}
  for i, rows in ipairs(lk.split(definition, '\n')) do
    for j, char in ipairs(lk.split(definition)) do
      if char ~= '.' then
        local cell = {dir=char, row = i, col = j}
        table.insert(cells, cell)
      end
    end
  end

  return cells
end

cells = cells or make_cells[[
.........
..v......
......v..
.........
.........
.........
.........
.>..1..<.
^.......^
]]

function inlet.advance(bang)
  local grid = make_grid()

  for _, cell in ipairs(cells) do
    local dir = cell.dir
    if dir == UP then
      if cell.row == 1 then
        hit(cell.row - 1, cell.col)
        cell.dir = DOWN
        cell.row = cell.row + 1
      else
        cell.row = cell.row - 1
      end
    elseif dir == RIGHT then
      if cell.col == COL_COUNT then
        hit(cell.row, cell.col + 1)
        cell.dir = LEFT
        cell.col = cell.col - 1
      else
        cell.col = cell.col + 1
      end
    elseif dir == DOWN then
      if cell.row == ROW_COUNT then
        hit(cell.row, cell.col + 1)
        cell.dir = UP
        cell.row = cell.row - 1
      else
        cell.row = cell.row + 1
      end
    elseif dir == LEFT then
      if cell.col == 1 then
        hit(cell.row - 1, cell.col)
        cell.dir = RIGHT
        cell.col = cell.col + 1
      else
        cell.col = cell.col - 1
      end
    end
    local meet = grid[cell.row][cell.col]
    if meet then
      -- turn both cells
      meet.dir = TURN[meet.dir]
      cell.dir = TURN[cell.dir]
    end
    grid[cell.row][cell.col] = cell
  end
  updated(true)
end

-- painter, width, height
function inlet.paint(p, w, h)
  local cell_w = w / COL_COUNT
  local cell_h = h / ROW_COUNT
  -- draw grid
  p:fillRect(0, 0, w, h, GRID_COLOR)
  -- draw cells
  for _, cell in ipairs(cells) do
    p:fillRect((cell.col-1) * cell_w, (cell.row-1) * cell_h, cell_w, cell_h, CELL_COLOR)
  end
end

print "GRID OK"