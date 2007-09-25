require 'rtmidi'

@midi ||= RtMidi.new(nil)
@notes =  [62,65, 0, 62, 26]
@notes2 = [65,69, 38, 74, 0]
@i  = 0
@i2 = 0
def runner
  @midi.noteOn( 1,@notes[@i],100)
  @midi.noteOn( 1,@notes2[@i2],110)
  sleep 0.2
  @midi.noteOff(1,@notes[@i],100)
  @midi.noteOff(1,@notes[@i2],110)
  @i  = (@i  + 1) % @notes.size
  @i2 = (@i2 + 1) % @notes2.size
end

@running = :run
(@notes + @notes2).uniq.each do |n|
  @midi.noteOff(1,n,0)
end