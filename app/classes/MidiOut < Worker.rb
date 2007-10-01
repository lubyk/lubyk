# we are in the instance @total
require 'rtmidi'
def sync
  @i  = 0
  @i1 = 0
  @i2 = 0
end

def init
  @midi = RtMidi.new(nil)
  @notes =  [62,65, 0, 62, 26]
  @notes2 = [65,69, 38, 74, 0]
  @i  = 0
  @i2 = 0
end

def bang
  noteOn @notes[@i]
  noteOn @notes2[@i2]
  
  @i  = (@i  + 1) % @notes.size
  @i2 = (@i2 + 1) % @notes2.size
end


def noteOn(note, velocity=80, duration=0.5)
  return unless note
  @midi.noteOn(@channel || 1,note,velocity)
  Worker.add_to_queue(@time + duration, self, :noteOff, note)
end

def noteOff(note)
  @midi.noteOff(@channel || 1,note,0)
end

def destroy
  (@notes + @notes2).uniq.each do |n|
    noteOff(n)
  end
end