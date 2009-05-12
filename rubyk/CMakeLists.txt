cmake_minimum_required(VERSION 2.6)
set(RAGEL "ragel")
if(UNIX)
  if(APPLE)
    set(PLAT "macosx")
		add_definitions(-D__MACOSX_CORE__)
    set(PLAT_LINK "-framework Accelerate")
		set(PLAT_OBJ_FLAGS "${PLAT_LINK} -dynamic -bundle")
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
add_definitions(-D__${PLAT}__ -DUSE_READLINE)

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Wall -O2 -g")
set(CMAKE_CXX_FLAGS_RELEASE        "-Wall -O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG          "-Wall -O2 -g -0b0 -noalign")
set(CMAKE_CXX_FLAGS_DEBUGFULL      "-Wall -g -Ob0 -noalign")

project (RUBYK)

# ----------- TESTING -------------

include_directories (AFTER ${RUBYK_SOURCE_DIR}/test)

# one test for all CxxTests
enable_testing()
file (GLOB RUBYK_TEST_SOURCES test/*_test.h src/objects/*_test.h src/lib_objects/*/*_test.h)
file (GLOB RUBYK_TEST_MOCKS test/mock/*.h)
add_custom_command (PRE_BUILD
  OUTPUT  test_runner.cpp
  COMMAND ${RUBYK_SOURCE_DIR}/test/cxxtest/cxxtestgen.pl --error-printer -o test_runner.cpp ${RUBYK_TEST_SOURCES}
  DEPENDS ${RUBYK_TEST_SOURCES} ${OSCIT_TEST_MOCKS}
)

add_executable (test_runner test_runner.cpp)
target_link_libraries (test_runner rubyk_core)
add_test (oscit_test test_runner)

# this is to enable verbose output during testing
add_custom_target (test_all ALL ${CMAKE_CTEST_COMMAND} -V)
add_dependencies (test_all test_runner)

# ----------- RUBYK -------------

file (GLOB RUBYK_SOURCES src/core/*.cpp src/core/${PLAT}/*.cpp)
file (GLOB RUBYK_INCLUDES ${RUBYK_SOURCE_DIR}/src/core ${RUBYK_SOURCE_DIR}/src/lib_objects)

include_directories (${RUBYK_INCLUDES} ${RUBYK_SOURCE_DIR}/src/lib/lua ${RUBYK_SOURCE_DIR}/../oscit/include)

file (GLOB RAGEL_SOURCES src/core/*.rl)
foreach (RAGEL_SRC ${RAGEL_SOURCES})
  get_filename_component (RAGEL_SRC_NAME ${RAGEL_SRC} NAME_WE)
	add_custom_command ( PRE_BUILD
	  OUTPUT  ${RUBYK_SOURCE_DIR}/src/core/${RAGEL_SRC_NAME}.cpp
	  COMMAND ${RAGEL} ${RAGEL_SRC} -o ${RUBYK_SOURCE_DIR}/src/core/${RAGEL_SRC_NAME}.cpp
	  DEPENDS ${RAGEL_SRC}
	)
	# make sure output is included in RUBYK_SOURCES in case 'make clean' removes cached cpp file
	# TODO: how to make sure 'clean' does not remove these generated files ?
	set (RUBYK_SOURCES ${RUBYK_SOURCES} ${RUBYK_SOURCE_DIR}/src/core/${RAGEL_SRC_NAME}.cpp)
endforeach (RAGEL_SRC)

add_library (rubyk_core SHARED ${RUBYK_SOURCES})
add_dependencies (rubyk_core ${RUBYK_SOURCE_DIR}/../oscit/build/liboscit.a)
target_link_libraries (rubyk_core ${RUBYK_SOURCE_DIR}/src/lib/lua/liblua.a ${RUBYK_SOURCE_DIR}/../oscit/build/liboscit.a ${RUBYK_SOURCE_DIR}/../oscit/oscpack/liboscpack.a -lreadline ${PLAT_LINK})

add_executable(rubyk ${RUBYK_SOURCE_DIR}/src/main.cpp)
target_link_libraries (rubyk rubyk_core)

# ----------- objects (all)

macro (add_rko_object RKO_NAME)
	add_library (${RKO_NAME} MODULE ${ARGN}) # ARGN = list of arguments passed last expected argument
	target_link_libraries (${RKO_NAME} rubyk_core)
	set_target_properties (${RKO_NAME} PROPERTIES 
		LIBRARY_OUTPUT_DIRECTORY ${RUBYK_SOURCE_DIR}/lib
		LINK_FLAGS ${PLAT_OBJ_FLAGS}
		PREFIX ""
		SUFFIX ".rko"
	)
	add_dependencies (test_runner ${RKO_NAME})
endmacro(add_rko_object)

# ----------- objects (simple build)

file (GLOB RUBYK_OBJECT_SOURCES src/objects/*.cpp)

foreach (RUBYK_OBJECT_SOURCE ${RUBYK_OBJECT_SOURCES})
	get_filename_component (RUBYK_OBJECT_NAME ${RUBYK_OBJECT_SOURCE} NAME_WE)
	
	add_rko_object (${RUBYK_OBJECT_NAME} ${RUBYK_OBJECT_SOURCE})
endforeach (RUBYK_OBJECT_SOURCE)

# ----------- lib_objects (special, complicated builds)

file (GLOB RUBYK_LIB_OBJECTS src/lib_objects/*)

foreach (RUBYK_LIB_OBJECT ${RUBYK_LIB_OBJECTS})
	add_subdirectory (${RUBYK_LIB_OBJECT})
endforeach (RUBYK_LIB_OBJECT)
