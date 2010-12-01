if(UNIX)
  if(APPLE)
    set(PLAT_OBJ_FLAGS "${PLAT_OBJ_FLAGS} -lobjc -Wno-import -framework QTKit")
    add_rko_object(VideoIn VideoIn.cpp macosx/video_in.mm)
  else(APPLE)

  endif(APPLE)
else(UNIX)
  if(WIN32)

  else(WIN32)

  endif(WIN32)
endif(UNIX)

