#include "rubyk.h"
#include "lua_script.h"

extern "C" {
#include "gllua/LuaGL.h"
}

class GLLua : public LuaScript {
public:
  // inlet 1
  void draw(const Value &val) {
    if (!is_opengl_thread()) {
      fprintf(stderr, "%s: cannot call draw (not an OpenGL thread)\n", name_.c_str());
      return;
    }

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    call_lua("draw", val);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
  }
protected:
  /* open all standard libraries and openGL libraries (called by LuaScript on init) */
  virtual void open_lua_libs() {
    LuaScript::open_lua_libs();
    open_lua_lib("opengl", luaopen_opengl);
    // TODO: adapt GLWindow with keyboard & mouse outlets to replce
    // what glut offered.
  }
};

extern "C" void init(Planet &planet) {
  CLASS (GLLua, "GLLua script.", "script: [script content] or file: [path to file]")
  // {1}
  INLET (GLLua, draw, NilIO("receives bangs from an OpenGL thread"))

  ADD_SUPER_METHOD(GLLua, Script, file, StringIO("path", "Set path to script content."))
  ADD_SUPER_METHOD(GLLua, Script, script, StringIO("lua code", "Script content."))
  ADD_SUPER_METHOD(GLLua, Script, reload, RealIO("seconds", "How often shoudl we check file for relaod."))
}
