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
      should_run = true,
      -- Ordered linked list of events by event time.
      at_next    = nil,
      -- List of threads that have added their filedescriptors to
      -- select.
      fd_count      = 0,
      idx_to_thread = {},
      -- These are plain lua functions that will be called when
      -- quitting.
      finalizers    = {},
      -- Using zmq for polling
      poller        = zmq.Poller(),
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

  if not rawget(mimas, 'Application') then
    -- without mimas
    self:loop()
  end

  if self.should_run or self.restart_with_mimas then
    self.restart_with_mimas = nil
    -- restarting
    self.should_run = true
    -- now we are with mimas
    self.mimas = true
    private.usePoller(self, mimas.Poller())
    app:exec()
  end
  private.finalize(self)
end

function lib:loop()
  local poller = self.poller
  local idx_to_thread = self.idx_to_thread
  while self.should_run do
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
        timeout = next_thread.at - now
      end
    end
      
    if self.fd_count == 0 and timeout == -1 and not self.mimas then
      -- done
      self.should_run = false
    else
      if not poller:poll(timeout) then
        -- interrupted
        print("\nBye...")
        break
      end
      while true do
        local ev_idx = poller:event()
        if ev_idx then
          local thread = idx_to_thread[ev_idx]
          private.runThread(self, thread)
        else
          break
        end
      end

    end
  end
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

function lib:removeFd(thread)
  local fd = thread.fd
  self.idx_to_thread[thread.idx] = nil
  if fd then
    self.poller:remove(thread.idx)
    self.fd_count = self.fd_count - 1
  end
  thread.idx = nil
  thread.fd  = nil
end

function lib:mimasLoaded()
  if coroutine.running() then
    coroutine.yield('mimas')
    -- we will continue here when triggered from within
    -- app:exec()
  end
end

--=============================================== PRIVATE

local zmq_POLLIN, zmq_POLLOUT = zmq.POLLIN, zmq.POLLOUT
local zmq_const = {read = zmq_POLLIN, write = zmq_POLLOUT}

function private:usePoller(new_poller)
  self.poller = new_poller
  local idx_to_thread = self.idx_to_thread
  self.idx_to_thread = {}
  -- add all filedescriptors
  for idx, thread in pairs(idx_to_thread) do
    thread.idx = new_poller:add(thread.fd, thread.event)
    self.idx_to_thread[thread.idx] = thread
  end
end

function private:runThread(thread)
  -- get thread from wrap
  local t = thread.t.t
  if not t or not t.co then
    -- has been killed or gc
    if thread.fd then
      self:removeFd(thread)
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
  local event = zmq_const[a]
  if not ok then
    -- a = error
    if thread.fd then
      self:removeFd(thread)
    end
    print('ERROR', a, debug.traceback(t.co))
  elseif event then
    if thread.fd then
      if thread.fd == b then
        -- same
        self.poller:modify(thread.idx, event)
      else
        -- changed fd
        self.poller:modify(thread.idx, event, b)
      end
    else
      -- add fd
      thread.fd = b
      self.fd_count = self.fd_count + 1
      thread.idx = self.poller:add(b, event)
      self.idx_to_thread[thread.idx] = thread
    end
    -- We need this information in case we change poller.
    thread.event = event
  elseif a == 'join' then
    if thread.fd then
      self:removeFd(thread)
    end
    b:addJoin(thread)
  elseif a == 'wait' then
    if thread.fd then
      self:removeFd(thread)
    end
    -- b = at
    -- a value of 0 == execute again as soon as possible
    self:scheduleAt(b, thread)
  elseif not t.co or coroutine.status(t.co) == 'dead' then
    if thread.fd then
      self:removeFd(thread)
    end
    -- let it repose in peace

    thread.fd = nil
    t.co = nil
    t:finalize(self)
  elseif a == 'mimas' then
    self.should_run = false
    self.restart_with_mimas = true
  else
    if thread.fd then
      self:removeFd(thread)
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

--=============================================== Without mimas

--=============================================== With mimas
