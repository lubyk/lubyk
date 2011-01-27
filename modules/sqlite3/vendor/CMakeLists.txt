set(SUB_MODULE_NAME "vendor")
set(SUB_MODULE_SOURCES LuaSQLite/lsqlite3.c sqlite3/sqlite3.c)
include_directories(sqlite3)

# create name/[SUB_MOD_NAME].so library
add_library("${target}_${SUB_MODULE_NAME}" MODULE ${SUB_MODULE_SOURCES})
set_target_properties("${target}_${SUB_MODULE_NAME}"
  PROPERTIES OUTPUT_NAME ${SUB_MODULE_NAME}
  LINK_FLAGS ${PLAT_OBJ_FLAGS}
  LIBRARY_OUTPUT_DIRECTORY ${LUBYK_SOURCE_DIR}/lib/${MODULE_NAME}
  PREFIX ""
  SUFFIX ".so"
)
add_dependencies(${target} "${target}_${SUB_MODULE_NAME}")