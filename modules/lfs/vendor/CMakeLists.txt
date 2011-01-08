
# ----------------------------------------------------------------------------------
#  [rubyk]
# ----------------------------------------------------------------------------------

include_directories(src)

file(GLOB SUB_MODULE_SOURCES src/*.c)
set(SUB_MODULE_NAME "vendor")


add_library("${target}_${SUB_MODULE_NAME}" MODULE ${SUB_MODULE_SOURCES})
set_target_properties("${target}_${SUB_MODULE_NAME}"
  PROPERTIES OUTPUT_NAME ${SUB_MODULE_NAME}
  LINK_FLAGS ${PLAT_OBJ_FLAGS}
  LIBRARY_OUTPUT_DIRECTORY ${RUBYK_SOURCE_DIR}/lib/${MODULE_NAME}
  PREFIX ""
  SUFFIX ".so"
)
add_dependencies (modules "${target}_${SUB_MODULE_NAME}")

# ----------------------------------------------------------------------------------
#  [/rubyk]
# ----------------------------------------------------------------------------------