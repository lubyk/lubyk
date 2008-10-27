function ShowArray(v)
  if(v == nil) then  f:write( ' - nil array - ') return end
  f:write( '{ ')
  for i, a in pairs(v) do
    f:write(a .. ',')
  end
  f:write( '}\n')
end
function Compare(n1, n2)
  if(n1 == nil or n2 == nil) then
     f:write('Compare Error (nil value)!\n')
  elseif(n1 == n2) then
     f:write('Ok.\n')
  else
     f:write('Compare Error!\n')
  end
end
function CompareF(n1, n2)
  if(n1 == nil or n2 == nil) then
     f:write('CompareF Error!\n')
  elseif( math.abs(n1 - n2) < 0.000001) then
     f:write('Ok.\n')
  else
     f:write('CompareF Error!\n')
  end
end
function CompareC(n1, n2)
  if(n1 == nil or n2 == nil) then
     f:write('CompareC Error!\n')
  elseif( math.abs(n1 - n2) < 0.05) then
     f:write('Ok.\n')
  else
     f:write('CompareC Error!\n')
  end
end
function CompareArrayF(v1, v2)
  if(v1 == nil or v2 == nil) then
    f:write('Test Error!\n')
	return
  end
  for i, v in pairs(v2) do
    if( v1[i] == nil ) then
       f:write('Test Error!\n')
       return
    elseif( math.abs(v - v1[i]) > 0.000001) then
       f:write('Test Error!\n')
       return
    end
  end
  for i, v in pairs(v1) do
    if( v2[i] == nil ) then
       f:write('Test Error!\n')
       return
    elseif( math.abs(v - v2[i]) > 0.000001) then
       f:write('Test Error!\n')
       return
    end
  end
  f:write('Ok.\n')
end
function CompareArrayC(v1, v2)
  if(v1 == nil or v2 == nil) then
    f:write('Test Error!\n')
    return
  end
  for i, v in pairs(v2) do
    if( v1[i] == nil ) then
      f:write('Test Error!\n')
      return
    elseif( math.abs(v - v1[i]) > 0.05) then
      f:write('Test Error!\n')
      return
    end
  end
  for i, v in pairs(v1) do
    if( v2[i] == nil ) then
      f:write('Test Error!\n')
      return
    elseif( math.abs(v - v2[i]) > 0.05) then
      f:write('Test Error!\n')
      return
    end
  end
  f:write('Ok.\n')
end
function CompareArray(v1, v2)
  for i, v in pairs(v2) do
    if( v1[i] == nil ) then
	   f:write('Test Error!\n')
	   return
    elseif( v ~= v1[i]) then
       f:write('Test Error!\n')
	   return
	end
  end
  for i, v in pairs(v1) do
    if( v2[i] == nil ) then
	   f:write('Test Error!\n')
	   return
    elseif( v ~= v2[i] ) then
       f:write('Test Error!\n')
	   return
	end
  end
  f:write('Ok.\n')
end
function TestTexture(image, n, target, iformat, format, level, priorities)
  test = gl.GenTextures(n)

  for i=1,n do
    if(gl.IsTexture(test[i]) == true) then
      f:write('Test Error (1)!\n')
      return
    end

    gl.BindTexture(target, test[i])
    gl.TexImage(level, iformat, format, image)
    testimage = gl.GetTexImage(target, level, format)

    testiformat = gl.GetTexLevelParameter(target, level, 'TEXTURE_INTERNAL_FORMAT')
    Compare(testiformat, iformat)

    if(target == 'TEXTURE_1D') then
      testwidth = gl.GetTexLevelParameter(target, level, 'TEXTURE_WIDTH')
      Compare(testwidth, table.getn(image)/iformat)
      CompareArrayC(image, testimage)

      image2 = {}
      k = 3*math.ceil(testwidth /6)
      for x=1,k do
        image2[x] = image[x] / 2
      end
      
      gl.TexSubImage(level, format, image2, 0)
      testimage = gl.GetTexImage(target, level, format)
      
      for x=1,k do
        CompareC(testimage[x], image2[x])
      end

      gl.TexSubImage(level, format, image2, 1)
      testimage = gl.GetTexImage(target, level, format)
      
      for x=1,k do
        CompareC(testimage[x+3], image2[x])
      end
     
    elseif(target == 'TEXTURE_2D') then
      testwidth = gl.GetTexLevelParameter(target, level, 'TEXTURE_WIDTH')
      testheight = gl.GetTexLevelParameter(target, level, 'TEXTURE_HEIGHT')
      Compare(testwidth, table.getn(image[1]) / iformat )
      Compare(testheight, table.getn(image))

      image2 = CreateArray(image)
      CompareArrayC(image2, testimage)
    end
    if(gl.IsTexture(test[i]) == false) then
      f:write('Test Error (2)!\n')
      return
    end

  end
  gl.PrioritizeTextures(test, priorities)
  for i=1,n do
    gl.BindTexture(target, test[i])
    testpriority = gl.GetTexParameter(target, 'TEXTURE_PRIORITY')
    CompareC(testpriority, priorities[i])
  end
  gl.DeleteTextures(test)
  for i=1,n do
    if(gl.IsTexture(test[i]) == true) then
      f:write('Test Error (3)!\n')
      return
    end
  end
  f:write('Ok.\n')
end
function TestTexParameter(target, pname, params)
  gl.TexParameter(target, pname, params)
  test = gl.GetTexParameter(target, pname)

  if pname == 'TEXTURE_BORDER_COLOR' then
    CompareArrayF(test, params)
  elseif pname == 'TEXTURE_PRIORITY' then
    CompareF(test, params)
  else
    Compare(test, params)
  end
end
function TestTexGen(coord, pname, params)
  gl.TexGen(coord, pname, params)
  test = gl.GetTexGen(coord, pname, params)

  if(pname == 'TEXTURE_GEN_MODE') then
    Compare(test, params)
  else
    CompareArrayF(test, params)
  end
end
function TestTexEnv(pname, params)
  gl.TexEnv(pname, params)
  test = gl.GetTexEnv(pname)

  if(pname == 'TEXTURE_ENV_MODE') then
    Compare(test, params)
  else
    CompareArrayF(test, params)
  end
end
function MultMatrix(m1, m2)
  m = {}

  for i=1,4  do
  for j=1,4  do
    k = (i-1)*4;
    m[k+j] = (m2[k+1] * m1[0+j]) + (m2[k+2] * m1[4+j]) +  (m2[k+3] * m1[8+j]) + (m2[k+4] * m1[12+j])
  end
  end

  return m
end
function TestMatrix(matrix, tx, ty, tz, sx, sy, sz, ra, rx, ry, rz,
	            fleft, fright, fbottom, ftop, fnear, ffar,
		    oleft, oright, obottom, otop, onear, ofar)

  identity =  { 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1}
  translate = { 1,0,0,0,  0,1,0,0,  0,0,1,0,  tx,ty,tz,1}
  scale =     { sx,0,0,0, 0,sy,0,0, 0,0,sz,0, 0,0,0,1}

  A = (fright + fleft) / (fright - fleft)
  B = (ftop + fbottom) / (ftop - fbottom)
  C = - (ffar + fnear) / (ffar - fnear)
  D = - 2 * ffar * fnear / (ffar - fnear)

  frustum = { 2 * fnear / (fright - fleft), 0, 0, 0,
              0, 2 * fnear / (ftop - fbottom), 0, 0,
	      A, B, C, -1,
	      0, 0, D, 0 }

  A = - (oright + oleft) / (oright - oleft)
  B = - (otop + obottom) / (otop - obottom)
  C = - (ofar + onear) / (ofar - onear)
  
  ortho = { 2 / (oright - oleft), 0, 0, 0,
            0, 2 / (otop - obottom), 0, 0,
	    0, 0, -2 / (ofar - onear), 0,
	    A, B, C, 1 }

  n = math.sqrt( rx^2 + ry^2 + rz^2 )
  x = rx / n
  y = ry / n
  z = rz / n

  A = { x^2, x*y, x*z,
        x*y, y^2, y*z,
        x*z, y*z, z^2}
	    
  B = { 1-(x^2),     x*y,     x*z,
            x*y, 1-(y^2),     y*z,
	    x*z,     y*z, 1-(z^2)}
	    
  C = {  0, z,-y,
        -z, 0, x,
         y,-x, 0}

  rotate = {}
  
  cos = math.cos(ra * 2*math.pi / 360)
  sin = math.sin(ra * 2*math.pi / 360)

  for i=1,4 do
  for j=1,4 do
    k1 = (i-1) * 4 + j
    if(i == 4 or j == 4) then rotate[k1] = 0
    else
      k2 = (i-1) * 3 + j
      rotate[k1] = A[k2] + (cos * B[k2]) + (sin * C[k2])
    end
  end
  end
  rotate[16] = 1

  gl.MatrixMode('MODELVIEW')

  -- test gl.LoadMatrix
  gl.LoadMatrix(matrix)
  test = gl.GetArray('MODELVIEW_MATRIX')
  CompareArrayF(test, matrix)

  -- test gl.LoadIdentity
  gl.LoadIdentity()
  test = gl.GetArray('MODELVIEW_MATRIX')
  CompareArrayF(test, identity)

  -- test gl.Rotate, gl.MultMatrix, gl.GetArray, gl.PopMatrix, gl.PushMatrix
  gl.PushMatrix()
    gl.Rotate(ra, rx, ry, rz)
    test2 = gl.GetArray('MODELVIEW_MATRIX')
  gl.PopMatrix()
  gl.MultMatrix(rotate)
  test3 = gl.GetArray('MODELVIEW_MATRIX')
  CompareArrayF(test2, test3)

  -- test gl.Frustum, gl.MultMatrix, gl.GetArray, gl.PopMatrix, gl.PushMatrix
  gl.PushMatrix()
    gl.Frustum(fleft, fright, fbottom, ftop, fnear, ffar)
    test2 = gl.GetArray('MODELVIEW_MATRIX')
  gl.PopMatrix()
  gl.MultMatrix(frustum)
  test3 = gl.GetArray('MODELVIEW_MATRIX')
  CompareArrayF(test2, test3)

  -- test gl.Ortho, gl.MultMatrix, gl.GetArray, gl.PopMatrix, gl.PushMatrix
  gl.PushMatrix()
    gl.Ortho(oleft, oright, obottom, otop, onear, ofar)
    test2 = gl.GetArray('MODELVIEW_MATRIX')
  gl.PopMatrix()
  gl.MultMatrix(ortho)

  test3 = gl.GetArray('MODELVIEW_MATRIX')
  CompareArrayF(test2, test3)

  test = gl.GetArray('MODELVIEW_MATRIX')

  -- test gl.Translate, gl.MultMatrix, gl.GetArray, gl.PopMatrix, gl.PushMatrix
  gl.PushMatrix()
    gl.Translate(tx, ty, tz)
    test2 = gl.GetArray('MODELVIEW_MATRIX')
    test3 = MultMatrix(test, translate)
    CompareArrayF(test2, test3)
  gl.PopMatrix()
  gl.MultMatrix(translate)
  test4 = gl.GetArray('MODELVIEW_MATRIX')
  test = gl.GetArray('MODELVIEW_MATRIX')
  CompareArrayF(test3, test4)

  test = gl.GetArray('MODELVIEW_MATRIX')

  -- test gl.Scale, gl.MultMatrix, gl.GetArray, gl.PopMatrix, gl.PushMatrix
  gl.PushMatrix()
    gl.Scale(sx, sy, sz)
    test2 = gl.GetArray('MODELVIEW_MATRIX')
    test3 = MultMatrix(test, scale)
    CompareArrayF(test2, test3)
  gl.PopMatrix()
  gl.MultMatrix(scale)
  test4 = gl.GetArray('MODELVIEW_MATRIX')
  CompareArrayF(test3, test4)

  test = gl.GetArray('MODELVIEW_MATRIX')

  -- test gl.Translate, gl.MultMatrix, gl.GetArray, gl.PopMatrix, gl.PushMatrix
  gl.PushMatrix()
    gl.Translate(tx, ty, tz)
    test2 = gl.GetArray('MODELVIEW_MATRIX')
    test3 = MultMatrix(test, translate)
    CompareArrayF(test2, test3)
  gl.PopMatrix()
  gl.MultMatrix(translate)
  test4 = gl.GetArray('MODELVIEW_MATRIX')
  CompareArrayF(test3, test4)
end
function TestIndexPointer(indexArray)
  gl.EnableClientState('INDEX_ARRAY')
  gl.IndexPointer(indexArray)

  c = 0
  for i, x in pairs(indexArray) do  c = c + 1  end

  test = gl.GetPointer('INDEX_ARRAY_POINTER', c)
  CompareArray(test, indexArray)
end
function TestEdgeFlagPointer(flagsArray)
  gl.EnableClientState('EDGE_FLAG_ARRAY')
  gl.EdgeFlagPointer(flagsArray)

  c = 0
  for i, x in pairs(flagsArray) do  c = c + 1  end

  test = gl.GetPointer('EDGE_FLAG_ARRAY_POINTER', c)
  for i=1, c do
     Compare(flagsArray[i], ToBoolean(test[i]))
  end
end
function TestVertexPointer(vArray, size)
  gl.EnableClientState('VERTEX_ARRAY')
  if(size ~= nil) then

    gl.VertexPointer(vArray, size)
    c = 0
    for i, x in pairs(vArray) do  c = c + 1   end
    test = gl.GetPointer('VERTEX_ARRAY_POINTER', c)
    CompareArray(test, vArray)

  else
    gl.VertexPointer(vArray)
    k, c = CreateArray(vArray)
    test = gl.GetPointer('VERTEX_ARRAY_POINTER', c-1)
    CompareArray(test, k)
  end
end
function TestTexCoordPointer(vArray, size)
  gl.EnableClientState('TEXTURE_COORD_ARRAY')
  if(size ~= nil) then
    gl.TexCoordPointer(vArray, size)
    c = 0
    for i, x in pairs(vArray) do  c = c + 1   end
    test = gl.GetPointer('TEXTURE_COORD_ARRAY_POINTER', c)
    CompareArray(test, vArray)
  else
    gl.TexCoordPointer(vArray)
    k, c = CreateArray(vArray)
    test = gl.GetPointer('TEXTURE_COORD_ARRAY_POINTER', c-1)
    CompareArray(test, k)
  end
end
function TestColorPointer(colorArray, size)
  gl.EnableClientState('COLOR_ARRAY')

  if(size ~= nil) then
    gl.ColorPointer(colorArray, size)
    c = 0
    for i, x in pairs(colorArray) do  c = c + 1  end
    test = gl.GetPointer('COLOR_ARRAY_POINTER', c)
    CompareArray(test, colorArray)
  else
    gl.ColorPointer(colorArray)
    k, c = CreateArray(colorArray)
    test = gl.GetPointer('COLOR_ARRAY_POINTER', c-1)
    CompareArray(test, k)
  end
end
function TestAlphaFunc(func, ref)
  gl.AlphaFunc(func,  ref)

  a = gl.GetConst('ALPHA_TEST_FUNC')  Compare(a, func)
  b = gl.Get('ALPHA_TEST_REF')        CompareF(b, ref)
end
function TestBindTexture(target, texture)
  gl.BindTexture(target, texture)

  if(target == 'TEXTURE_1D') then
     test = gl.Get('TEXTURE_BINDING_1D')
  elseif (target == 'TEXTURE_2D') then
     test = gl.Get('TEXTURE_BINDING_2D')
  else 
     f:write("Test Error!")
  end
  Compare(test, texture)
end
function TestBlendFunc(sfactor, dfactor)
  gl.BlendFunc(sfactor, dfactor)

  a = gl.GetConst('BLEND_SRC') Compare(a, sfactor)
  b = gl.GetConst('BLEND_DST') Compare(b, dfactor)
end
function TestClipPlane(plane, equation)
  gl.ClipPlane(plane, equation)

  test = gl.GetClipPlane(plane)
  CompareArrayF(test, equation)
end
function TestColor(color)
  gl.Color(color)
  test = gl.GetArray('CURRENT_COLOR')

  if(color[4] == nil) then test[4] = nil end

  CompareArrayF(test, color)

  if(color[4] == nil) then
     gl.Color(color[1], color[2], color[3])
  else
     gl.Color(color[1], color[2], color[3], color[4])
  end

  test = gl.GetArray('CURRENT_COLOR')

  if(color[4] == nil) then test[4] = nil end

  CompareArrayF(test, color)
end

function ToBoolean(num)
  if num == 0  then return false end
  return true
end

function TestColorMask(red, green, blue, alpha)
  gl.ColorMask(red, green, blue, alpha)
  r, g, b, a = gl.Get('COLOR_WRITEMASK')

  Compare(red,   ToBoolean(r))
  Compare(green, ToBoolean(g))
  Compare(blue,  ToBoolean(b))
  Compare(alpha, ToBoolean(a))
end

function TestColorMaterial(face, mode)
  gl.ColorMaterial(face, mode)

  a = gl.GetConst('COLOR_MATERIAL_FACE')
  b = gl.GetConst('COLOR_MATERIAL_PARAMETER')

  Compare(a, face)
  Compare(b, mode)
end

function CreateArray(Array)
  local k = {}

  local c = 1
  for i, x in pairs(Array) do
    for j, y in pairs(x) do
      k[c] = y
    c = c + 1
    end
  end

  return k, c
end

function TestCullFace(mode)
  gl.CullFace(mode)
  test = gl.GetConst('CULL_FACE_MODE')
  Compare(test, mode)
end
function TestDeleteLists(range)
  list = gl.GenLists(range)

  for i=0,range-1 do
    if(gl.IsList(list + i) == false) then
      f:write('Test Error!\n')
	  return
    end
  end
  gl.DeleteLists(list, range)

  for i=0,range-1 do
    if(gl.IsList(list + i) == true) then
      f:write('Test Error!\n')
	  return
    end
  end
  f:write('Ok.\n')
end
function TestDepthFunc(func)
  gl.DepthFunc(func)
  test = gl.GetConst('DEPTH_FUNC')
  Compare (test, func)
end
function TestDepthMask(flag)
  gl.DepthMask(flag)
  test = ToBoolean(gl.Get('DEPTH_WRITEMASK'))
  Compare(test, flag)
end
function TestDepthRange(znear, zfar)
  gl.DepthRange(znear, zfar)
  near, far = gl.Get('DEPTH_RANGE')
  CompareF(near, znear)
  CompareF(far, zfar)
end

function TestDisable(cap)
  gl.Enable(cap)

  if(gl.IsEnabled(cap) == false) then
     f:write('Test Error!\n')
     return
  end
  gl.Disable(cap)
  if(gl.IsEnabled(cap) == true) then
     f:write('Test Error!\n')
     return
  end
  gl.Enable(cap)
  if(gl.IsEnabled(cap) == false) then
     f:write('Test Error!\n')
     return
  end
  f:write('Ok.\n')
end

function TestDisableClientState(array)
  gl.EnableClientState(array)
  if(gl.IsEnabled(array) == false) then
     f:write('Test Error!\n')
	 return
  end
  gl.DisableClientState(array)
  if(gl.IsEnabled(array) == true) then
     f:write('Test Error!\n')
	 return
  end
  gl.EnableClientState(array)
  if(gl.IsEnabled(array) == false) then
     f:write('Test Error!\n')
	 return
  end
  f:write('Ok.\n')
end

function TestEdgeFlag(flag)
  gl.EdgeFlag(flag)
  test = ToBoolean(gl.Get('EDGE_FLAG'))
  Compare (test, flag)
end
function TestFog(pname, param)
  gl.Fog(pname, param)

  if(pname == 'FOG_MODE') then
    test = gl.GetConst(pname)
  elseif(pname == 'FOG_COLOR') then
    test = gl.GetArray(pname)
  else
    test = gl.Get(pname)
  end
  if(pname == 'FOG_MODE') then
    Compare(test, param)
  elseif(pname == 'FOG_COLOR') then
    CompareArrayF(test, param)
  else
    CompareF(test, param)
  end
end

function TestFrontFace(mode)
  gl.FrontFace(mode)
  test = gl.GetConst('FRONT_FACE')
  Compare(mode, test)
end
function TestLight(light, pname, params)
  gl.Light(light, pname, params)
  test = gl.GetLight(light, pname)
  if(type(params) == 'table') then
    CompareArrayF(test, params)
  else
	CompareF(test[1], params)
  end
end
function TestMaterial(face, pname, param)
  gl.Material(face, pname, param)
  test = gl.GetMaterial(face, pname)
  if(type(param) == 'table') then
    CompareArrayF(test, param)
  else
	CompareF(test[1], param)
  end
end
function TestPixelMap(map, values)
  gl.PixelMap(map, values)
  test = gl.GetPixelMap(map)
  CompareArrayF(test, values)
end
function TestIndex(c)
  gl.Index(c)
  test = gl.Get('CURRENT_INDEX')
  CompareF (test, c)
end
function TestLineWidth(width)
  gl.LineWidth(width)
  test = gl.Get('LINE_WIDTH')
  CompareF(test, width)
end
function TestListBase(base)
  gl.ListBase(base)
  test = gl.Get('LIST_BASE')
  Compare(test, base)
end
function TestLogicOp(opcode)
  gl.LogicOp(opcode)
  test = gl.GetConst('LOGIC_OP_MODE')
  Compare(test, opcode)
end
function TestMatrixMode(mode)
  gl.MatrixMode(mode)
  test = gl.GetConst('MATRIX_MODE')
  Compare(test, mode)
end
function TestNormal(normal)
  gl.Normal(normal)
  test = gl.GetArray('CURRENT_NORMAL')
  CompareArrayF(test, normal)
  gl.Normal(normal[1], normal[2], normal[3])
  test = gl.GetArray('CURRENT_NORMAL')
  CompareArrayF(test, normal)
end

function TestNormalPointer(normalArray)
  gl.EnableClientState('NORMAL_ARRAY')
  if(type(normalArray[1]) == 'number') then
    gl.NormalPointer(normalArray)
    c = 0
    for i, x in pairs(normalArray) do  c = c + 1 end
    test = gl.GetPointer('NORMAL_ARRAY_POINTER', c)
    CompareArrayF(test, normalArray)
  else
    gl.NormalPointer(normalArray)
    k, c = CreateArray(normalArray)
    test = gl.GetPointer('NORMAL_ARRAY_POINTER', c-1)
    CompareArrayF(test, k)
  end
end
function TestPixelStore(pname, param)
  gl.PixelStore(pname, param)
  test = gl.Get(pname)

  if(type(param) == 'boolean') then
    Compare(ToBoolean(test), param)
  else
    Compare(test, param)
  end
end
function TestPixelTransfer(pname, param)
  gl.PixelTransfer(pname, param)
  test = gl.Get(pname)

  if(type(param) == 'boolean') then
    Compare(ToBoolean(test), param)
  else
    CompareF(test, param)
  end
end
function TestPixelZoom(xfactor, yfactor)
  gl.PixelZoom(xfactor, yfactor)
  test = gl.Get('ZOOM_X')
  CompareC(test, xfactor)
  test = gl.Get('ZOOM_Y')
  CompareC(test, yfactor)
end
function TestPointSize(size)
  gl.PointSize(size)
  test = gl.Get('POINT_SIZE')
  Compare(test, size)
end
function TestPolygonMode(face, mode)
  gl.PolygonMode(face, mode)
  testfront, testback = gl.GetConst('POLYGON_MODE')
  if (face == 'FRONT') then
    Compare(testfront, mode)
  elseif (face == 'BACK') then
    Compare(testback, mode)
  elseif (face == 'FRONT_AND_BACK') then
    Compare(testfront, mode)
    Compare(testback, mode)
  end
end
function TestPolygonOffset(factor, units)
  gl.PolygonOffset(factor, units)
  test = gl.Get('POLYGON_OFFSET_FACTOR')
  CompareF(test, factor)
  test = gl.Get('POLYGON_OFFSET_UNITS')
  CompareF(test, units)
end
function TestReadBuffer(mode)
  gl.ReadBuffer(mode)
  test = gl.GetConst('READ_BUFFER')
  Compare(test, mode)
end
function TestRenderMode(mode)
gl.SelectBuffer(100)
gl.FeedbackBuffer(100, '2D')
  gl.RenderMode(mode)
  test = gl.GetConst('RENDER_MODE')
  Compare(test, mode)
end
function TestScissor(x, y, width, height)
  gl.Scissor(x, y, width, height)
  testx, testy, testwidth, testheight = gl.Get('SCISSOR_BOX')
  CompareF(testx, x)
  CompareF(testy, y)
  CompareF(testwidth, width)
  CompareF(testheight, height)
end
function TestShadeModel(mode)
  gl.ShadeModel(mode)
  test = gl.GetConst('SHADE_MODEL')
  Compare(test, mode)
end
function TestStencilOp(fail, zfail, zpass)
  gl.StencilOp(fail, zfail, zpass)
  test = gl.GetConst('STENCIL_FAIL')
  Compare(test, fail)
  test = gl.GetConst('STENCIL_PASS_DEPTH_PASS')
  Compare(test, zpass)
  test = gl.GetConst('STENCIL_PASS_DEPTH_FAIL')
  Compare(test, zfail)
end
function TestViewport(x, y, w, h)
  gl.Viewport(x, y, w, h)
  tx, ty, tw, th = gl.Get('VIEWPORT')
  Compare(tx, x)
  Compare(ty, y)
  Compare(tw, w)
  Compare(th, h)
end
function TestPushAttrib()
  gl.RenderMode('RENDER')
  gl.SelectBuffer(0)
  gl.RenderMode('SELECT')
  gl.InitNames()

  test = gl.Get('NAME_STACK_DEPTH')
  Compare(test, 0)
  gl.PushName(10)
  test = gl.Get('NAME_STACK_DEPTH')
  Compare(test, 1)
  gl.PushName(7)  
  test = gl.Get('NAME_STACK_DEPTH')
  Compare(test, 2)
  gl.PushName(1)
  test = gl.Get('NAME_STACK_DEPTH')
  Compare(test, 3)

  gl.PopName()  
  test = gl.Get('NAME_STACK_DEPTH')
  Compare(test, 2)
  gl.InitNames()  
  test = gl.Get('NAME_STACK_DEPTH')
  Compare(test, 0)

  gl.Color(0,1,0,1)
  test = gl.GetArray('CURRENT_COLOR')

  gl.Enable('FOG')
  test = gl.IsEnabled('FOG')
  Compare(test, true)
  
  gl.PushAttrib('ENABLE_BIT')
  gl.PushAttrib('CURRENT_BIT')
  
    gl.Color(1,0,1,0)
    test = gl.GetArray('CURRENT_COLOR')

    gl.Disable('FOG')
    test = gl.IsEnabled('FOG')
    Compare(test, false)

  gl.PopAttrib()
  gl.PopAttrib()
  
  test = gl.IsEnabled('FOG')
  Compare(test, true)
  test = gl.GetArray('CURRENT_COLOR')
  CompareArray(test, {0,1,0,1})

  gl.EnableClientState('COLOR_ARRAY')
  gl.PixelStore('PACK_SWAP_BYTES', 0.0)
  gl.PushClientAttrib('CLIENT_VERTEX_ARRAY_BIT')
  gl.PushClientAttrib('CLIENT_PIXEL_STORE_BIT')
    gl.DisableClientState('COLOR_ARRAY')
    gl.PixelStore('PACK_SWAP_BYTES', 0.3)
  gl.PopClientAttrib()
  gl.PopClientAttrib()
  
  test = gl.IsEnabled('COLOR_ARRAY')
  Compare(test, true)
  test = gl.Get('PACK_SWAP_BYTES')
  Compare(test, 0.0)

  test = gl.GetError()
  Compare(test, 'NO_ERROR')
  gl.PopAttrib()
  test = gl.GetError()
  Compare(test, 'STACK_UNDERFLOW')
  test = gl.GetError()
  Compare(test, 'NO_ERROR')
end
function TestLightModel(pname, param)
  gl.LightModel(pname, param)

  if(type(param) == 'table') then
    test = gl.GetArray(pname)
    CompareArrayF(test, param)
  else
    test = gl.Get(pname)
    CompareF(test, param)
  end
end

function TestMapGrid(un, u1, u2, vn, v1, v2)
  if(vn == nil) then
    gl.MapGrid(un, u1, u2)
    test1, test2 = gl.Get('MAP1_GRID_DOMAIN')
    Compare(test1, u1)
    Compare(test2, u2)
    test = gl.Get('MAP1_GRID_SEGMENTS')
    Compare(test, un)
  else
    gl.MapGrid(un, u1, u2, vn, v1, v2)
    test = gl.GetArray('MAP2_GRID_DOMAIN')
    CompareF(test[1], u1)
    CompareF(test[2], u2)
    CompareF(test[3], v1)
    CompareF(test[4], v2)
    test1, test2 = gl.Get('MAP2_GRID_SEGMENTS')
    Compare(test1, un)
    Compare(test2, vn)
  end
end

function TestMap(target, points, u1, u2, uorder, v1, v2, vorder)
  if(v1 == nil) then
    gl.Map(target, u1, u2, points)
    test = gl.GetMap(target, 'ORDER')
    Compare(test[1], uorder)
    test = gl.GetMap(target, 'COEFF')
    CompareArrayF(test, points)
  else
    gl.Map(target, u1, u2, v1, v2, points)
    test = gl.GetMap(target, 'ORDER')
    Compare(test[1], uorder)
    Compare(test[2], vorder)

    test = gl.GetMap(target, 'COEFF')
    k = CreateArray(points)
    CompareArrayF(test, k)
  end
end

function TestRasterPos(pos)
  gl.Disable('CULL_FACE')

  gl.MatrixMode('MODELVIEW')
  gl.LoadIdentity()
  gl.MatrixMode('PROJECTION')
  gl.LoadIdentity()

  gl.RasterPos(pos)
  test = gl.GetArray('CURRENT_RASTER_POSITION')

  if pos[1] == nil then test[1] = nil end
  if pos[2] == nil then test[2] = nil end
  if pos[3] == nil then test[3] = nil end
  if pos[4] == nil then test[4] = nil end

  if(test[1] ~= nil) then  test[1] = (test[1] - 320) / 320 end
  if(test[2] ~= nil) then  test[2] = (test[2] - 240) / 240 end
  if(test[3] ~= nil) then  test[3] = (test[3] * 2) - 1 end
  if(test[4] ~= nil) then
     test[1] = test[1] * test[4]
     test[2] = test[2] * test[4]
     test[3] = test[3] * test[4]
  end
  
  CompareArrayF(test, pos)
end
function TestClearAccum(r, g, b, a)
  gl.ClearAccum(r, g, b, a)
  tr, tg, tb, ta = gl.Get('ACCUM_CLEAR_VALUE')
  CompareF(tr, r) CompareF(tg, g)  CompareF(tb, b)  CompareF(ta, a)
end
function TestClearColor(r, g, b, a)
  gl.ClearColor(r, g, b, a)
  tr, tg, tb, ta = gl.Get('COLOR_CLEAR_VALUE')
function TestDrawBuffer(mode)
  gl.DrawBuffer(mode)
  test = gl.GetConst('DRAW_BUFFER')
  Compare(test, mode)
end  CompareF(tr, r) CompareF(tg, g)  CompareF(tb, b)  CompareF(ta, a)
end
function TestClearDepth(d)
  gl.ClearDepth(d)
  test = gl.Get('DEPTH_CLEAR_VALUE')
  CompareF(test, d)
end
function TestClearIndex(c)
  gl.ClearIndex(c)
  test = gl.Get('INDEX_CLEAR_VALUE')
  CompareF(test, c)
end
function TestClearStencil(s)
  bits = gl.Get('STENCIL_BITS')
  if(s <= 2^bits - 1) then
    gl.ClearStencil(s)
    test = gl.Get('STENCIL_CLEAR_VALUE')
    CompareF(test, s)
  end
end
function TestHint(target, mode)
  gl.Hint(target, mode)
  teste = gl.GetConst(target)
  Compare(teste, mode)
end
function TestNewList(list, mode)
  gl.NewList(list, mode)
  test = gl.Get('LIST_INDEX')
  Compare(test, list)
  test = gl.GetConst('LIST_MODE')
  Compare(test, mode)
  gl.EndList()
  test = gl.Get('LIST_INDEX')
  Compare(test, 0)
end
function TestDrawBuffer(mode)
  gl.DrawBuffer(mode)
  test = gl.GetConst('DRAW_BUFFER')
  Compare(test, mode)
end
function TestIndexMask(mask)
  bits = gl.Get('INDEX_BITS')
  gl.IndexMask(mask)
  if((mask+0) < (2^bits - 1)*10) then
    test = gl.Get('INDEX_WRITEMASK')
    Compare(test+0, mask+0)
  end
end
function TestLineStipple(factor, pattern)
  gl.LineStipple(factor, pattern)
  test = gl.Get('LINE_STIPPLE_PATTERN')
  Compare(test, pattern)
end
function TestStencilFunc(func, ref, mask, mask2)
  gl.StencilFunc(func, ref, mask)
  test = gl.GetConst('STENCIL_FUNC')
  Compare(test, func)

  bits = gl.Get('STENCIL_BITS')
  if(ref <= 2^bits - 1) then
    test = gl.Get('STENCIL_REF')
    Compare(test, ref)
  end
  if((mask+0) < 10^(2^bits - 1)) then
    test = gl.Get('STENCIL_VALUE_MASK')
    Compare(test+0, mask+0)
  end
  gl.StencilMask(mask2)
  if((mask2+0) < 10^(2^bits - 1)) then
    test = gl.Get('STENCIL_WRITEMASK')
    Compare(test+0, mask2+0)
  end
end

function TestAll()

  f = io.open('tests.txt', 'w+')
	
  -----------------------------------------------
  -- Tests: gl.DeleteTexture, gl.GenTextures, gl.IsTexture,
  --        gl.GetTexImage, gl.GetTexLevelParameter, gl.TexImage,
  --        gl.TexSubImage, and gl.PrioritizeTextures

  f:write ('\nTesting functions: gl.DeleteTexture, gl.GenTextures,' ..
            '\n                   gl.IsTexture, gl.TexImage, gl.GetTexImage,' ..
	    '\n                   gl.TexSubImage, gl.PrioritizeTextures,' ..
	    '\n                   and GetTexLevelParameter \n')

  Image1D = { 0.0, 0.1, 0.2,
              0.3, 0.4, 0.5,
              0.6, 0.7, 0.8,
              0.9, 1.0, 0.0 }

  Image2D = { { 0,0,0, 1,1,1, 1,1,1, 0,0,0 },
              { 0,0,0, 1,1,1, 1,1,1, 0,0,0 },
              { 1,1,1, 1,1,1, 1,1,1, 0,0,0 },
              { 1,1,1, 1,1,1, 1,1,1, 0,0,0 } }

  TestTexture(Image1D,  1, 'TEXTURE_1D', 3, 'RGB', 0, {0.0})
  TestTexture(Image2D,  5, 'TEXTURE_2D', 3, 'RGB', 0, {0.1, 0.2, 0.3, 0.4, 0.5})
  TestTexture(Image1D,  7, 'TEXTURE_1D', 3, 'RGB', 0, {0.6, 0.7, 0.8, 0.9, 1.0,  0.33, 0.67})
  TestTexture(Image2D, 10, 'TEXTURE_2D', 3, 'RGB', 0, {1.0, 1.0, 0.0, 0.0, 0.5,  1.0, 1.0, 0.0, 0.0, 0.5})

  -----------------------------------------------
  -- Test gl.RasterPos

  f:write ('\nTesting function: gl.RasterPos \n')

  TestRasterPos({0.0, 1.0})
  TestRasterPos({0.2, 0.3, 0.0})
  TestRasterPos({0.2, 0.3, -1.0})
  TestRasterPos({0.2, 0.3, -0.5})
  TestRasterPos({0,0,0,1.0})if(test[2] ~= nil) then  test[2] = (test[2] - 240) / 240 end
  TestRasterPos({-0.4, 0.2, 0.5, 0.75})

  -----------------------------------------------
  -- Test gl.AlphaFunc

  f:write ('\nTesting function: gl.AlphaFunc \n')

  TestAlphaFunc('NEVER', 0.0)
  TestAlphaFunc('LESS',  0.3)
  TestAlphaFunc('EQUAL', 1.0)

  -----------------------------------------------
  -- Test gl.BindTexture

  f:write ('\nTesting function: gl.BindTexture \n')

  textures = gl.GenTextures(6)

  TestBindTexture('TEXTURE_1D', textures[1])
  TestBindTexture('TEXTURE_1D', textures[2])
  TestBindTexture('TEXTURE_2D', textures[3])
  TestBindTexture('TEXTURE_2D', textures[4])
  TestBindTexture('TEXTURE_2D', textures[5])
  TestBindTexture('TEXTURE_1D', textures[6])

  textures = gl.DeleteTextures(textures)

  -----------------------------------------------
  -- Test gl.BlendFunc

  f:write ('\nTesting function: gl.BlendFunc \n')

  TestBlendFunc('SRC_ALPHA', 'ONE_MINUS_SRC_ALPHA')
  TestBlendFunc('ONE_MINUS_DST_COLOR', 'ONE_MINUS_DST_ALPHA')
  TestBlendFunc('SRC_ALPHA_SATURATE', 'DST_ALPHA')

  -----------------------------------------------
  -- Test gl.ClipPlane, gl.GetClipPlane

  f:write ('\nTesting functions: gl.ClipPlane and gl.GetClipPlane \n')

  TestClipPlane('CLIP_PLANE0', {2.3, 3.1, 4.0, 5.7})
  TestClipPlane('CLIP_PLANE1', {6, 7, 8, 9})
  TestClipPlane('CLIP_PLANE2', {2, 3, 5, 7})
  TestClipPlane('CLIP_PLANE3', {43002, 999999, 9657945, 0})
  TestClipPlane('CLIP_PLANE4', {58, 346, 23675, -757})
  TestClipPlane('CLIP_PLANE5', {0, -3, -5, -7})

  -----------------------------------------------
  -- Test gl.Color

  f:write ('\nTesting function: gl.Color \n')

  TestColor({0.0, 0.0, 0.0, 0.0})
  TestColor({0.1, 0.2, 0.3, 0.4})
  TestColor({0.8, 0.7, 0.6})
  TestColor({0.1, 0.2, 0.3})

  -----------------------------------------------
  -- Test gl.ColorMask

  f:write ('\nTesting function: gl.ColorMask \n')

  TestColorMask( false, false, false, false)
  TestColorMask( true , false, true , false)
  TestColorMask( false, true , false, true )
  TestColorMask( true , true , true , true )
  TestColorMask( false, false, false, false)

  -----------------------------------------------
  -- Test gl.ColorMaterial

  f:write ('\nTesting function: gl.ColorMaterial \n')

  TestColorMaterial('FRONT_AND_BACK', 'DIFFUSE')
  TestColorMaterial('BACK' , 'AMBIENT_AND_DIFFUSE')
  TestColorMaterial('FRONT', 'EMISSION')
  TestColorMaterial('BACK' , 'AMBIENT')
  TestColorMaterial('FRONT', 'SPECULAR')

  -----------------------------------------------
  -- Test gl.CullFace

  f:write ('\nTesting function: gl.CullFace \n')

  TestCullFace('FRONT')
  TestCullFace('BACK')
  TestCullFace('FRONT_AND_BACK')

  -----------------------------------------------
  -- Test gl.DeleteLists, gl.GenLists, gl.IsList

  f:write ('\nTesting functions: gl.DeleteLists, gl.GenLists and gl.IsList \n')

  TestDeleteLists(1)
  TestDeleteLists(2)
  TestDeleteLists(999)
  TestDeleteLists(5)
  TestDeleteLists(10)

  -----------------------------------------------
  -- Test gl.DepthFunc

  f:write ('\nTesting function: gl.DepthFunc \n')

  TestDepthFunc('NEVER')
  TestDepthFunc('LESS')
  TestDepthFunc('LEQUAL')
  TestDepthFunc('EQUAL')
  TestDepthFunc('GREATER')
  TestDepthFunc('NOTEQUAL')
  TestDepthFunc('GEQUAL')
  TestDepthFunc('ALWAYS')

  -----------------------------------------------
  -- Test gl.DepthMask

  f:write ('\nTesting function: gl.DepthMask \n')

  TestDepthMask(true)
  TestDepthMask(false)
  TestDepthMask(true)

  -----------------------------------------------
  -- Test gl.DepthRange

  f:write ('\nTesting function: gl.DepthRange \n')

  TestDepthRange(0.0 ,0.0)
  TestDepthRange(1.0 ,1.0)
  TestDepthRange(0.32,0.34)
  TestDepthRange(0.17,0.87)
  TestDepthRange(0.5 ,0.5)

  -----------------------------------------------
  -- Test gl.Disable, gl.Enable, gl.IsEnabled

  f:write ('\nTesting functions: gl.Disable, gl.Enable and gl.IsEnabled\n')

  TestDisable('ALPHA_TEST')
  TestDisable('BLEND')
  TestDisable('COLOR_ARRAY')
  TestDisable('LIGHTING')
  TestDisable('POINT_SMOOTH')
  TestDisable('VERTEX_ARRAY')
  TestDisable('STENCIL_TEST')
  TestDisable('MAP2_INDEX')
  TestDisable('DITHER')

  -----------------------------------------------
  -- Test gl.DisableClientState, gl.EnableClientState

  f:write ('\nTesting functions: gl.DisableClientState and gl.EnableClientState\n')

  TestDisableClientState('COLOR_ARRAY')
  TestDisableClientState('EDGE_FLAG_ARRAY')
  TestDisableClientState('INDEX_ARRAY')
  TestDisableClientState('NORMAL_ARRAY')
  TestDisableClientState('TEXTURE_COORD_ARRAY')
  TestDisableClientState('VERTEX_ARRAY')

  -----------------------------------------------
  -- Test gl.EdgeFlag()

  f:write ('\nTesting function: gl.EdgeFlag \n')

  TestEdgeFlag(true)
  TestEdgeFlag(false)
  TestEdgeFlag(true)

  -----------------------------------------------
  -- Test gl.Fog()

  f:write ('\nTesting function: gl.Fog \n')

  TestFog('FOG_MODE', 'LINEAR')
  TestFog('FOG_MODE', 'EXP')
  TestFog('FOG_MODE', 'EXP2')
  TestFog('FOG_DENSITY', 0.13)
  TestFog('FOG_DENSITY', 0.2)
  TestFog('FOG_START', 0.76)
  TestFog('FOG_END', 0.34)
  TestFog('FOG_COLOR', { 0.8, 0.3, 0.2, 1.0})
  TestFog('FOG_COLOR', { 0.5, 0.9, 1.0, 0.0})

  -----------------------------------------------
  -- Test gl.FrontFace()

  f:write ('\nTesting function: gl.FrontFace \n')

  TestFrontFace('CW')
  TestFrontFace('CCW')
  TestFrontFace('CW')

  -----------------------------------------------
  -- Test gl.Light, gl.GetLight

  f:write ('\nTesting functions: gl.Light and gl.GetLight \n')

  TestLight('LIGHT3', 'AMBIENT', { 0.1, 0.2, 0.3, 0.4 })
  TestLight('LIGHT1', 'SPOT_DIRECTION', { 0.5, 0.6, 0.7 })
  TestLight('LIGHT6', 'LINEAR_ATTENUATION', 0.8)

  -----------------------------------------------
  -- Test gl.Material, gl.GetMaterial

  f:write ('\nTesting functions: gl.Material and gl.GetMaterial \n')

  TestMaterial('FRONT', 'AMBIENT', { 0.1, 0.2, 0.3, 0.4 })
  TestMaterial('BACK', 'DIFFUSE', { 0.5, 0.6, 0.7, 0.8 })
  TestMaterial('FRONT', 'SHININESS', 0.9 )
  TestMaterial('BACK', 'COLOR_INDEXES', { 0.0, 0.5, 1.0 })

  -----------------------------------------------
  -- Test gl.PixelMap, gl.GetPixelMap

  f:write ('\nTesting functions: gl.PixelMap and gl.GetPixelMap \n')

  TestPixelMap('PIXEL_MAP_R_TO_R', { 0.1, 0.2, 0.3, 0.4 } )
  TestPixelMap('PIXEL_MAP_I_TO_R', { 0.5, 0.6, 0.7, 0.8 } )
  TestPixelMap('PIXEL_MAP_I_TO_B', { 0.9, 1.0 } )
  TestPixelMap('PIXEL_MAP_A_TO_A', { 0.5 } )

  -----------------------------------------------
  -- Test gl.Index

  f:write ('\nTesting function: gl.Index \n')

  TestIndex(0)
  TestIndex(0.2)
  TestIndex(1.0)
  TestIndex(0)

  -----------------------------------------------
  -- Test gl.LineWidth

  f:write ('\nTesting function: gl.LineWidth \n')

  TestLineWidth(1)
  TestLineWidth(1.5)
  TestLineWidth(10.3)
  TestLineWidth(0.01)
  TestLineWidth(1)

  -----------------------------------------------
  -- Test gl.ListBase

  f:write ('\nTesting function: gl.ListBase \n')

  TestListBase(1)
  TestListBase(20)
  TestListBase(0)
  TestListBase(55)

  -----------------------------------------------
  -- Test gl.LogicOp

  f:write ('\nTesting function: gl.LogicOp \n')

  TestLogicOp('CLEAR')
  TestLogicOp('SET')
  TestLogicOp('COPY')
  TestLogicOp('NOOP')
  TestLogicOp('INVERT')
  TestLogicOp('AND')
  TestLogicOp('XOR')

  -----------------------------------------------
  -- Test gl.MatrixMode

  f:write ('\nTesting function: gl.MatrixMode \n')

  TestMatrixMode('MODELVIEW')
  TestMatrixMode('PROJECTION')
  TestMatrixMode('TEXTURE')
  TestMatrixMode('MODELVIEW')

  -----------------------------------------------
  -- Test gl.Normal

  f:write ('\nTesting function: gl.Normal \n')

  TestNormal({0.0, 0.0, 0.0})
  TestNormal({0.1, 0.2, 0.3})
  TestNormal({1.0, 1.0, 1.0})
  TestNormal({0.8, 0.7, 0.6})
  TestNormal({0.1, 0.2, 0.3})

  -----------------------------------------------
  -- Test gl.NormalPointer

  f:write ('\nTesting function: gl.NormalPointer \n')
 
  TestNormalPointer( {0.7, 0.9, 0.8,
                      0.0, 0.11,0.34})
  TestNormalPointer( {0.1,1.0,0.3,
                      0.4,0.5,0.6,
                      0.7,0.8,0.9})
  TestNormalPointer( { {1.0, 1.0, 0.0},
                       {0.1, 0.2, 0.3},
                       {0.0, 0.0, 1.0},
		       {0.54,0.78,0.5} } )

  -----------------------------------------------
  -- Test gl.PixelStore

  f:write ('\nTesting function: gl.PixelStore \n')

  TestPixelStore('UNPACK_SWAP_BYTES', true)
  TestPixelStore('UNPACK_SWAP_BYTES', false)
  TestPixelStore('UNPACK_SWAP_BYTES', true)
  TestPixelStore('UNPACK_SWAP_BYTES', false)
  TestPixelStore('PACK_LSB_FIRST', true)
  TestPixelStore('PACK_LSB_FIRST', false)
  TestPixelStore('PACK_LSB_FIRST', true)
  TestPixelStore('PACK_ROW_LENGTH', 8)
  TestPixelStore('PACK_ROW_LENGTH', 16)
  TestPixelStore('PACK_ROW_LENGTH', 0)
  TestPixelStore('UNPACK_ALIGNMENT', 8)
  TestPixelStore('UNPACK_ALIGNMENT', 1)
  TestPixelStore('UNPACK_ALIGNMENT', 2)
  TestPixelStore('UNPACK_ALIGNMENT', 4)

  -----------------------------------------------
  -- Test gl.PixelTransfer

  f:write ('\nTesting function: gl.PixelTransfer \n')

  TestPixelTransfer('MAP_COLOR', true)
  TestPixelTransfer('MAP_COLOR', false)
  TestPixelTransfer('MAP_COLOR', true)
  TestPixelTransfer('INDEX_SHIFT', 0)
  TestPixelTransfer('INDEX_SHIFT', 2)
  TestPixelTransfer('INDEX_SHIFT', 8)
  TestPixelTransfer('RED_SCALE', 0)
  TestPixelTransfer('RED_SCALE', 1.0)
  TestPixelTransfer('RED_SCALE', 0.3)

  -----------------------------------------------
  -- Test gl.PixelZoom

  f:write ('\nTesting function: gl.PixelZoom \n')

  TestPixelZoom(0.5,0.7)
  TestPixelZoom(1.0,1.0)
  TestScissor(1.0,0.0,0.0,1.0)
  TestPixelZoom(1.7,4.3)
  TestPixelZoom(2.0,3.0)

  -----------------------------------------------
  -- Test gl.PointSize

  f:write ('\nTesting function: gl.PointSize \n')

  TestPointSize(1)
  TestPointSize(7)
  TestPointSize(3)
  TestPointSize(2)
  TestPointSize(1)

  -----------------------------------------------
  -- Test gl.PolygonMode

  f:write ('\nTesting function: gl.PolygonMode \n')

  TestPolygonMode('FRONT', 'POINT')
  TestPolygonMode('FRONT', 'LINE')
  TestPolygonMode('FRONT', 'FILL')
  TestPolygonMode('BACK', 'POINT')
  TestPolygonMode('BACK', 'LINE')
  TestPolygonMode('BACK', 'FILL')
  TestPolygonMode('FRONT_AND_BACK', 'POINT')
  TestPolygonMode('FRONT_AND_BACK', 'LINE')
  TestPolygonMode('FRONT_AND_BACK', 'FILL')

  -----------------------------------------------
  -- Test gl.PolygonOffset 

  f:write ('\nTesting function: gl.PolygonOffset \n')

  TestPolygonOffset(0, 0)
  TestPolygonOffset(-1, -2)
  TestPolygonOffset(3, 5)
  TestPolygonOffset(1.3, 500)

  -----------------------------------------------
  -- Test gl.LoadMatrix, gl.GetMatrix, gl.LoadIdentity, gl.Translate, gl.Scale, gl.Rotate,
  --      gl.Frustum, gl.Ortho, gl.MultMatrix, gl.PopMatrix, gl.PushMatrix

  f:write ('\nTesting functions: gl.LoadMatrix, gl.GetMatrix, gl.LoadIdentity \n')

  TestMatrix({ 0,0.1,0.2,0.3,  0.4,0.5,0.6,0.7,  0.8,0.9,1.0,0.1,  0.11,0.12,0.13,0.14},
               1,0.5,1.3,  1.4,0.7,0.3,  50, 1, 0, 0,
               -0.5, 0.5, -0.5, 0.5, 1, 100,
	       -0.5, 0.5, -0.5, 0.5, 1, 100)

  -----------------------------------------------
  -- Test gl.ReadBuffer

  f:write ('\nTesting function: gl.ReadBuffer\n')

  TestReadBuffer('FRONT_LEFT')
  TestReadBuffer('BACK_LEFT')
  TestReadBuffer('FRONT')
  TestReadBuffer('BACK')
  TestReadBuffer('LEFT')
  
  -----------------------------------------------
  -- Test gl.RenderMode

  f:write ('\nTesting function: gl.RenderMode\n')

  TestRenderMode('RENDER')
  TestRenderMode('SELECT')

  -----------------------------------------------
  -- Test gl.Scissor

  f:write ('\nTesting function: gl.Scissor\n')

  TestScissor(0,0,300,300)
  TestScissor(10,20,100,200)
  TestScissor(200,100,30,80)

  -----------------------------------------------
  -- Test gl.ShadeModel

  f:write ('\nTesting function: gl.ShadeModel\n')

  TestShadeModel('FLAT')
  TestShadeModel('SMOOTH')

  -----------------------------------------------
  -- Test gl.StencilOp

  f:write ('\nTesting function: gl.StencilOp\n')

  TestStencilOp('KEEP', 'DECR', 'INCR')
  TestStencilOp('INCR', 'REPLACE', 'INVERT')

  -----------------------------------------------
  -- Test gl.GetError, gl.PushAttrib, gl.PopAttrib, gl.PushClientAttrib, gl.PopClientAttrib,
  --      gl.PushName, gl.PopName, gl.InitNames

  f:write ('\nTesting functions: gl.GetError, gl.PushAttrib, gl.PopAttrib,' ..
            '\n                   gl.PushName, gl.PopName, gl.InitNames,' ..
	    '\n                   gl.PushClientAttrib and gl.PopClientAttrib\n')

  TestPushAttrib()

  -----------------------------------------------
  -- Test gl.Map, gl.GetMap

  f:write ('\nTesting functions: gl.Map, gl.GetMap\n')
  
  TestMap('MAP1_VERTEX_3', {0.1,0.2,0.3, 0.4,0.5,0.6, 0.7,0.8,0.9, 1.0,0.9,0.8}, 0.1, 0.2, 4)
  TestMap('MAP2_TEXTURE_COORD_3', {{ 0.1,0.2,0.3, 0.4,0.5,0.6, 0.7,0.8,0.9, 1.0,0.9,0.8}}, 0.2,0.3,4, 0.5,0.6, 1)
  TestMap('MAP2_TEXTURE_COORD_2', {{ 0.0,0.0 }, { 1.0,1.0 }}, 0.0, 1.0, 1, 0.0,1.0, 2)
  TestMap('MAP2_TEXTURE_COORD_2', {{ 0.7,0.3}, {0.8,0.0 }, { 0.1,0.3},  {0.4,0.5 }}, 0.0, 1.0, 1, 0.0,1.0, 4)

  -----------------------------------------------
  -- Test gl.MapGrid

  f:write ('\nTesting function: gl.MapGrid\n')

  TestMapGrid(4, 0.0,1.0)
  TestMapGrid(5, 0.1,0.2, 9, 0.3,0.4)
  TestMapGrid(30, 0.5,0.6, 10, 0.7,0.8)

  -----------------------------------------------
  -- Test gl.Viewport

  f:write ('\nTesting function: gl.Viewport\n')

  TestViewport(10, 20, 200, 300)
  TestViewport(100, 200, 12, 75)
  TestViewport(0, 0, 640, 480)

  -----------------------------------------------
  -- Test gl.ColorPointer

  f:write ('\nTesting function: gl.ColorPointer \n')

  TestColorPointer( {0.7, 0.9, 0.8, 0.4,
                     0.0, 0.11,0.34,0.85} , 4)
  TestColorPointer( {0.1,1.0,0.3,
                     0.4,0.5,0.6,
		     0.7,0.8,0.9} , 3)
  TestColorPointer( { {1.0, 1.0, 0.0},
                      {0.1, 0.2, 0.3},
                      {0.0, 0.0, 1.0},
		      {0.54,0.78,0.5} } )
  TestColorPointer( { {0.8, 0.2, 0.3, 0.4},
                      {0.1, 0.2, 0.3, 0.4},
		      {0.0, 1.0, 0.7, 0.5} } )

  -----------------------------------------------
  -- Test gl.TexCoordPointer

  f:write ('\nTesting function: gl.TexCoordPointer \n')


  TestTexCoordPointer( {0.7, 0.9, 0.8, 0.4,
                        0.0, 0.11,0.34,0.85} , 4)
  TestTexCoordPointer( {0.1,1.0,0.3,
                        0.4,0.5,0.6,
   		        0.7,0.8,0.9} , 3)
  TestTexCoordPointer( {1.0,1.0,
                        0.0,0.0,
		        0.3,0.7} , 2)
  TestTexCoordPointer( { {1.0, 1.0, 0.0},
                         {0.1, 0.2, 0.3},
                         {0.0, 0.0, 1.0},
		         {0.54,0.78,0.5} } )
  TestTexCoordPointer( { {0.8, 0.2, 0.3, 0.4},
                         {0.1, 0.2, 0.3, 0.4},
		         {0.0, 1.0, 0.7, 0.5} } )

  -----------------------------------------------
  -- Test gl.VertexPointer

  f:write ('\nTesting function: gl.VertexPointer \n')

  TestVertexPointer( {0.7, 0.9, 0.8, 0.4,
                      0.0, 0.11,0.34,0.85} , 4)
  TestVertexPointer( {0.7, 0.9, 0.8, 0.4,
                      0.0, 0.11,0.34,0.85} , 2)
  TestVertexPointer( {0.1,1.0,0.3,
                      0.4,0.5,0.6,
   		      0.7,0.8,0.9} , 3)
  TestVertexPointer( { {1.0, 1.0, 0.0},
                       {0.1, 0.2, 0.3},
                       {0.0, 0.0, 1.0},
		       {0.54,0.78,0.5} } )
  TestVertexPointer( { {0.8, 0.2, 0.3, 0.4},
                       {0.1, 0.2, 0.3, 0.4},
	 	       {0.0, 1.0, 0.7, 0.5} } )

  -----------------------------------------------
  -- Test gl.IndexPointer

  f:write ('\nTesting function: gl.IndexPointer \n')

  TestIndexPointer( { 0.7, 0.9 })
  TestIndexPointer( { 0.1,1.0,0.3, 0.4,0.5,0.6, 0.7,0.8,0.9 })
  TestIndexPointer( { 1.0, 1.0, 0.0, 0.1, 0.2, 0.3, 0.0, 0.0, 1.0 })

  -----------------------------------------------
  -- Test gl.EdgeFlagPointer

  f:write ('\nTesting function: gl.EdgeFlagPointer \n')

  TestEdgeFlagPointer( {true, false, false, true, true, false, false, false, false})
  TestEdgeFlagPointer( {false, true})
  TestEdgeFlagPointer( {true})

  -----------------------------------------------
  -- Test gl.LightModel

  f:write ('\nTesting function: gl.LightModel \n')

  TestLightModel('LIGHT_MODEL_AMBIENT', { 0.1, 0.2, 0.3, 0.4 })
  TestLightModel('LIGHT_MODEL_AMBIENT', { 0.5, 0.6, 0.7, 0.8 })
  TestLightModel('LIGHT_MODEL_LOCAL_VIEWER', 1)
  TestLightModel('LIGHT_MODEL_LOCAL_VIEWER', 0)
  TestLightModel('LIGHT_MODEL_LOCAL_VIEWER', { 1 })
  TestLightModel('LIGHT_MODEL_TWO_SIDE', { 0 })
  TestLightModel('LIGHT_MODEL_TWO_SIDE',  1)
  TestLightModel('LIGHT_MODEL_TWO_SIDE',  0)

  -----------------------------------------------
  -- Test gl.TexEnv

  f:write ('\nTesting function: gl.TexEnv \n')

  TestTexEnv('TEXTURE_ENV_MODE', 'MODULATE')
  TestTexEnv('TEXTURE_ENV_MODE', 'DECAL')
  TestTexEnv('TEXTURE_ENV_MODE', 'BLEND')
  TestTexEnv('TEXTURE_ENV_MODE', 'REPLACE')

  TestTexEnv('TEXTURE_ENV_COLOR', { 0.1, 0.2, 0.3, 0.4})
  TestTexEnv('TEXTURE_ENV_COLOR', { 0, 0, 0, 0})

  -----------------------------------------------
  -- Test gl.TexGen

  f:write ('\nTesting function: gl.TexGen \n')

  TestTexGen('R', 'TEXTURE_GEN_MODE', 'OBJECT_LINEAR')
  TestTexGen('T', 'TEXTURE_GEN_MODE', 'EYE_LINEAR')
  TestTexGen('S', 'TEXTURE_GEN_MODE', 'SPHERE_MAP')
  TestTexGen('Q', 'OBJECT_PLANE', { 1.0 , 0.0 , 0.7, 0.3 } )
  TestTexGen('S', 'OBJECT_PLANE', { 0.1 , 0.2 , 0.3, 0.4 } )

  -----------------------------------------------
  -- Test gl.TexParameter

  f:write ('\nTesting function: gl.TexParameter \n')

  TestTexParameter('TEXTURE_1D', 'TEXTURE_MIN_FILTER', 'NEAREST_MIPMAP_NEAREST')
  TestTexParameter('TEXTURE_2D', 'TEXTURE_MIN_FILTER', 'LINEAR_MIPMAP_NEAREST')
  TestTexParameter('TEXTURE_2D', 'TEXTURE_MIN_FILTER', 'NEAREST_MIPMAP_LINEAR')
  TestTexParameter('TEXTURE_1D', 'TEXTURE_MAG_FILTER', 'NEAREST')
  TestTexParameter('TEXTURE_2D', 'TEXTURE_MAG_FILTER', 'LINEAR')
  TestTexParameter('TEXTURE_1D', 'TEXTURE_WRAP_S', 'CLAMP')
  TestTexParameter('TEXTURE_2D', 'TEXTURE_WRAP_T', 'REPEAT')

  TestTexParameter('TEXTURE_2D', 'TEXTURE_BORDER_COLOR', { 1.0, 0.0, 0.5, 1.0 } )
  TestTexParameter('TEXTURE_1D', 'TEXTURE_BORDER_COLOR', { 0.3, 0.5, 0.7, 0.7 } )
  TestTexParameter('TEXTURE_2D', 'TEXTURE_PRIORITY', 0.0)
  TestTexParameter('TEXTURE_1D', 'TEXTURE_PRIORITY', 1.0)
  TestTexParameter('TEXTURE_2D', 'TEXTURE_PRIORITY', 0.8)

  -----------------------------------------------
  -- Test gl.TestClearAccum, gl.TestClearColor, gl.TestClearDepth,
  --      gl.TestClearIndex, gl.TestClearStencil

  f:write ('\nTesting functions: gl.TestClearAccum, gl.TestClearColor,'..
            '\n                   gl.TestClearDepth, gl.TestClearIndex,'..
            '\n                   gl.TestClearStencil\n')

  TestClearAccum(0,0,0,0)
  TestClearAccum(0.1,0.2,0.3,0.4)
  TestClearAccum(0.5,0.6,0.7,0.9)
  TestClearAccum(1,1,1,1)

  TestClearColor(0,0,0,0)
  TestClearColor(0.1,0.2,0.3,0.4)
  TestClearColor(0.5,0.6,0.7,0.9)
  TestClearColor(1,1,1,1)

  TestClearDepth(0)
  TestClearDepth(0.1)
  TestClearDepth(0.5)
  TestClearDepth(1,1,1,1)

  TestClearIndex(0)
  TestClearIndex(0.125)
  TestClearIndex(0.5)
  TestClearIndex(1)

  TestClearStencil(0)
  TestClearStencil(1)
  TestClearStencil(2)

  -----------------------------------------------
  -- Test gl.Hint

  f:write ('\nTesting function: gl.Hint \n')

  TestHint('FOG_HINT', 'FASTEST')
  TestHint('LINE_SMOOTH_HINT', 'NICEST')
  TestHint('PERSPECTIVE_CORRECTION_HINT', 'DONT_CARE')
  TestHint('POINT_SMOOTH_HINT', 'NICEST')
  TestHint('POLYGON_SMOOTH_HINT', 'FASTEST')

  -----------------------------------------------
  -- Test gl.NewList, gl.EndList

  f:write ('\nTesting function: gl.NewList and gl.EndList \n')

  TestNewList(1, 'COMPILE')
  TestNewList(2, 'COMPILE_AND_EXECUTE')
  TestNewList(7, 'COMPILE')

  -----------------------------------------------
  -- Test gl.DrawBufferUNPACK_LSB_FIRST

  f:write ('\nTesting function: gl.DrawBuffer \n')

  TestDrawBuffer('FRONT_LEFT')
  TestDrawBuffer('FRONT')
  TestDrawBuffer('BACK')
  TestDrawBuffer('LEFT')
  TestDrawBuffer('FRONT_AND_BACK')

  -----------------------------------------------
  -- Test gl.LineStipple

  f:write ('\nTesting function: gl.LineStipple \n')

  TestLineStipple(1, '0000111100001111')
  TestLineStipple(2, '1111000011110000')
  TestLineStipple(3, '0101010101010101')
  TestLineStipple(4, '1101110110000101')

  -----------------------------------------------
  -- Test gl.StencilFunc, gl.StencilMask

  f:write ('\nTesting function: gl.StencilFunc and gl.StencilMask\n')

  TestStencilFunc('NEVER', 1, '000', '0')
  TestStencilFunc('LESS', 2, '001', '1')
  TestStencilFunc('LEQUAL', 4, '010', '01')
  TestStencilFunc('GREATER', 8, '1101', '10')
  TestStencilFunc('GEQUAL', 4, '100', '1001')
  TestStencilFunc('ALWAYS', 2, '011', '0101')

  -----------------------------------------------
  -- Test gl.IndexMask

  f:write ('\nTesting function: gl.IndexMask \n')

  TestIndexMask('0')
  TestIndexMask('1')
  TestIndexMask('01')
  TestIndexMask('10')
  TestIndexMask('1001')

  f:write('\n\ngl.GetError() -> ' .. gl.GetError())

  f:close()

end

glut.Init()
glut.InitDisplayMode()
glut.InitWindowSize(640, 480)
glut.CreateWindow("Lua GL Test Application")

TestAll()

