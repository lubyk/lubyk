
@notes =  [62,nil,nil,nil,nil,nil,62,nil]
@notes2 = [65,nil,69,nil, 38,nil,74,nil]

@seq = [
  [55 ,nil,nil,56 ,65 ,nil,67 ,nil,nil,nil],
  [nil,63 ,50 ,nil,nil,nil,nil,63,nil,nil],
  [65 ,nil,67 ,nil,50 ,nil,nil,62,nil,nil],
  [76 ,nil,68 ,65 ,nil,nil,61,nil,nil,nil]]
  
@seq_seq1 = [0,0,2,2] #,0,0,3,3]
@seq_seq2 = [1,0,1,0] #,1,0,3,3]
  
@channel ||= 1
@is1 ||= 0
@is2 ||= 0
@i1  ||= 0
@i2  ||= 0

@part = [0,0,0,0,1,1,1,1,2,2,2,2,3,3,4,3,3,5,1,1,3]
@delta ||= 5
def bang
  base = 9
  case @part[(@i1 / (base * 2)) % @part.size]
  when 0
    @delta_done = false
    noteOn 62, 120, 0.1 if @i1 % 5 == 0
    #noteOn @seq[@seq_seq1[@is1]][@i1 % base], 90, 0.2
    #noteOn @seq[@seq_seq2[@is2]][@i2 % base], 80, 0.1
  when 1
    #noteOn @notes[@i % @notes.size], 120, 0.1
    noteOn @seq[@seq_seq1[@is1]][@i1 % base], 90, 0.2
    #noteOn @seq[@seq_seq2[@is2]][@i2 % base], 80, 0.1
  when 2
    @seq_seq1 = [0,0,2,2]
    #noteOn @notes[@i % @notes.size], 120, 0.1
    noteOn @seq[@seq_seq1[@is1]][@i1 % base], 90, 0.2
    noteOn @seq[@seq_seq2[@is2]][@i2 % base], 80, 0.1
  when 3  
    noteOn 62, 120, 0.1
    noteOn @seq[@seq_seq1[@is1]][@i1 % base], 90, 0.2
    noteOn @seq[@seq_seq2[@is2]][@i2 % base], 80, 0.1
  when 4
    noteOn 62, 120, 0.1 if @i1 % 4 == 0
    noteOn 61, 120, 0.1 if @i1 % 3 == 0
  when 5
    unless @delta_done
      @delta = ((@delta + 1) % 7) + 1 
      puts "delta: #{@delta}"
      @delta_done = true
      if @delta == 1
        @seq_seq1 = [1,3,1,3]
      elsif @delta == 7
        @seq_seq1 = [0,2,0,1]
      end
    end
    noteOn 62, 120, 0.1 if @i1 % 4 == 0
    noteOn 61, 120, 0.1 if @i1 % 3 == 0
    noteOn @seq[@seq_seq1[@is1]][@i1 % base], 90, 0.2 if @i1 % 2 == 0
    noteOn @seq[@seq_seq2[@is2]][@i2 % base], 80, 0.1 if @i1 % 2 == 0
  end
  
  @i1 += 1
  @i2 = @i1 + @delta
  
  if @i1 % base == 0
    @is1 = (@is1 + 1) % @seq_seq1.size
  end
  
  if @i2 % base == 0
    @is2 = (@is2 + 1) % @seq_seq2.size
  end
   
end