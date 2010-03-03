/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#include "rubyk.h"
#define INIT_SCRIPT "view = {width=0, height=0}\n\
function draw()\n\
end\n\
function resize(width, height)\n\
  view = {width=width, height=height}\n\
end"

class GLLua : public LuaScript {
public:
  const Value init() {
    // insert dummy 'draw' and 'resize' methods
    view_width_  = 0;
    view_height_ = 0;
    return lua_init(INIT_SCRIPT);
  }

  // inlet 1
  void draw(const Value &val) {
    if (!is_opengl_thread()) {
      fprintf(stderr, "%s: cannot call draw (not an OpenGL thread)\n", name_.c_str());
      return;
    }

    if (val[0].r != view_width_ || val[1].r != view_height_) {
      view_width_  = val[0].r;
      view_height_ = val[1].r;
      call_lua("resize", val);
    }

    call_lua("draw", gNilValue);
  }
protected:
  /* open all standard libraries and openGL libraries (called by LuaScript on init) */
  virtual void open_lua_libs() {
    LuaScript::open_lua_libs();
    open_lua_lib("opengl", luaopen_opengl);
    open_lua_lib("glut", luaopen_glut);
    open_lua_lib("rubyk", luaopen_rubyk);
    // TODO: adapt GLWindow with keyboard & mouse outlets to replce
    // what glut offered.
  }

  /** View's width in pixels.
   */
  double view_width_;

  /** View's height in pixels.
   */
  double view_height_;
};

extern "C" void init(Planet &planet) {
  CLASS (GLLua, "GLLua script.", "script: [script content] or file: [path to file]")
  // [1]
  INLET (GLLua, draw, Value(Json("[0,0]")).push_back("Receives [width, height] from an OpenGL thread."))

  ADD_SUPER_METHOD(GLLua, Script, file, StringIO("path", "Set path to script content."))
  ADD_SUPER_METHOD(GLLua, Script, script, StringIO("lua code", "Script content."))
  ADD_SUPER_METHOD(GLLua, Script, reload, RealIO("seconds", "How often shoudl we check file for relaod."))
}
