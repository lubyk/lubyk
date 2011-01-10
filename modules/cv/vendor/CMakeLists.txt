include_directories("${MODULE_SOURCE_DIR}/include/cv")

add_subdirectory(3rdparty)
add_subdirectory(cxcore)
add_subdirectory(cv)
#add_subdirectory(cvaux)
add_subdirectory(ml)
#add_subdirectory(highgui)

# ----------------------------------------------------------------------------------
#  [rubyk]
# ----------------------------------------------------------------------------------

set(STATIC_LIBS "")
list(APPEND STATIC_LIBS libcv cxcore ml opencv_lapack flann zlib)
set(SUB_MODULE_NAME "vendor")
set(SUB_MODULE_SOURCES rubyk.cpp)

add_library("${target}_${SUB_MODULE_NAME}" MODULE ${SUB_MODULE_SOURCES})
set_target_properties("${target}_${SUB_MODULE_NAME}"
  PROPERTIES OUTPUT_NAME ${SUB_MODULE_NAME}
  LINK_FLAGS ${PLAT_OBJ_FLAGS}
  LIBRARY_OUTPUT_DIRECTORY ${RUBYK_SOURCE_DIR}/lib/${MODULE_NAME}
  PREFIX ""
  SUFFIX ".so"
)
target_link_libraries("${target}_${SUB_MODULE_NAME}" ${STATIC_LIBS} ${PLAT_LINK})
add_dependencies (modules "${target}_${SUB_MODULE_NAME}")

# ----------------------------------------------------------------------------------
#  [/rubyk]
# ----------------------------------------------------------------------------------