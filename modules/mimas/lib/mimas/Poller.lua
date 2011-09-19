--[[------------------------------------------------------

  mimas.Poller
  ------------

  This is used to bridge lk.Scheduler with Qt's event
  loop.

--]]------------------------------------------------------
require 'mimas.SocketNotifier'
local lib    = {type = 'mimas.Poller'}
lib.__index  = lib
mimas.Poller = lib
local private = {}

setmetatable(lib, {
  -- Create a new mimas.Poller. This is used by mimas.Application.
  __call = function(lib)
    local self = {}
    self.timeout_callback = mimas.Callback(function()
      private.resume(self)
    end)

    self.co = coroutine.create(function()
      assert(sched.poller == self)
      -- run scheduler (poll operation will yield)
      sched:loop()
    end)

    -- This will start the coroutine
    app:singleShot(0, self.timeout_callback)
    return setmetatable(self, lib)
  end
})

local zmq_to_mimas_ev = {
  [zmq.POLLIN]  = mimas.SocketNotifier_const.Read,
  [zmq.POLLOUT] = mimas.SocketNotifier_const.Write,
}

function private.makeNotifier(self, fd, event)
  local notifier = mimas.SocketNotifier(fd, zmq_to_mimas_ev[event])
  notifier.fd    = fd
  notifier.event = event
  notifier.poller = self
  notifier.callback = private.socketCallback
  return notifier
end

-- This callback is called whenever we have an event for a given
-- socket (we simulate poll return)
function private.socketCallback(notifier)
  local self = notifier.poller
  -- Add idx to events
  self.idx = notifier
  private.resume(self)
end

function private.zmqSocketCallback(notifier)
  local sock = notifier.zmq_socket
  -- Empty inbox (we only receive a call if the inbox has been emptied)
  while sock:hasEvent(notifier.event) do
    private.socketCallback(notifier)
  end
end

function private:resume()
  local co = self.co
  local ok, timeout = coroutine.resume(co, true)
  if not ok then
    -- error
    print('ERROR', timeout, debug.traceback(co))
    app:quit()
  elseif coroutine.status(co) == 'dead' then
    app:quit()
  elseif timeout >= 0 then
    app:singleShot(timeout, self.timeout_callback)
  end
end

--- Adds a filedescriptor or zmq.Socket to the poll items.
-- @return a unique id for this element.
function lib:add(fd, event)
  local notifier
  if type(fd) == 'userdata' then
    notifier = private.makeNotifier(self, fd:fd(), event)
    notifier.callback = private.zmqSocketCallback
    notifier.zmq_socket = fd
  else
    notifier = private.makeNotifier(self, fd, event)
  end
  notifier[event]  = notifier
  notifier.current = notifier
  return notifier
end

--- Modify the events (zmq.POLLIN, zmq.POLLOUT) for an fd identified
-- by its id.
function lib:modify(notifier, event)
  local current = notifier.current
  if current.event == event then
    return
  end
  -- disable current
  current:setEnabled(false)
  -- enable
  local n = notifier[event]
  if not n then
    n = private.makeNotifier(self, notifier.fd, event)
    notifier[event] = n
  else
    n:setEnabled(true)
  end
  notifier.current = n
end

--- Remove a filedescriptor of zmq.Socket from the list of poll items.
-- The item is identified by its id.
function lib:remove(notifier)
  notifier.current:setEnabled(false)
  -- we let it be removed by garbage collector.
end

--- Poll for new events with a given timeout in ms.
-- @return false to quit/interrupt.
function lib:poll(timeout)
  return coroutine.yield(timeout) 
end

--- Return next event signaled in poll.
function lib:event()
  if self.idx then
    local event = self.idx
    self.idx = nil
    return event
  end
  return nil
end


