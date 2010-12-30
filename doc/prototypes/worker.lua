
-- mock
function current_time()
  return os.time() / 10
end

local worker = {events = {}, now = current_time()}

function worker.spawn(func)
  worker.schedule(0, func)
end

function worker.sleep(duration)
  coroutine.yield(duration)
end

-- This should be written in C++
function worker.schedule(duration, func)
  local ev = {when = worker.now + duration, co = coroutine.create(func)}
  table.insert(worker.events, ev)
end

function worker.run()
  worker.schedule(0, worker.main)
  -- double buffering...
  local events = worker.events
  worker.events = {}

  worker.should_quit = false
  -- spin (no sleep)
  print('Start..')

  -- round robin
  while #events > 0 do
    os.execute('sleep 1') -- releases lock
    local now = current_time()
    for i = 1, #events do
      local ev = events[i]
      if ev.when <= now then
        worker.now = ev.when
        local ok, sleep = coroutine.resume(ev.co)
        if not ok then
          -- error
          print(sleep)
        elseif not sleep then
          -- ends normally
        else
          ev.when = ev.when + sleep
          table.insert(worker.events, ev)
        end
      else
        table.insert(worker.events, ev)
      end
      if worker.should_quit then
        return
      end
    end
    events = worker.events
    worker.events = {}
  end
end

function worker.quit()
  worker.should_quit = true
end

local counter = 0

function loop()
  while counter < 50 do
    counter = counter + 1
    -- sleep for another 0.1 ms
    worker.sleep(0.2)
  end
end

function worker.main()
  -- execute function in 0.1 seconds
  worker.schedule(0.2, loop)
  -- pass control (yield)
  -- 0.2 .. 1 .. 0.2 .. 2 .. 0.2 .. 3
  worker.sleep(0.7)

  assert(counter == 3)
  -- remove all scheduled events
  worker.quit()
  print('bye..')
end

worker.run()








