--[[------------------------------------------------------

  midi.libote
  ---------

  Simplify midi note number manipulation. For example,
    libote['C0'] => 24.

  In this system, 'middle' C is C4 (ISO Standard)

--]]------------------------------------------------------
local lib = {}
midi.Note = lib
local reverse = {}
midi.Note.name = reverse

local OCTAVES = {[''] = 0}
for i=-1,9 do
  OCTAVES[string.format('%i', i)] = i
end

local BASE_NOTES = {
  [ 'C'] = 12,
  ['C#'] = 13,
  ['Db'] = 13,
  [ 'D'] = 14,
  ['D#'] = 15,
  ['Eb'] = 15,
  [ 'E'] = 16,
  [ 'F'] = 17,
  ['F#'] = 18,
  ['Gb'] = 18,
  [ 'G'] = 19,
  ['G#'] = 20,
  ['Ab'] = 20,
  [ 'A'] = 21,
  ['A#'] = 22,
  ['Bb'] = 22,
  [ 'B'] = 23,
}

for oct_name, delta in pairs(OCTAVES) do
  for note, base_value in pairs(BASE_NOTES) do
    local value = base_value + 12 * delta
    local name  = string.format('%s%s', note, oct_name)
    lib[name] = value
    if oct_name ~= '' then
      reverse[value] = name
    end
  end
end
