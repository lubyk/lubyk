if(UNIX)
  if(APPLE)
    set(PLAT_OBJ_FLAGS "${PLAT_OBJ_FLAGS} -framework OpenGL")
  else(APPLE)

  endif(APPLE)
else(UNIX)
  if(WIN32)

  else(WIN32)

  endif(WIN32)
endif(UNIX)

add_rko_object(GLLua GLLua.cpp LuaGL.c)
#  lib/GLLua.rko: src/objects/GLLua.cpp src/objects/gllua/LuaGL.c src/objects/gllua/LuaGlut.c $(OBJECT_DEPENDS)
#  	$(CC) $(CFLAGS) $(OBJECT_COMPILE) -Isrc/objects/gllua -framework GLUT -framework OpenGL -framework Cocoa src/objects/GLLua.cpp src/objects/gllua/LuaGL.c src/objects/gllua/LuaGlut.c -o $@
