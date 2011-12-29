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
local app    = app
local private = {}

setmetatable(lib, {
  -- Create a new mimas.Poller. This is used by mimas.Application.
  __call = function(lib)
    local self = {
      -- FIXME: REMOVE THIS !!! (SHOULD NOT BE NEEDED)
      notifiers = {},
      timer     = mimas.Timer(0)
    }

    function self.timer.timeout()
      private.resume(self)
    end

    self.co = coroutine.create(function()
      assert(sched.poller == self)
      -- run scheduler (poll operation will yield)
      sched:loop()

      -- We need this to quit
      --app:quit()
    end)

    -- This will start the coroutine
    self.timer:start(0)
    return setmetatable(self, lib)
  end
})

local zmq_to_mimas_ev = {
  [zmq.POLLIN]  = mimas.SocketNotifier_const.Read,
  [zmq.POLLOUT] = mimas.SocketNotifier_const.Write,
}

function private.makeNotifier(self, fd, event)
  local list = self.notifiers[event]
  if not list then
    list = {}
    self.notifiers[event] = list
  end
  local notifier = list[fd]
  if not notifier then
    notifier = mimas.SocketNotifier(fd, zmq_to_mimas_ev[event])
    list[fd] = notifier
    notifier.fd    = fd
    notifier.event = event
    notifier.poller = self
    notifier.activated = private.socketCallback
  end
  return notifier
end

-- This callback is called whenever we have an event for a given
-- socket (we simulate poll return)
function private.socketCallback(notifier)
  private.resume(notifier.poller, notifier)
end

function private.zmqSocketCallback(notifier)
  local sock = notifier.zmq_socket
  -- We have to check for the correct event (sometimes it fires too soon)
  if sock:hasEvent(notifier.event) then
    private.socketCallback(notifier)
  end
end

-- Resume poller
function private:resume(idx)
  local co = self.co
  if not co then
    return
  end
  self.idx = idx
  local ok, timeout = coroutine.resume(co, true)
  if not ok then
    -- error
    print('ERROR', timeout, debug.traceback(co))
    app:quit()
  elseif coroutine.status(co) == 'dead' then
    self.co = nil
    app:quit()
  elseif timeout >= 0 then
    self.timer:start(timeout)
  else
    self.timer:stop()
  end
end

--- Adds a filedescriptor or zmq.Socket to the poll items.
-- @return a unique id for this element.
function lib:add(fd, event)
  local notifier
  if type(fd) == 'userdata' then
    notifier = private.makeNotifier(self, fd:fd(), event)
    notifier.activated = private.zmqSocketCallback
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
  if not notifier:deleted() then
    notifier:setEnabled(false)
  end
end

--- Poll for new events with a given timeout in ms.
-- @return false to quit/interrupt.
function lib:poll(timeout)
  return coroutine.yield(timeout) 
end

--- Return list of events signaled in poll.
function lib:events()
  if self.idx then
    local event = self.idx
    self.idx = nil
    return {event}
  end
  return nil
end

