require 'lk.more_c'

-- Load defaults in 'Lubyk'
require 'lk.Lubyk'

lubyk = Autoload('lubyk')

-------------------------------- CONSTANTS
lubyk.service_type    = Lubyk.service_type

-- Get content at path (used with lk.Morph)
lubyk.get_url         = '/lk/get'
-- Get patch basic information
lubyk.info_url        = '/lk/info'
-- Get all patch information at once
lubyk.dump_url        = '/lk/dump'
-- (Partial) update of a patch
lubyk.update_url      = '/lk/update'
-- Execute method (actions depend on the specific service)
lubyk.execute_url     = '/lk/do' -- action, params
-- Quit process
lubyk.quit_url        = '/lk/quit'

-- Notification for new service (in-process)
lubyk.add_service_url = '/lk/add_service'
-- Notification for removed service (in-process)
lubyk.rem_service_url = '/lk/rem_service'

require 'lk.Scheduler'
sched = lk.Scheduler()
function sleep(ms)
  sched:sleep(ms)
end

function run(...)
  sched:run(...)
end

require 'lk.util'

