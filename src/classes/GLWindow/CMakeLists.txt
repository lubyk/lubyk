if(UNIX)
  if(APPLE)
    set(PLAT_OBJ_FLAGS "${PLAT_OBJ_FLAGS} -lobjc -Wno-import")
    add_rko_object(GLWindow GLWindow.cpp macosx/gl_window.mm)
  else(APPLE)

  endif(APPLE)
else(UNIX)
  if(WIN32)

  else(WIN32)

  endif(WIN32)
endif(UNIX)

