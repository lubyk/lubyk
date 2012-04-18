# ----------------------------------------------------------------------------
#  CMake file to build all bindings for projects in 'modules'.
#  See root CMakeLists.txt
#
# ----------------------------------------------------------------------------
project(modules)

# ----------------------------------------------------------------------------------
#  Include common directories
# ----------------------------------------------------------------------------------
include_directories(lk/include lk/include/lua ${CMAKE_CURRENT_SOURCE_DIR})

# ----------------------------------------------------------------------------------
#  Build each module
# ----------------------------------------------------------------------------------

set(MODULES "")
file (GLOB MOD_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*/CMakeLists.txt)
foreach(FILE ${MOD_LIST})
  get_filename_component(PARENT_DIR ${FILE} PATH)
  list(APPEND MODULES ${PARENT_DIR})
endforeach(FILE)

# ==============================================================================
#
#  Create all targets for module (lib, core, sub, vendor)
#
# ==============================================================================

foreach (MODULE ${MODULES})
  add_subdirectory(${MODULE})
endforeach (MODULE)

# ----------------------------------------------------------------------------------
#  base = dub lk lfs mdns test xml yaml zmq
# ----------------------------------------------------------------------------------
add_custom_target(base true)
add_dependencies(base dub lk lfs mdns test xml yaml zmq)
