# we are in the instance @total
require 'rtmidi'

def bang
  @midi.noteOn( 1,@notes[@i],100)
  @midi.noteOn( 1,@notes2[@i2],110)
  sleep 0.2
  @midi.noteOff(1,@notes[@i],100)
  @midi.noteOff(1,@notes[@i2],110)
  @i  = (@i  + 1) % @notes.size
  @i2 = (@i2 + 1) % @notes2.size
end