@midi ||= RtMidi.new(nil)
@notes =  [62,65, 0, 62, 26]
@notes2 = [65,69, 38, 74, 0]
@i  = 0
@i2 = 0

(@notes + @notes2).uniq.each do |n|
  @midi.noteOff(1,n,0)
end