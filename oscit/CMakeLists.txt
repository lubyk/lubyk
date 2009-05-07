cmake_minimum_required(VERSION 2.6)

if(UNIX)
  if(APPLE)
    set(PLAT "macosx")
    set(PLAT_LINK "")
  else(APPLE)
    set(PLAT "linux")
    set(PLAT_LINK avahi-client rt)
  endif(APPLE)
else(UNIX)
  if(WIN32)
    set(PLAT "win32")
  else(WIN32)
    set(PLAT "unsupported")
  endif(WIN32)
endif(UNIX)
MESSAGE("Platform is ${PLAT}")
add_definitions(-D__${PLAT}__)

project (OSCIT)
set (CMAKE_CXX_FLAGS "-g -Wall")
file (GLOB OSCIT_SOURCES src/*.cpp src/${PLAT}/*.cpp)

include_directories (${OSCIT_SOURCE_DIR}/oscpack ${OSCIT_SOURCE_DIR}/include)
include_directories (AFTER ${OSCIT_SOURCE_DIR}/test)

add_library (oscit STATIC ${OSCIT_SOURCES})

# oscpack
add_custom_command (
  OUTPUT  ${OSCIT_SOURCE_DIR}/oscpack/liboscpack.a
  COMMAND cd ${OSCIT_SOURCE_DIR}/oscpack && make liboscpack.a
)
message("oscpack: '${OSCIT_SOURCE_DIR}/oscpack/liboscpack.a'")

# testing

# one test for all CxxTests
enable_testing()
file (GLOB OSCIT_TEST_SOURCES test/*_xxtest.h test/*_test_slow.h)
add_custom_command ( PRE_BUILD
  OUTPUT  test_runner.cpp
  COMMAND ${OSCIT_SOURCE_DIR}/test/cxxtest/cxxtestgen.pl --error-printer -o test_runner.cpp ${OSCIT_TEST_SOURCES}
  DEPENDS ${OSCIT_TEST_SOURCES}
)

add_executable (test_runner test_runner.cpp ${OSCIT_SOURCE_DIR}/oscpack/liboscpack.a)

target_link_libraries (test_runner oscit ${OSCIT_SOURCE_DIR}/oscpack/liboscpack.a ${PLAT_LINK})
add_test (oscit_test test_runner)

# this is to enable verbose output during testing
add_custom_target (test_all ALL ${CMAKE_CTEST_COMMAND} -V)
add_dependencies (test_all test_runner)

# one test for each file
# # this is to enable verbose output during testing
# add_custom_target (test ALL ${CMAKE_CTEST_COMMAND} -V)
# enable_testing()
# file (GLOB OSCIT_TEST_SOURCES test/*_test.h test/*_test_slow.h)
# foreach (TEST_SOURCE ${OSCIT_TEST_SOURCES})
#   get_filename_component (TEST_NAME ${TEST_SOURCE} NAME_WE)
#   message ("Building test for ${TEST_NAME}...")
#   add_custom_command (
#     OUTPUT  ${TEST_NAME}.cpp
#     COMMAND ${OSCIT_SOURCE_DIR}/test/cxxtest/cxxtestgen.pl --error-printer -o ${TEST_NAME}.cpp ${TEST_SOURCE})
#
#   add_executable (${TEST_NAME}.runner ${TEST_NAME}.cpp)
#
#   target_link_libraries (${TEST_NAME}.runner oscit ${OSCIT_SOURCE_DIR}/oscpack/liboscpack.a)
#   add_test (${TEST_NAME} ${TEST_NAME}.runner)
#   add_dependencies(test ${TEST_NAME}.runner)
# endforeach (TEST_SOURCE)

add_executable(publish publish.c)
target_link_libraries(publish avahi-client)
