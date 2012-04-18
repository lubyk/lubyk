# --------------------------------------------------------------
#  module_core                                 module/core.so
# --------------------------------------------------------------
set(target "${MODULE_NAME}_core")

include_directories(${LUBYK_MODULES}/lk/include ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB CORE_SOURCES dub/*.cpp *.cpp ${PLAT}/*.cpp ${PLAT}/*.mm)

add_library(${target} MODULE ${CORE_SOURCES})
set_target_properties(${target}
  PROPERTIES OUTPUT_NAME core
  LINK_FLAGS ${PLAT_OBJ_FLAGS}
  PREFIX ""
  SUFFIX ".so"
)
if(LINUX)
  target_link_libraries(${target} ${PLAT_LINK} -lavahi-client -lpthread)
else(LINUX)
  target_link_libraries(${target} ${PLAT_LINK})
endif(LINUX)

add_dependencies(${MODULE_NAME} ${target})

# --------------------------------------------------------------
#  install                                     
# --------------------------------------------------------------
install(TARGETS ${target}
  DESTINATION lib/lubyk/${MODULE_NAME}
)

