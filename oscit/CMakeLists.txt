project (OSCIT)
cmake_minimum_required(VERSION 2.8)

# ==============================================================================
#
#  Options
#
# ==============================================================================
option (OSCIT_MEMORY_CHECKING    "Enable checking against memory leaks?"     NO  )
option (OSCIT_ENABLE_TESTING     "Build and run tests ?"                     YES )


# handle memory checking option
if (OSCIT_MEMORY_CHECKING)

  if (APPLE)
    find_library (HAVE_MALLOC_DEBUG MallocDebug ${lib_locations})

    if (HAVE_MALLOC_DEBUG)
      message (STATUS "libMallocDebug = ${HAVE_MALLOC_DEBUG}")
    else (HAVE_MALLOC_DEBUG)
      set (HAVE_MALLOC_DEBUG NO)
    endif (HAVE_MALLOC_DEBUG)
  endif (APPLE)

endif (OSCIT_MEMORY_CHECKING)

# handle testing option
if (OSCIT_ENABLE_TESTING)
  enable_testing()
endif (OSCIT_ENABLE_TESTING)

# ragel executable
set(RAGEL "ragel")

# ==============================================================================
#
#  Platform guessing
#
# ==============================================================================
if(UNIX)
  set(OSCPACK_PLAT "posix")
  if(APPLE)
    set(PLAT "macosx")
    set(PLAT_LINK "")
    set(CMAKE_CXX_FLAGS "-mmacosx-version-min=10.4 -g -Wall")
    if (RELEASE)
      set(CMAKE_OSX_ARCHITECTURES "ppc;i386;x86_64" CACHE STRING "Build architectures for OSX" FORCE)
    endif(RELEASE)
  else(APPLE)
    set(PLAT "linux")
    set(PLAT_LINK avahi-client rt)
    set (CMAKE_CXX_FLAGS "-g -Wall")
  endif(APPLE)
else(UNIX)
  if(WIN32)
    set(PLAT "win32")
    set(OSCPACK_PLAT "win32")
    set (CMAKE_CXX_FLAGS "-g -Wall")
  else(WIN32)
    set(PLAT "unsupported")
  endif(WIN32)
endif(UNIX)

add_definitions(-D__${PLAT}__)

# ==============================================================================
#
#  oscit library build
#
# ==============================================================================
file (GLOB OSCIT_SOURCES src/*.cpp src/${PLAT}/*.cpp)

include_directories (${OSCIT_SOURCE_DIR}/oscpack ${OSCIT_SOURCE_DIR}/include)
include_directories (AFTER ${OSCIT_SOURCE_DIR}/test)

file (GLOB RAGEL_SOURCES src/*.rl)
foreach (RAGEL_SRC ${RAGEL_SOURCES})
  get_filename_component (RAGEL_SRC_NAME ${RAGEL_SRC} NAME_WE)
	add_custom_command ( PRE_BUILD
	  OUTPUT  ${OSCIT_SOURCE_DIR}/src/${RAGEL_SRC_NAME}.cpp
	  COMMAND ${RAGEL} ${RAGEL_SRC} -o ${OSCIT_SOURCE_DIR}/src/${RAGEL_SRC_NAME}.cpp
	  DEPENDS ${RAGEL_SRC}
	)
	# make sure output is included in OSCIT_SOURCES in case 'make clean' removes cached cpp file
	# TODO: how to make sure 'clean' does not remove these generated files ?
	set (OSCIT_SOURCES ${OSCIT_SOURCES} ${OSCIT_SOURCE_DIR}/src/${RAGEL_SRC_NAME}.cpp)
endforeach (RAGEL_SRC)

file (GLOB OSCPACK_SOURCES oscpack/ip/*.cpp oscpack/ip/${OSCPACK_PLAT}/*.cpp oscpack/osc/*.cpp)
add_library (oscit STATIC ${OSCIT_SOURCES} ${OSCPACK_SOURCES})

# not cmake based oscpack
# add_custom_command (
#   OUTPUT  ${OSCIT_SOURCE_DIR}/oscpack/liboscpack.a
#   COMMAND cd ${OSCIT_SOURCE_DIR}/oscpack && make liboscpack.a
# )
# add_custom_target (generate_liboscpack DEPENDS ${OSCIT_SOURCE_DIR}/oscpack/liboscpack.a)
# add_dependencies (oscit generate_liboscpack)

# ==============================================================================
#
#  test_runner build
#
# ==============================================================================
# (one test for all CxxTests)

if(OSCIT_MEMORY_CHECKING)
  file (GLOB OSCIT_TEST_SOURCES test/*_test.h test/*_test_slow.h test/zpause_for_memory_analysis.h)
else(OSCIT_MEMORY_CHECKING)
  file (GLOB OSCIT_TEST_SOURCES test/*_test.h test/*_test_slow.h)
endif(OSCIT_MEMORY_CHECKING)

file (GLOB OSCIT_TEST_MOCKS test/mock/*.h)
add_custom_command ( PRE_BUILD
  OUTPUT  test_runner.cpp
  COMMAND ${OSCIT_SOURCE_DIR}/test/cxxtest/cxxtestgen.pl --error-printer -o test_runner.cpp ${OSCIT_TEST_SOURCES}
  DEPENDS ${OSCIT_TEST_SOURCES} ${OSCIT_TEST_MOCKS}
)

add_executable (test_runner test_runner.cpp)
target_link_libraries (test_runner oscit ${PLAT_LINK})
add_test (oscit_test test_runner)

# this is to enable verbose output during testing
add_custom_target (test_all ALL ${CMAKE_CTEST_COMMAND} -V)
add_dependencies (test_all test_runner)

# avahi testing...
if(PLAT STREQUAL "linux")
	add_executable(publish ${OSCIT_SOURCE_DIR}/test/mock/publish.c)
	target_link_libraries(publish ${PLAT_LINK})

	add_executable(registration ${OSCIT_SOURCE_DIR}/test/mock/registration.cpp)
	target_link_libraries(registration ${PLAT_LINK})
endif(PLAT STREQUAL "linux")


# valgrind command ?
# valgrind --leak-check=full --track-origins=yes ./test_runner


# ==============================================================================
#
#  configuration feedback
#
# ==============================================================================

message (STATUS "===========================================================================")
message (STATUS "   OSCIT build configuration settings                                      ")
message (STATUS "===========================================================================")
message (STATUS "   Platform                                                     = ${PLAT}")
message (STATUS "   OSCIT_ENABLE_TESTING  (Build and run tests)                  = ${OSCIT_ENABLE_TESTING}")
message (STATUS "   OSCIT_MEMORY_CHECKING (Enable checking against memory leaks) = ${OSCIT_MEMORY_CHECKING}")
if(OSCIT_MEMORY_CHECKING)
message (STATUS "       you should run test_runner with")
message (STATUS "       > export MallocStackLogging=1;./test_runner")
endif(OSCIT_MEMORY_CHECKING)
message (STATUS "===========================================================================")
