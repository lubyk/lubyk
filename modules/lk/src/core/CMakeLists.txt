# --------------------------------------------------------------
#  core
# --------------------------------------------------------------
set(target "${MODULE_NAME}_core")
file(GLOB CORE_SOURCES dub/*.cpp *.cpp ${PLAT}/*.cpp ${PLAT}/*.mm)
add_library(${target} MODULE ${CORE_SOURCES})
set_target_properties(${target}
  PROPERTIES OUTPUT_NAME core
  LINK_FLAGS ${PLAT_OBJ_FLAGS}
  PREFIX ""
  SUFFIX ".so"
)  
# msgpack needed for lk.Socket
# time_ref needed by lk.TimeRef
target_link_libraries(${target} msgpack time_ref lua_dump)

add_dependencies(${MODULE_NAME} ${target})

# --------------------------------------------------------------
#  install                                     
# --------------------------------------------------------------
install(TARGETS ${target}
  DESTINATION lib/lubyk/${MODULE_NAME}
)
