# --------------------------------------------------------------
#  time_ref.a
# --------------------------------------------------------------
file(GLOB SOURCES *.cpp ${PLAT}/*.cpp ${PLAT}/*.mm)
add_library(time_ref ${SOURCES})

if(LINUX)
  target_link_libraries("time_ref" -lrt)
endif(LINUX)

if(MODULES_STATIC)
  # --------------------------------------------------------------
  #  .a
  # --------------------------------------------------------------
  install(TARGETS "time_ref"
    DESTINATION ${MODULES_STATIC}
  )
endif(MODULES_STATIC)
