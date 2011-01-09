# ----------------------------------------------------------------------------
#  CMake file for ml. See root CMakeLists.txt
# ----------------------------------------------------------------------------
project(ml)

file(GLOB lib_srcs "*.cpp")
source_group("Src" FILES ${lib_srcs})
set(lib_hdrs ${MODULE_SOURCE_DIR}/include/cv/ml.h)
source_group("Include\\External" FILES ${lib_hdrs})
file(GLOB lib_int_hdrs "*.h*")
source_group("Include\\Internal" FILES ${lib_int_hdrs})

add_definitions(-DCVAPI_EXPORTS)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}"
                    "${CMAKE_CURRENT_BINARY_DIR}")

set(the_target "ml")

add_library(${the_target} ${lib_srcs} ${lib_hdrs} ${lib_int_hdrs})

if(PCHSupport_FOUND)
    set(pch_header ${CMAKE_CURRENT_SOURCE_DIR}/_ml.h)
    if(${CMAKE_GENERATOR} MATCHES "Visual*" OR ${CMAKE_GENERATOR} MATCHES "Xcode*")
        if(${CMAKE_GENERATOR} MATCHES "Visual*")
            set(${the_target}_pch "ml.cpp")
        endif()
        add_native_precompiled_header(${the_target} ${pch_header})
    elseif(CMAKE_COMPILER_IS_GNUCXX AND ${CMAKE_GENERATOR} MATCHES ".*Makefiles")
        add_precompiled_header(${the_target} ${pch_header})
    endif()
endif()

set_target_properties(${the_target}
	PROPERTIES OUTPUT_NAME "${the_target}"
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/vendor_lib
	)

# Linker flag needed for Windows Mobile 5 and 6 SDKs
if(MSVC AND CMAKE_CROSSCOMPILING)
	set_target_properties(${the_target} PROPERTIES
		LINK_FLAGS "/NODEFAULTLIB:secchk"
		)
endif()

# Add the required libraries for linking:
target_link_libraries(${the_target} ${OPENCV_LINKER_LIBS} cxcore)

# Dependencies of this target:
add_dependencies(${the_target} cxcore)
