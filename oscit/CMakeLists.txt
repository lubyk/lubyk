cmake_minimum_required(VERSION 2.8)
set(RAGEL "ragel")
if(UNIX)
  set(OSCPACK_PLAT "posix")
  if(APPLE)
    set(PLAT "macosx")
    set(PLAT_LINK "")
    set(CMAKE_CXX_FLAGS "-mmacosx-version-min=10.4 -g -Wall")
    set(CMAKE_OSX_ARCHITECTURES "ppc;i386;x86_64" CACHE STRING "Build architectures for OSX" FORCE)
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
MESSAGE("Platform is ${PLAT}")
add_definitions(-D__${PLAT}__)

project (OSCIT)
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

# testing

# one test for all CxxTests
enable_testing()
file (GLOB OSCIT_TEST_SOURCES test/*_test.h test/*_test_slow.h)
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
