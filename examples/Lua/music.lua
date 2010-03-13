require('cv')

mi = mi or Outlet("mi", MidiIO("sends midi notes"))

Note = oscit.MidiMessage_Note

l = l or 85.8
i = i or 1
oct = oct or 0
function bang()
  i = i + 1
  N = {
    {34, 36, 37, 39, 41, 83},
    {34, 41, 39, 37, 39, 65, 72},
  }
  n = n or N[1]
  if i % 10 == 0 then
    oct = (oct + 1) % 2
  end

  if i % 20 == 0 then
    n = N[1 + (i/20) % #N]
  end

  mi.send(Note(12 * oct + n[1 + i % #n],74, l))
end
inlet("bang", BangIO("hop"))


function length(r)
  if r then
    l = r
  end
  return l
end
inlet("length", RangeIO(10,1000, "Set note length"))
