
# ----------------------------------------------------------------------------------
#  [lubyk]
# ----------------------------------------------------------------------------------

include_directories(src)

file(GLOB SUB_MODULE_SOURCES *.c)
set(SUB_MODULE_NAME "vendor")
find_package(OpenGL REQUIRED)

build_sub_module("vendor" ${SUB_MODULE_SOURCES})

target_link_libraries("${MODULE_NAME}_vendor" ${OPENGL_LIBRARIES})

# ----------------------------------------------------------------------------------
#  [/lubyk]
# ----------------------------------------------------------------------------------