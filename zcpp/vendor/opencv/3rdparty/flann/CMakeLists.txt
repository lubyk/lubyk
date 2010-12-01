# ----------------------------------------------------------------------------
#  CMake file for libflann. See root CMakeLists.txt
#
# ----------------------------------------------------------------------------
project(flann)

# List of C++ files:

#set(CMAKE_BUILD_TYPE Debug)

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/algorithms
   ${CMAKE_CURRENT_SOURCE_DIR}/util
   ${CMAKE_CURRENT_SOURCE_DIR}/nn
   "${CMAKE_CURRENT_SOURCE_DIR}/../include"
   )

# The .cpp files:
file(GLOB_RECURSE flann_sources_cpp *.cpp)
file(GLOB_RECURSE flann_sources_h1 "${CMAKE_CURRENT_SOURCE_DIR}/../include/flann/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/../include/flann/*.hpp")
file(GLOB_RECURSE flann_sources_h2 *.h *.hpp)

source_group("Src" FILES ${flann_sources_cpp})
source_group("Include\\External" FILES ${flann_sources_h1})
source_group("Include\\Internal" FILES ${flann_sources_h2})

set(flann_sources ${flann_sources_cpp} ${flann_sources_h1} ${flann_sources_h2})

# ----------------------------------------------------------------------------------
#           Define the library target:
# ----------------------------------------------------------------------------------

set(the_target "flann")

add_library(${the_target} STATIC ${flann_sources})
add_definitions(-Dflann_EXPORTS)

set_target_properties(${the_target}
	PROPERTIES OUTPUT_NAME "${the_target}"
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/vendor_lib
	)

if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3")
    add_definitions(-DJAS_WIN_MSVC_BUILD)
endif()

if(UNIX)
  if(CMAKE_COMPILER_IS_GNUCXX OR CV_ICC)
     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
  endif()
endif()

