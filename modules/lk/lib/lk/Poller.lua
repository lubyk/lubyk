--[[------------------------------------------------------

  # Default poller

  This poller is used when neither networking or GUI is used. The API of this
  poller shows the interface that any poller must support. The poller handles 
  is the "main" loop.
  
  This is an internal API, usage is done through lk.Scheduler.

  Since Lubyk is single-threaded, we must often pass control to some external
  tool in which case we must write a custom poller. See zmq.Poller and 
  mimas.Poller for examples.

  TODO: This is an empty shell but if we write the C++ code, we could remove
  zmq dependency on lub module.

--]]------------------------------------------------------
local lib = class 'lk.Poller'


-- Flag set when we want to yield when the file descriptor si ready for *Read*
-- operations.
lib.READ_FLAG  = 1

-- Flag set when we want to yield when the file descriptor si ready for *Write*
-- operations.
lib.WRITE_FLAG = 2

-- nodoc
lib.ERROR_FLAG = 4


-- # Constructor

-- Create a poller object ready to register sockets.
function lib.new()
end

-- # Polling for events

-- Poll for new events with a given `timeout` in [s]. Returns false if the poll
-- operation was interrupted (with `^C` or by other means).
function lib:poll(timeout)
end

-- Return the list of events (sockets ready for read/write operations) after a
-- call to #poll. Returns `nil` if no event is available.
-- 
-- The returned table contains a list of thread indices `idx`. The thread
-- identified by `idx` should be resumed.
function lib:events()
end

-- This is called from lk.Scheduler loop in case the poller needs time to
-- run (for example for GUI stuff). This is not the case with this poller
-- where all events are handled by the poller itself so this function does
-- nothing.
function lib:resumeAt(time)
end

-- # Thread management
--
-- In this context "threads" are simply file descriptors on which a coroutine
-- needs to wait before resuming.

-- Add file descriptor `fd` (a number) to the watched files. `event` contains
-- what needs to be watched for this descriptor (lk.Poller.READ_FLAG or
-- lk.Poller.WRITE_FLAG).
--
-- This function returns an identifier `idx` that should be used to reference
-- the thread linked to the file descriptor. `idx` is an opaque value.
function lib:add(fd, event)
end

-- Remove a thread from the watched events. `idx` must be the thread identifier
-- as returned by #add.
function lib:remove(idx)
end

-- Modifiy a thread's watched events. Read/write operations in sockets should
-- be done in a way as to minimize calls to this function. For example avoid
-- reading and writing from the same thread if writing could block.
function lib:modify(idx, event)
end
