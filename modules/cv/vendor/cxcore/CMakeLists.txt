# ----------------------------------------------------------------------------
#  CMake file for cxcore. See root CMakeLists.txt
# ----------------------------------------------------------------------------
project(cxcore)

file(GLOB lib_srcs "*.cpp")
source_group("Src" FILES ${lib_srcs})

add_definitions(-DCVAPI_EXPORTS)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/include")

set(lib_hdr_names cxcore.h cxcore.hpp cxerror.h cxmat.hpp cxmisc.h cxoperations.hpp cxtypes.h cvver.h cvwimage.h cxflann.h)
set(lib_hdrs)
foreach(h ${lib_hdr_names})
  list(APPEND lib_hdrs "${MODULE_SOURCE_DIR}/include/cv/${h}")
endforeach()
source_group("Include\\External" FILES ${lib_hdrs})

file(GLOB lib_int_hdrs "*.h*")
source_group("Include\\Internal" FILES ${lib_int_hdrs})

set(the_target "cxcore")

add_library(${the_target} ${lib_srcs} ${lib_hdrs} ${lib_int_hdrs})

# if(PCHSupport_FOUND)
#     set(pch_header ${CMAKE_CURRENT_SOURCE_DIR}/_cxcore.h)
#     if(${CMAKE_GENERATOR} MATCHES "Visual*" OR ${CMAKE_GENERATOR} MATCHES "Xcode*")
#         if(${CMAKE_GENERATOR} MATCHES "Visual*")
#             set(${the_target}_pch "cxprecomp.cpp")
#         endif()
#         add_native_precompiled_header(${the_target} ${pch_header})
#     elseif(CMAKE_COMPILER_IS_GNUCXX AND ${CMAKE_GENERATOR} MATCHES ".*Makefiles")
#         add_precompiled_header(${the_target} ${pch_header})
#     endif()
# endif()

# Add the required libraries for linking:
target_link_libraries(${the_target} ${OPENCV_LINKER_LIBS} ${IPP_LIBS} opencv_lapack zlib flann)

add_dependencies(${the_target} opencv_lapack zlib flann)

set_target_properties(${the_target}
	PROPERTIES OUTPUT_NAME "${the_target}"
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/vendor_lib
	)
