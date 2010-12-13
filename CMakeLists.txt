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

# ==============================================================================
#
#  Build all modules in 'modules'
#
# ==============================================================================
add_custom_target(modules)
add_subdirectory(modules)


# ==============================================================================
#
#  Test all
#
# ==============================================================================
add_custom_target(test_all ALL cd .. && ./rtest)
add_dependencies (test_all modules)

