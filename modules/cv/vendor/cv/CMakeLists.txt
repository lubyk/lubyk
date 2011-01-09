# ----------------------------------------------------------------------------
#  CMake file for cv. See root CMakeLists.txt
# ----------------------------------------------------------------------------
project(cv)

file(GLOB lib_srcs "*.cpp")
source_group("Src" FILES ${lib_srcs})

add_definitions(-DCVAPI_EXPORTS)

set(lib_hdr_names cv.h cv.hpp cvcompat.h cvtypes.h)
set(lib_hdrs)
foreach(h ${lib_hdr_names})
    list(APPEND lib_hdrs "${MODULE_SOURCE_DIR}/include/cv/${h}")
endforeach()
source_group("Include\\External" FILES ${lib_hdrs})

file(GLOB lib_int_hdrs "*.h*")
source_group("Include\\Internal" FILES ${lib_int_hdrs})

set(the_target "libcv")

add_library(${the_target} STATIC ${lib_srcs} ${lib_hdrs} ${lib_int_hdrs})

set_target_properties(${the_target}
	PROPERTIES OUTPUT_NAME "${the_target}"
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/vendor_lib
	)


# if(PCHSupport_FOUND)
#     set(pch_header ${CMAKE_CURRENT_SOURCE_DIR}/_cv.h)
#     if(${CMAKE_GENERATOR} MATCHES "Visual*" OR ${CMAKE_GENERATOR} MATCHES "Xcode*")
#         if(${CMAKE_GENERATOR} MATCHES "Visual*")
#             set(${the_target}_pch "cvprecomp.cpp")
#         endif()
#         add_native_precompiled_header(${the_target} ${pch_header})
#     elseif(CMAKE_COMPILER_IS_GNUCXX AND ${CMAKE_GENERATOR} MATCHES ".*Makefiles")
#         add_precompiled_header(${the_target} ${pch_header})
#     endif()
# endif()
#