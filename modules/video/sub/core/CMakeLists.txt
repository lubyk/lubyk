if(UNIX)
  if(APPLE)
    set(PLAT_OBJ_FLAGS "${PLAT_OBJ_FLAGS} -lobjc -Wno-import -framework QTKit")
    # Macro defined in the top level Lubyk CMakeLists.txt
    build_sub_module_with_defaults()
  else(APPLE)

  endif(APPLE)
else(UNIX)
  if(WIN32)

  else(WIN32)

  endif(WIN32)
endif(UNIX)

