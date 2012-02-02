--[[------------------------------------------------------

  mimas.GLSLWidget
  --------------

  A context to draw modern OpenGL content.

--]]------------------------------------------------------
local mt         = mimas_core.GLSLWidget_
mimas.GLSLWidget_ = mt
local close  = mt.close

local constr  = mimas_core.GLSLWidget
function mimas.GLSLWidget(...)
  return mimas.bootstrap('GLSLWidget', constr, ...)
end

--=============================================== COPY CODE FROM Widget
local addWidget = mt.addWidget
function mt:addWidget(other, ...)
  addWidget(self, other:widget(), ...)
end

local setParent = mt.setParent
function mt:setParent(other, ...)
  setParent(self, other:widget(), ...)
end

local addLayout = mt.addLayout
function mt:addLayout(other)
  addLayout(self, other:layout())
end

-- default keyboard action
-- TODO: add CMD+W
function mt:keyboard(key, on)
  if on then
    if key == mimas.ESC then
      -- ESC
      self:close()
    elseif key == mimas.Space then
      self:swapFullScreen()
    end
  end
end

local close  = mt.close
function mt:close()
  if self:deleted() then
    return false
  else
    close(self)
    return true
  end
end

function mt:center()
  local w, h = app:screenSize()
  local sw, sh = self:size()
  self:move((w - sw) / 2, (h - sh) / 2)
end

--=============================================== DEFAULT METHODS
function mt:initializeGL()
  gl.Enable("POINT_SMOOTH")
  gl.Enable("SMOOTH")
  gl.Enable("BLEND")                                -- Enable alpha blending
  gl.BlendFunc("SRC_ALPHA", "ONE_MINUS_SRC_ALPHA")  -- Set blend function

  gl.ClearDepth(1.0)
  gl.DepthFunc("LEQUAL");

  -- gl.Enable('GL_CULL_FACE')
  -- gl.Enable('GL_DEPTH_TEST')

  gl.Hint("PERSPECTIVE_CORRECTION_HINT", "NICEST") -- Really nice perspective
  gl.ClearColor(0.2,0.2,0.2,0.5)
end

function mt:resizeGL(w, h)
  self.w = w
  self.h = h
  gl.Enable("BLEND")
  --gl.Disable("DEPTH_TEST")
  gl.BlendFunc("SRC_ALPHA", "ONE_MINUS_SRC_ALPHA")

  gl.Enable("LINE_SMOOTH")
  -- Select the projection matrix
  gl.MatrixMode("PROJECTION")
  -- reset
  gl.LoadIdentity()
  -- Calculate the aspect ratio of the view
  glu.Perspective(
  45,               -- Field of view angle
  w / h,   -- Aspect ration
  1,                -- zNear
  100               -- zFar
  )
end

