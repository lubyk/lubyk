@channel = 2
@i ||= 0
@seq = [38,nil,39,nil,38,38,39,nil,38,nil,39,39,nil,38,39,nil]
def bang
  @i = (@i + 1)
  noteOn @seq[@i % @seq.size], 20
  noteOn (36 + (@i % 2)), 40, 0.1
  noteOn 62, 40, 0.1 if @i % 4 == 0
  noteOn 63, 40, 0.1 if @i % 3 == 0
end