cmake_minimum_required(VERSION 2.6)

IF(UNIX)
  IF(APPLE)
    SET(PLAT "macosx")
  ELSE(APPLE)
    SET(PLAT "linux")
  ENDIF(APPLE)
ELSE(UNIX)
  IF(WIN32)
    SET(PLAT "win32")
  ELSE(WIN32)
    SET(GUI "unsupported")
  ENDIF(WIN32)
ENDIF(UNIX)
MESSAGE("Platform is ${PLAT}")

project (OSCIT)
set (CMAKE_CXX_FLAGS "-g -Wall")
file (GLOB OSCIT_COMMON_SOURCES src/*.cpp)
set (OSCIT_SOURCES
${OSCIT_COMMON_SOURCES}
src/${PLAT}/zeroconf_browser.cpp
src/${PLAT}/zeroconf_registration.cpp
)

include_directories (${OSCIT_SOURCE_DIR}/oscpack ${OSCIT_SOURCE_DIR}/include)
include_directories (AFTER ${OSCIT_SOURCE_DIR}/test)

add_library (oscit STATIC ${OSCIT_SOURCES})

# testing

# one test for all CxxTests
enable_testing()
file (GLOB OSCIT_TEST_SOURCES test/*_test.h test/*_test_slow.h)
add_custom_command (
  OUTPUT  ${OSCIT_SOURCE_DIR}/test/runner.cpp
  COMMAND ${OSCIT_SOURCE_DIR}/test/cxxtest/cxxtestgen.pl --error-printer -o ${OSCIT_SOURCE_DIR}/test/runner.cpp ${OSCIT_TEST_SOURCES})

add_executable (test_runner test/runner.cpp)

target_link_libraries (test_runner oscit ${OSCIT_SOURCE_DIR}/oscpack/liboscpack.a)
add_test (oscit_test test_runner)

# this is to enable verbose output during testing
add_custom_target (test_all ALL ${CMAKE_CTEST_COMMAND} -V)
add_dependencies(test_all test_runner)

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
