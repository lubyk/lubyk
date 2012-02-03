require 'lubyk'

io.write('concurrent: ')
io.flush()
-- concurrent execution of the two loops
local pid = worker:spawn[[
require 'lubyk'
for i=1,6 do
  sleep(20)
  io.write('.')
  io.flush()
end
worker:exit(5)
print('WORKER DONE.')
]]

print("STARTED", pid)

for i=1,5 do
  sleep(20)
  io.write('x')
  io.flush()
end
-- wait for pid to finish
print(worker:waitpid(pid))
print('DONE.')
