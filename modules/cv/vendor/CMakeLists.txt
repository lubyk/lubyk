include_directories("${MODULE_SOURCE_DIR}/include/cv")

add_subdirectory(3rdparty)
add_subdirectory(cxcore)
add_subdirectory(cv)
#add_subdirectory(cvaux)
add_subdirectory(ml)
#add_subdirectory(highgui)

# ----------------------------------------------------------------------------------
#  [lubyk]
# ----------------------------------------------------------------------------------
build_sub_module("vendor" lubyk.cpp)
target_link_libraries("${MODULE_NAME}_vendor" libcv cxcore ml opencv_lapack flann zlib ${PLAT_LINK})

# ----------------------------------------------------------------------------------
#  [/lubyk]
# ----------------------------------------------------------------------------------