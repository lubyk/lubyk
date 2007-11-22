=begin rdoc
This is the implementation of a machine learning algorythm based on Support Vector Machines and using the
following libraries:
   
- BLAS () fast matrix computations
- LAPACK () used to compute Principal Component Analysis
- libsvm () used to do the pattern recognition

The machine works either in recognition mode or recording mode.

Recording mode
==============

You start the machine with the path to a folder, a filehandle to read the data to be learned, a block for countdown, vector size, move duration and a name for this machine's brain:

ex. path = /Users/gaspard/svm/test,  name = leg_movements, source = SerialIO.new(...), &countdown_block = {|msg| puts msg}, vector_size = 12 [signals], move_duration = 100 [samples]

The source must respond to the following method:
def buffer(vector_size, sample_count)
end

This method returns a pointer to a buffer of vector_size floats containing the last sample_count vectors. If sample_count is 'nil', then the last vector is returned (same as vector_size = 1)


During the learning process, the machine waits for a keypress (key = label). It then starts a countdown by calling the countdown_block with :ready, :set, :go. The recording starts a little before :go and ends a little after :stop so that we have a margin to better fit the window on the signal based on the preceding sequences. Once a signal is recorded, the deviation to the preceding mean value is shown pressing 'delete' removes this last take. Pressing 'esc' quits the recording phase. Pressing any other key starts the recording of the next movement.

When the 'esc' key is pressed, the machine leaves 'recording' mode.

You must call 'learn' to build (or rebuild) the learning database before you can pass to 'Recognition' mode.

This will produce the following files:

- Raw data containing the recorded sequences: /Users/gaspard/svm/test/leg_movements.txt
- Markers containing the labels for the sequences: /Users/gaspard/svm/test/leg_movements.mrk
- Learning database: /Users/gaspard/svm/test/leg_movements.svm

Recognition mode
================

You start the machine with the path to a folder and the 'brain' name:

ex. path = /Users/gaspard/svm/test, name = leg_movements

You then call the method 'label' or 'labels' passing a pointer to the current buffer:

current_movement = @brain.label(@serial.buffer(12, 150))
Note that you have to pass 1.5 times the sample_count used during training to let the machine slide into this space to find the best match.

If you want an array of the possible labels, use 'labels':

labels = @brain.labels(@serial.buffer(12,150)) 

This returns an array of tuples (label, probability) ordered by probability (highest first). The sum of all probabilities is always 1.0.

ex. return value for labels : [['a',0.8], ['d', 0.1], ['b', 0.1]]

=end


path = /Users/gaspard/svm/test,  name = leg_movements, source = SerialIO.new(...), &countdown_block = {|msg| puts msg}, vector_size = 12 [signals], move_duration = 100 [samples]

class MoveRecognition
  attr_accessor :datafh, :markersfh, :databasefh, :
  
  def record(folder, name, source, vector_size, sample_count, &block )
    @folder = folder
    @name   = name
    @vector_size     = vector_size
    @sample_count    = sample_count
    @countdown_block = block
    @source          = source
    @datafh    = File.open(File.join(folder, "#{name}.txt"), 'w')
    @markersfh = File.open(File.join(folder, "#{name}.mrk"), 'w')
    @label     = nil
    # test source
    buf = source.buffer(@vector_size, @sample_count)
    while(@label != "\e")
      record_one_move(getc)
    end
  end
end







