require 'lubyk'
require 'sched'
require 'timer'

timer = Timer(200, function()
  print('hop')
end)

timer:start()

sched:run()
