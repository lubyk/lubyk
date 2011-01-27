include_directories(src)
set(target "glut")

find_package(GLUT REQUIRED)

file (GLOB MODULE_SOURCES ${MODULE}/src/*.cpp ${MODULE}/src/${PLAT}/*.cpp ${MODULE}/src/*.c ${MODULE}/src/${PLAT}/*.c)

# create name.so library
add_library(${target} MODULE ${MODULE_SOURCES})
set_target_properties(${target}
  PROPERTIES OUTPUT_NAME ${target}
  LINK_FLAGS ${PLAT_OBJ_FLAGS}
  LIBRARY_OUTPUT_DIRECTORY ${LUBYK_SOURCE_DIR}/lib
  PREFIX ""
  SUFFIX ".so"
)
target_link_libraries(${target} ${GLUT_LIBRARIES})
add_dependencies(modules ${target})
