@channel = 2
@i = 0
@seq = [38,nil,39,nil,38,38,39,nil,38,nil,39,39,nil,38,39,nil]
def bang
  base = 8
  noteOn @seq[@i % base], 10
  noteOn 42, 110, 0.1 if @i % 2 == 0
  #noteOn 36, 10, 0.1 if @i % 5 == 0
  noteOn 42, 10, 0.1 if @i % 8 == 0
  noteOn 36, 70, 0.1 if @i % 4 == 0
  noteOn 63, 70, 0.1 if @i % 2 == 0
  
  @i += 1
end