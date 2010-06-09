require('cv')

mi = mi or Outlet("mi", MidiIO("sends midi notes"))

Note = oscit.MidiMessage_Note

l = l or 85.8
i = i or 1
oct = oct or 0
function bang()
  i = i + 1
  N = {
    {60, nil, 69, 65},
    {65, 60, 67, nil},
--    {63, 60, 50, nil},
--    {},
    {63, 60, 72, nil, 63, 60, 72, 73},
--    {},
  }

  N2 = {
    {65, nil, 68, 79},
--    {63, 60, 50, nil},
    {63, 60, 72, nil, 63, 60, 72, 73},
--    {},
--    {},
--    {60, 63, 65, 34, 60, 58, nil, 65},
  }

  n  = n  or N[1]
  n2 = n2 or N2[1]

  if i % 16 == 0 then
    oct = ((oct + 1) % 2)
  end

  if i % 8 == 0 then
    n  =  N[1 + (i/8) % #N]
    n2 = N2[1 + (i/8) % #N2]
  end

  local nval = n[1 + i % #n]
  if nval then
    mi.send(Note(12 * oct + nval - 12, 74, l))
  end

  local nval2 = n2[1 + i % #n2]
  if nval2 then
    mi.send(Note(12 * oct + nval2 - 19, 74, l))
  end
end
inlet("bang", BangIO("hop"))


function length(r)
  if r then
    l = r
  end
  return l
end
inlet("length", RangeIO(10, 1000, "Set note length"))

function notes(n)
  if n then
    mi.send(Note(n, 74, l))
    return n
  else
    return 0
  end
end
inlet("notes", RangeIO(1,127, "Send note out"))

-- Should be
-- method('notes', n, x, y) do
-- end
-- Or even better:
-- method notes(n)
-- end