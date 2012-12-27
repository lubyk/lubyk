# --------------------------------------------------------------
#  module_core
# --------------------------------------------------------------

# to find dub/dub.h
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB CORE_SOURCES dub/*.cpp *.cpp ${PLAT}/*.cpp ${PLAT}/*.mm)

if(MODULES_STATIC)
  set(target "${MODULE_NAME}_static")
  add_library(${target} ${CORE_SOURCES})
  # --------------------------------------------------------------
  #  .a
  # --------------------------------------------------------------
  install(TARGETS ${target}
    DESTINATION ${MODULES_STATIC}
  )
else(MODULES_STATIC)
  set(target "${MODULE_NAME}_core")
  add_library(${target} MODULE ${CORE_SOURCES})
  set_target_properties(${target}
    PROPERTIES OUTPUT_NAME core_c
    LINK_FLAGS ${PLAT_OBJ_FLAGS}
    PREFIX ""
    SUFFIX ".so"
  )  

  # --------------------------------------------------------------
  #  .so
  # --------------------------------------------------------------
  install(TARGETS ${target}
    DESTINATION ${LUBYK_LIB}/lib/lk
  )
endif(MODULES_STATIC)

target_link_libraries(${target} time_ref)
add_dependencies(${MODULE_NAME} ${target})
