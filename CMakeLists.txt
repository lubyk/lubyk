cmake_minimum_required(VERSION 2.6.2)

# ==============================================================================
#
#  Rubyk
#
# ==============================================================================

project (RUBYK)
set(RUBYK_VERSION "0.5")

# ==============================================================================
#
#  Options
#
# ==============================================================================
option (RUBYK_MEMORY_CHECKING    "Enable checking against memory leaks ?"       OFF )
option (RUBYK_ENABLE_TESTING     "Build and run tests ?"                        ON  )
option (RUBYK_USE_LUA_JIT        "Use Lua Just In Time compiler ?"              OFF )

# ==============================================================================
#
#  Doxygen
#
# ==============================================================================
find_package(Doxygen)

if(DOXYGEN_FOUND)
    set(RUBYK_BUILD_DOXYGEN_DOCS ON CACHE BOOL "Build HTML+XML documentation with Doxygen")
endif()

set (luaopen)
# handle memory checking option
if (RUBYK_MEMORY_CHECKING)
  if (APPLE)
    find_library (HAVE_MALLOC_DEBUG MallocDebug ${lib_locations})

    if (HAVE_MALLOC_DEBUG)
      message (STATUS "libMallocDebug = ${HAVE_MALLOC_DEBUG}")
    else (HAVE_MALLOC_DEBUG)
      set (HAVE_MALLOC_DEBUG NO)
    endif (HAVE_MALLOC_DEBUG)
  endif (APPLE)
endif (RUBYK_MEMORY_CHECKING)

# handle testing option
if (RUBYK_ENABLE_TESTING)
  enable_testing()
endif (RUBYK_ENABLE_TESTING)

# ragel executable
set(RAGEL "ragel")

# ==============================================================================
#
#  Platform guessing
#
# ==============================================================================
if(UNIX)
  if(APPLE)
    set(PLAT "macosx")
    add_definitions(-DLUA_USE_MACOSX)
    add_definitions(-D__MACOSX_CORE__)
    # CoreFoundation needed by Midi and cannot be added in dylib
    set(PLAT_LINK "-all_load -flat_namespace -lreadline -framework Accelerate -framework CoreFoundation -framework OpenGL -framework GLUT -framework Cocoa -lobjc") # -framework Cocoa -lobjc ===> planet_gui.mm
    set(PLAT_OBJ_FLAGS "-flat_namespace -undefined suppress -dynamic -bundle")

    # set(CMAKE_CXX_FLAGS "-mmacosx-version-min=10.6 -g -Wall")
    set(CMAKE_CXX_FLAGS "-g -Wall")
    if (RELEASE)
      set(CMAKE_OSX_ARCHITECTURES "ppc;i386;x86_64" CACHE STRING "Build architectures for OSX" FORCE)
    endif(RELEASE)
  else(APPLE)
    set(PLAT "linux")
    set(CMAKE_CXX_FLAGS "-g -Wall")
    add_definitions(-D__LINUX_ALSASEQ__)
    set(PLAT_LINK "-lavahi-client -lrt -lreadline -lasound")
    set(PLAT_OBJ_FLAGS "${PLAT_LINK} -dynamic")
  endif(APPLE)
else(UNIX)
  if(WIN32)
    set(PLAT "win32")
    set(CMAKE_CXX_FLAGS "-g -Wall")
  else(WIN32)
    set(PLAT "unsupported")
  endif(WIN32)
endif(UNIX)
MESSAGE("Platform is ${PLAT}")

if (RELEASE)
  add_definitions(-O2)
endif(RELEASE)
add_definitions(-D__${PLAT}__ -DUSE_READLINE)

set(STATIC_LIBS)

# TODO: learn how to use these flags...
# set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Wall -O2 -g")
# set(CMAKE_CXX_FLAGS_RELEASE        "-Wall -O2 -DNDEBUG")
# set(CMAKE_CXX_FLAGS_DEBUG          "-Wall -O2 -g -0b0 -noalign")
# set(CMAKE_CXX_FLAGS_DEBUGFULL      "-Wall -g -Ob0 -noalign")

# ==============================================================================
#
#  VENDOR
#
# ==============================================================================

# ------------------------------------------------ cv, cxcore, ml, opencv_lapack, flann, zlib
# we need to cache now so that we overwrite option in OSCIT
set(OSCIT_OPENCV_LINK    OFF CACHE BOOLEAN "doc")
set(OSCIT_ENABLE_TESTING OFF CACHE BOOLEAN "doc")
add_subdirectory(vendor/oscit)
list(APPEND STATIC_LIBS oscit)

if (RUBYK_USE_LUA_JIT)
# ------------------------------------------------ luajit
  add_subdirectory(vendor/lua-jit2)
  list(APPEND STATIC_LIBS luajit)
else(RUBYK_USE_LUA_JIT)

# ------------------------------------------------ lua
  add_subdirectory(vendor/lua)
  list(APPEND STATIC_LIBS lua)
endif(RUBYK_USE_LUA_JIT)

# ------------------------------------------------ lua_bindings (static)
add_subdirectory(vendor/lua_bindings)
list(APPEND STATIC_LIBS lua_bindings)

# ------------------------------------------------ lua_modules
add_custom_target(lua_modules)
add_subdirectory(vendor/lua_modules)

# ------------------------------------------------ cv, cxcore, ml, opencv_lapack, flann, zlib
add_subdirectory(vendor/opencv)
list(APPEND STATIC_LIBS cv cxcore ml opencv_lapack flann zlib)

# ==============================================================================
#
#  rubyk build
#
# ==============================================================================

file (GLOB RUBYK_SOURCES src/core/*.cpp src/core/${PLAT}/* src/support/*)

include_directories (${RUBYK_SOURCE_DIR}/include ${RUBYK_SOURCE_DIR}/include/rubyk/lua  ${RUBYK_SOURCE_DIR}/vendor/oscit/include ${RUBYK_SOURCE_DIR}/vendor/oscit/oscpack ${RUBYK_SOURCE_DIR}/src/classes)

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

add_library(lrubyk STATIC ${RUBYK_SOURCES})
list(APPEND STATIC_LIBS lrubyk)
add_dependencies (lrubyk lua_modules)

add_executable(rubyk ${RUBYK_SOURCE_DIR}/src/main.cpp ${CUSTOM_BUILD_DEPENDS})


target_link_libraries (rubyk ${STATIC_LIBS} ${CUSTOM_BUILD_DEPENDS} ${PLAT_LINK})

# ==============================================================================
#
#  rko classes build
#
# ==============================================================================

add_custom_target(objects)

macro (add_rko_object RKO_NAME)
  add_library (${RKO_NAME} MODULE ${ARGN}) # ARGN = list of arguments passed after RKO_NAME
  set_target_properties (${RKO_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${RUBYK_SOURCE_DIR}/build/lib
    LINK_FLAGS ${PLAT_OBJ_FLAGS}
    PREFIX ""
    SUFFIX ".rko"
  )
  add_dependencies (objects ${RKO_NAME})
endmacro(add_rko_object)

# ----------- classes (simple build)

file (GLOB RUBYK_OBJECT_SOURCES src/classes/*.cpp)

foreach (RUBYK_OBJECT_SOURCE ${RUBYK_OBJECT_SOURCES})
  get_filename_component (RUBYK_OBJECT_NAME ${RUBYK_OBJECT_SOURCE} NAME_WE)

  add_rko_object (${RUBYK_OBJECT_NAME} ${RUBYK_OBJECT_SOURCE})
endforeach (RUBYK_OBJECT_SOURCE)

# ----------- classes/xxx (special, complicated builds)

file (GLOB RUBYK_LIB_OBJECTS   src/classes/*)
file (GLOB RUBYK_OBJECTS_TESTS src/classes/*.h)
file (GLOB RUBYK_OBJECTS_BAD   src/classes/.*)

# -- Filter out all *.cpp and *.h from the 'classes' folder
list (REMOVE_ITEM RUBYK_LIB_OBJECTS ${RUBYK_OBJECT_SOURCES} ${RUBYK_OBJECTS_TESTS} ${RUBYK_OBJECTS_BAD})

foreach (RUBYK_LIB_OBJECT ${RUBYK_LIB_OBJECTS})
  add_subdirectory (${RUBYK_LIB_OBJECT})
endforeach (RUBYK_LIB_OBJECT)

# ==============================================================================
#
#  test build
#
# ==============================================================================

include_directories (AFTER ${RUBYK_SOURCE_DIR}/test)

# one test for all CxxTests
file (GLOB RUBYK_TEST_SOURCES test/*_test.h src/classes/*_test.h src/classes/*/*_test.h)
file (GLOB RUBYK_TEST_MOCKS test/mock/*.h)
add_custom_command (PRE_BUILD
  OUTPUT  test_runner.cpp
  COMMAND ${RUBYK_SOURCE_DIR}/test/cxxtest/cxxtestgen.pl --error-printer -o test_runner.cpp ${RUBYK_TEST_SOURCES}
  DEPENDS ${RUBYK_TEST_SOURCES} ${OSCIT_TEST_MOCKS}
)

add_executable (test_runner test_runner.cpp ${CUSTOM_BUILD_DEPENDS})

target_link_libraries (test_runner -all_load ${STATIC_LIBS} ${CUSTOM_BUILD_DEPENDS} ${PLAT_LINK})

add_test (rubyk_test test_runner)

# this is to enable verbose output during testing
add_custom_target (test_all ALL ${CMAKE_CTEST_COMMAND} -V)
add_dependencies (test_all test_runner)
add_dependencies (test_runner objects)


# ==============================================================================
#
#  Documentation / xml for bindings
#
# ==============================================================================

# find_package(Doxygen)
#
# if(DOXYGEN_FOUND)
#     set(BUILD_DOXYGEN_DOCS ON CACHE BOOL "Generate HTML docs using Doxygen")
# endif()


# ==============================================================================
#
#  configuration feedback
#
# ==============================================================================

message (STATUS "===========================================================================")
message (STATUS "   RUBYK build configuration settings                                      ")
message (STATUS "===========================================================================")
message (STATUS "   Platform                                                     = ${PLAT}")

# ================= RUBYK_ENABLE_TESTING
message (STATUS "   RUBYK_ENABLE_TESTING  (Build and run tests)                  = ${RUBYK_ENABLE_TESTING}")
if(RUBYK_ENABLE_TESTING)
else(RUBYK_ENABLE_TESTING)
  message (STATUS "")
  message (STATUS "   Type: 'make objects' to build rko objects")
endif(RUBYK_ENABLE_TESTING)

# ================= RUBYK_MEMORY_CHECKING
message (STATUS "   RUBYK_MEMORY_CHECKING (Enable checking against memory leaks) = ${RUBYK_MEMORY_CHECKING}")
if(RUBYK_MEMORY_CHECKING)
message (STATUS "       you should run test_runner with")
message (STATUS "       > export MallocStackLogging=1;./test_runner")
endif(RUBYK_MEMORY_CHECKING)

# ================= RUBYK_USE_LUA_JIT
message (STATUS "   RUBYK_USE_LUA_JIT     (Experimental Lua JIT compiler)        = ${RUBYK_USE_LUA_JIT}")
message (STATUS "===========================================================================")
