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
      fd_list    = {n=0},
      -- These are plain lua functions that will be called when
      -- quitting.
      finalizers = {},
      -- Using zmq for polling
      poller = zmq.poller.new(3),
    }
    return setmetatable(self, lib)
  end
})

function lib:thread(func)
  local thread = {
    co = coroutine.create(func),
  }

  private.scheduleAt(self, 0, thread)
  return thread
end

function lib:wait(delay)
  if delay == 0 then
    coroutine.yield(0)
  else
    coroutine.yield(self.now + delay)
  end
end

function lib:waitRead(fd)         
  coroutine.yield(fd, 'read')
end

function lib:run()
  local fd_list = self.fd_list
  while true do
    local now = worker:now()
    local now_list = {}
    local timeout = -1
    local thread = self.at_next
    while thread and thread.at < now do
      -- remove from at_list
      self.at_next = thread.at_next
      -- collect
      table.insert(now_list, thread)
      thread = self.at_next
    end
    self.at_next = thread

    for i, thread in ipairs(now_list) do
      -- We run them all now so that we give time for 'select' in
      -- case they keep on adding new events in at_next list.
      private.runThread(self, thread)
    end

    if self.at_next then
      -- timeout
      -- our timeout is in ms
      -- zmq is in us ==> 1000*
      timeout = 1000 * (self.at_next.at - now)
    end
      
    if fd_list.n == 0 and timeout == -1 then
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

function lib:addFd(thread, fd)
  local fd_list = self.fd_list
  thread.fd = fd
  table.insert(fd_list, thread)
  fd_list.n = fd_list.n + 1
  self.poller:add(fd, zmq.POLLIN + zmq.POLLOUT, function()
    private.runThread(self, thread)
  end)
end

function lib:removeFd(fd)
  local fd_list = self.fd_list
  self.poller:remove(fd)
  for i, thread in ipairs(self.fd_list) do
    if thread.fd == fd then
      table.remove(self.fd_list, i)
      fd_list.n = fd_list.n - 1
      break
    end
  end
end
--=============================================== PRIVATE

function private:runThread(thread)
  if thread.at and thread.at > 0 then
    self.now = thread.at
  else
    self.now = worker:now()
  end                
  -- FIXME: pcall ?
  thread.at = nil
  local ok, a, b = coroutine.resume(thread.co)
  if not ok then
    -- a = error
    if thread.fd then
      self:removeFd(thread.fd)
    end
    print('ERROR', a)
  elseif b then
    --thread.fd = a
    --thread.op = b -- read/write/error
    -- add thread to select
    -- FIXME: merge fdSet + insert
    -- worker:fdReadSet(thread.fd)
    -- worker.fdSet[b](worker, a)
  elseif coroutine.status(thread.co) == 'dead' then
    -- let it repose in peace
    if thread.fd then
      -- remove from poller
      self:removeFd(thread.fd)
    end
    thread.fd = nil
    thread.co = nil
  else
    -- a = at
    -- a value of 0 == execute again as soon as possible
    if a then
      private.scheduleAt(self, a, thread)
    end
  end
end

function private:scheduleAt(at, thread)
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

function private:finalize()
  for _, func in ipairs(self.finalizers) do
    -- if one fails, should not crash all finalizers...
    pcall(func)
  end
end

--=============================================== Without mimas

--=============================================== With mimas
