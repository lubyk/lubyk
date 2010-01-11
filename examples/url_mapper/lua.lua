inlet('bang', BangIO('bang', 'Receive bangs to play notes.'))
notes = Outlet('notes', RealIO('real', 'Note value.'))

note_list = {60, 63, 65, 66}
i = i or 0

function bang()
  i = i + 1
  notes.send(note_list[1 + i % #note_list])
end
  