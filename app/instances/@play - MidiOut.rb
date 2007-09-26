
@notes =  [62,nil,65,nil,nil,nil,62,nil]
@notes2 = [65,nil,69,nil, 38,nil,74,nil]

@seq = [
  [50,nil,62,65,nil,nil],
  [65,nil,67,nil,50,nil],
  [74,nil,68,67,nil,nil]]
  
@seq_seq = [0,0,1,0,2,1,1]
  
@channel = 1
@is  = 0
@is2 = 1
@i = 5
@i2 = 0
def bang
  #noteOn @notes[@i], 120, 0.1
  noteOn @seq[@seq_seq[@is]][@i], 90, 0.1
  noteOn (@seq[@seq_seq[@is2]][@i2]), 80, 0.3 if @seq[@seq_seq[@is2]][@i2]
  #
  @i  = (@i  + 1) % @seq[0].size
  @i2 = (@i2 + 1) % @seq[0].size
  if @i >= @seq.size
    @i == 0
    @is = (@is + 1) % @seq_seq.size
  end
  #
  if @i2 >= @seq.size
    @i2 == 0
    @is2 = (@is2 + 1) % @seq_seq.size
  end
end