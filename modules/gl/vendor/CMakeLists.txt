
# ----------------------------------------------------------------------------------
#  [lubyk]
# ----------------------------------------------------------------------------------

#include_directories(CMAKE_CURRENT_DIR)

#set(SUB_MODULE_NAME "luagl")
find_package(OpenGL REQUIRED)

build_sub_module("luagl"  LuaGL.c luagl_util.c)
build_sub_module("luaglu" luaglu.c)

target_link_libraries("${MODULE_NAME}_luagl" ${OPENGL_LIBRARIES})

# ----------------------------------------------------------------------------------
#  [/lubyk]
# ----------------------------------------------------------------------------------
