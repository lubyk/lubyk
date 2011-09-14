--[[------------------------------------------------------

  lk.Scheduler
  ------------

  Main event loop management. When we use a graphical
  interface, the scheduler automatically passes the
  loop to the GUI.

--]]------------------------------------------------------
local lib     = {}
lib.__index   = lib
lk.Scheduler  = lib
local private = {}

setmetatable(lib, {
  __call = function()
    local self = {
      -- Ordered linked list of events by event time.
      at_next    = nil,
      -- List of threads that have added their filedescriptors to
      -- select.
      fd_count   = 0,
      fd_read    = {},
      fd_write   = {},
      -- These are plain lua functions that will be called when
      -- quitting.
      finalizers = {},
      -- Using zmq for polling
      poller = zmq.poller.new(3),
    }
    return setmetatable(self, lib)
  end
})

--- Sleeps a given number of milliseconds starting from the
-- current time (not the logical time). This should not be used as a
-- timer because it will drift.
function lib:sleep(delay)
  if delay == 0 then
    coroutine.yield('wait', 0)
  else
    coroutine.yield('wait', worker:now() + delay)
  end
end

--- Waits a given number of milliseconds starting from the
-- logical time. This should be used as a timer because it will not
-- drift.
function lib:wait(delay)
  if delay == 0 then
    coroutine.yield('wait', 0)
  else
    coroutine.yield('wait', self.now + delay)
  end
end

function lib:waitRead(fd)         
  coroutine.yield('read', fd)
end

function lib:waitWrite(fd)         
  coroutine.yield('write', fd)
end

function lib:run(func)
  if func then
    self.main = lk.Thread(func)
  end
  while true do
    local now = worker:now()
    local now_list = {}
    local timeout = -1
    local next_thread = self.at_next
    while next_thread and next_thread.at < now do
      -- collect
      table.insert(now_list, next_thread)
      -- get next
      next_thread = next_thread.at_next
    end
    -- remove from at_list
    self.at_next = next_thread

    for i, thread in ipairs(now_list) do
      -- We run them all now so that we give time for 'select' in
      -- case they keep on adding new events in at_next list.
      private.runThread(self, thread)
    end

    -- the running thread might have added new elements
    next_thread = self.at_next

    if next_thread then
      if next_thread.at < worker:now() then
        timeout = 0
      else
        -- timeout
        -- our timeout is in ms
        -- zmq is in us ==> 1000*
        timeout = 1000 * (next_thread.at - now)
      end
    end
      
    if self.fd_count == 0 and timeout == -1 then
      -- done
      break
    else
      local status, err = self.poller:poll(timeout)
      if not status then
        print("ERROR", err)
        break
      end
    end
  end
  private.finalize(self)
end

function lib:scheduleAt(at, thread)
  -- We use the wrapper internally so that we do not prevent
  -- garbage collection.
  thread.at = at
  -- sorted insert (next event first)
  local prev = self
  while true do
    local ne = prev.at_next
    if not ne then
      prev.at_next = thread
      thread.at_next = nil
      break
    elseif at < ne.at then
      prev.at_next   = thread
      thread.at_next = ne
      break
    else
      prev = ne
    end
  end
end
--=============================================== PRIVATE

local zmq_POLLIN, zmq_POLLOUT = zmq.POLLIN, zmq.POLLOUT

function private:runThread(thread)
  -- get thread from wrap
  local t = thread.t.t
  if not t or not t.co then
    -- has been killed or gc
    if thread.fd then
      private.removeFd(self, thread)
      thread.fd = nil
    end
    -- let it repose in peace
    if t then
      t:finalize(self)
    end
    return
  end
  if thread.at and thread.at > 0 then
    sched.now = thread.at
  else
    sched.now = worker:now()
  end                
  -- FIXME: pcall ?
  thread.at = nil
  local ok, a, b = coroutine.resume(t.co)
  if not ok then
    -- a = error
    if thread.fd then
      private.removeFd(self, thread)
    end
    print('ERROR', a, debug.traceback(t.co))
  elseif a == 'read' then
    if thread.fd then
      -- same ?
      if thread.events == zmq_POLLIN then
        -- done
      else
        -- update
        self.fd_read[b]  = thread
        self.fd_write[b] = nil
        self.poller:modify(b, zmq_POLLIN, t.cb)
      end
    else
      -- add fd
      thread.fd = b
      self.fd_read[b]  = thread
      self.fd_count    = self.fd_count + 1
      thread.events = zmq_POLLIN
      function t.cb()
        private.runThread(self, thread)
      end
      self.poller:add(b, thread.events, t.cb)
    end
  elseif a == 'write' then
    if thread.fd then
      -- same ?
      if thread.events == zmq_POLLOUT then
        -- done
      else
        -- update
        self.fd_read[b]  = nil
        self.fd_write[b] = thread
        self.poller:modify(b, zmq_POLLOUT, t.cb)
      end
    else
      -- add fd
      thread.fd = b
      self.fd_write[b] = thread
      self.fd_count    = self.fd_count + 1
      thread.events = zmq_POLLOUT
      function thread.cb()
        private.runThread(self, thread)
      end
      self.poller:add(b, thread.events, t.cb)
    end
  elseif a == 'join' then
    if thread.fd then
      private.removeFd(self, thread)
    end
    b:addJoin(thread)
  elseif a == 'wait' then
    if thread.fd then
      private.removeFd(self, thread)
    end
    -- b = at
    -- a value of 0 == execute again as soon as possible
    self:scheduleAt(b, thread)
  elseif not t.co or coroutine.status(t.co) == 'dead' then
    if thread.fd then
      private.removeFd(self, thread)
    end
    -- let it repose in peace

    thread.fd = nil
    t.co = nil
    t:finalize(self)
  else
    if thread.fd then
      private.removeFd(self, thread)
    end
    print('BAD YIELD:', a, b)
  end
end


function private:finalize()
  for _, func in ipairs(self.finalizers) do
    -- if one fails, should not crash all finalizers...
    pcall(func)
  end
end

function private.removeFd(self, thread)
  local fd = thread.fd
  if self.fd_read[fd] then
    self.fd_count = self.fd_count - 1
    self.fd_read[fd] = nil
  elseif self.fd_write[fd] then
    self.fd_count = self.fd_count - 1
    self.fd_write[fd] = nil
  end
  thread.fd = nil
end
--=============================================== Without mimas

--=============================================== With mimas
