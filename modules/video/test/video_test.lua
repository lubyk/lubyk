--[[------------------------------------------------------

  video test
  ----------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('video')

function should.auto_load()
  assert_true(video)
end

function should.open_camera(t)
  local camera = video.Camera()
  function camera.receive(frame)
    -- received video frame
    assert_match('xxx', frame.__tostring())
    camera:close()
  end

  while camera:is_open() do
    sleep(10)
  end
end

test.all()
