require 'lubyk'
t = {}
t.threads  = {}
t.services = {}

math.randomseed(os.time())

local service_count = 5
for i=1,service_count do
  t.threads[i] = lk.Thread(function()
    local sl = 400 + 1000 * math.random()
    print(sl)
    io.flush()
    sleep(sl)
    print('new', string.format('Process %i', i))
    t.services[i] = lk.Service(string.format('Process %i', i))
    sl = 1400 + 5000 * math.random()
    print(sl)
    io.flush()
    sleep(sl)
    print('kill', string.format('Process %i', i))
    t.services[i]:kill()
  end)
end

for i=1,service_count do
  t.threads[i]:join()
end