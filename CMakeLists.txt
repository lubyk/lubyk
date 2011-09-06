cmake_minimum_required(VERSION 2.6.2)

# ==============================================================================
#
#  Lubyk
#
# ==============================================================================

project (LUBYK)
set(LUBYK_VERSION "0.5")

# ==============================================================================
#
#  Options
#
# ==============================================================================
set(QT_SRC_PATH "" CACHE STRING    "Use static qt build (provide qt src path)")

# ==============================================================================
#
#  Platform guessing
#
# ==============================================================================
if(UNIX)
  add_definitions(-DLUA_USE_LINUX)
  if(APPLE)
    set(PLAT "macosx")
    set(LUBYK_LIB ${LUBYK_SOURCE_DIR}/deploy/macosx/Lubyk_dmg/Lubyk.app/Contents/Resources/lib)
    # add_definitions(-DLUA_USE_MACOSX)
    add_definitions(-D__MACOSX_CORE__)
    # CoreFoundation needed by Midi and cannot be added in dylib
    set(PLAT_LINK "-all_load -flat_namespace -lreadline -framework Accelerate -framework CoreFoundation -framework OpenGL -framework GLUT -framework Cocoa -lobjc") # -framework Cocoa -lobjc ===> planet_gui.mm
    set(PLAT_OBJ_FLAGS "-flat_namespace -undefined suppress -dynamic -bundle -fPIC")

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
    set(PLAT_OBJ_FLAGS "${PLAT_LINK} -dynamic -fPIC")
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
#  Doxygen
#
# ==============================================================================
find_package(Doxygen)
set(DOXYGEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/doc)
set(DOXYGEN_OUTPUT ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)
set(LUBYK_BUILD_DOXYGEN_DOCS ON CACHE BOOL "Build HTML+XML documentation with Doxygen")

# The code below is moved here so that 'dub' target (Doxygen based binding generator)
# can depend on this action.
if(LUBYK_BUILD_DOXYGEN_DOCS AND DOXYGEN_FOUND)
  file (GLOB MODULES ${LUBYK_SOURCE_DIR}/modules/*)
  foreach(MODULE ${MODULES})
    get_filename_component (MODULE_NAME ${MODULE} NAME_WE)
    file(GLOB MODULE_DUB_HEADERS ${MODULE}/include/${MODULE_NAME}/*.h)
    list(APPEND DOXYGEN_INPUT_LIST_B ${MODULE_DUB_HEADERS})
    # Dummy headers used to created automatic bindings to super-classes
    file(GLOB MODULE_DUB_HEADERS ${MODULE}/bind/*.h)
    list(APPEND DOXYGEN_INPUT_LIST_B ${MODULE_DUB_HEADERS})
  endforeach(MODULE ${MODULES})

  string(REGEX REPLACE ";" " " DOXYGEN_INPUT_LIST "${DOXYGEN_INPUT_LIST_B}")

  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile)

  add_custom_command(
    OUTPUT ${DOXYGEN_OUTPUT}
    COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile >/dev/null
    COMMAND ${CMAKE_COMMAND} -E echo "Done."
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${DOXYGEN_INPUT} ${CMAKE_BINARY_DIR}/Doxyfile
    )
  add_custom_target(doc ALL DEPENDS ${DOXYGEN_OUTPUT})
endif()


# ==============================================================================
#
#  Build all modules in 'modules'
#
# ==============================================================================
add_custom_target(modules ALL)
add_subdirectory(modules)

# ==============================================================================
#
#  Build lua binary (not mandatory: used to have debug info)
#
# ==============================================================================
# we do not shipt with a lua binary yet
#add_subdirectory(lua)

# ==============================================================================
#
#  Test all
#
# ==============================================================================
add_custom_target(test_all cd .. && ./ltest)
add_dependencies(test_all modules)

# ==============================================================================
#
#  Rebuild documentation on header changes (call make dub to rebuild bindings)
#
# ==============================================================================
add_custom_target(head rm -rf ${CMAKE_BINARY_DIR}/doc/xml && make doc)


message (STATUS "===========================================================================")
message (STATUS "   LUBYK build configuration settings                                      ")
message (STATUS "===========================================================================")
message (STATUS "   Platform                                                       = ${PLAT}")
message (STATUS "   Release                                                        = ${RELEASE}")
if(QT_SRC_PATH STREQUAL "")
message (STATUS "   QT_SRC_PATH              (build using Qt frameworks)           = ${QT_SRC_PATH}")
else(QT_SRC_PATH STREQUAL "")
message (STATUS "   QT_SRC_PATH                                                    = ${QT_SRC_PATH}")
endif(QT_SRC_PATH STREQUAL "")
message (STATUS "   Type: 'ccmake <path to lubyk>' to change settings")
message (STATUS "===========================================================================")
