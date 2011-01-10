# ----------------------------------------------------------------------------
#  CMake file to build all bindings for projects in 'modules'.
#  See root CMakeLists.txt
#
# ----------------------------------------------------------------------------
project(modules)

# ----------------------------------------------------------------------------------
#  Include common directories
# ----------------------------------------------------------------------------------
include_directories(rubyk/include lua/include ${CMAKE_CURRENT_SOURCE_DIR})

# ----------------------------------------------------------------------------------
#  Build each module
# ----------------------------------------------------------------------------------

file (GLOB MODULES ${CMAKE_CURRENT_SOURCE_DIR}/*)
list(REMOVE_ITEM MODULES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)
list(REMOVE_ITEM MODULES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeSub.txt)
list(REMOVE_ITEM MODULES ${CMAKE_CURRENT_SOURCE_DIR}/bind.rb)


# ==============================================================================
#
#  Update bindings with 'dub'
#
# ==============================================================================


# Build files from dub
add_custom_target(dub ALL
   COMMAND ruby ${RUBYK_SOURCE_DIR}/modules/bind.rb
   DEPENDS ${DOXYGEN_OUTPUT})

add_dependencies(modules dub)


# ==============================================================================
#
#  Create all targets for module (lua, core, sub, vendor)
#
# ==============================================================================


foreach (MODULE ${MODULES})
  if(EXISTS ${MODULE}/CMakeLists.txt)
    add_subdirectory(${MODULE})
  else(EXISTS ${MODULE}/CMakeLists.txt)
    # Simple build
    # get name
    get_filename_component (MODULE_NAME ${MODULE} NAME_WE)
    set(target "${MODULE_NAME}")
    set(MODULE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_NAME}")

    if(EXISTS ${MODULE}/include)
      include_directories(${MODULE}/include)
    endif(EXISTS ${MODULE}/include)

    if(EXISTS ${MODULE}/src)
      file (GLOB MODULE_SOURCES ${MODULE}/src/*.cpp ${MODULE}/src/${PLAT}/*.cpp)

      # create name.so library
      add_library(${target} MODULE ${MODULE_SOURCES})
      set_target_properties(${target}
        PROPERTIES OUTPUT_NAME ${target}
        LINK_FLAGS ${PLAT_OBJ_FLAGS}
        LIBRARY_OUTPUT_DIRECTORY ${RUBYK_SOURCE_DIR}/lib
        PREFIX ""
        SUFFIX ".so"
      )
      add_dependencies(modules ${target})
    else(EXISTS ${MODULE}/src)
      add_custom_target(${target} true) # noop
      add_dependencies(modules "${target}")
    endif(EXISTS ${MODULE}/src)

    if(EXISTS ${MODULE}/sub)
      # ----------------------------------------------------------------------------------
      #  sub: module_xxx, module_yyy, ...
      # ----------------------------------------------------------------------------------
      if(EXISTS ${MODULE}/sub/CMakeLists.txt)
        add_subdirectory(${MODULE}/sub)
      else(EXISTS ${MODULE}/sub/CMakeLists.txt)
        include(${RUBYK_SOURCE_DIR}/modules/CMakeSub.txt)
      endif(EXISTS ${MODULE}/sub/CMakeLists.txt)
    endif(EXISTS ${MODULE}/sub)

    # ----------------------------------------------------------------------------------
    #  module_vendor
    # ----------------------------------------------------------------------------------
    if(EXISTS ${MODULE}/vendor/CMakeLists.txt)
      add_subdirectory(${MODULE}/vendor)
      add_dependencies(${target} "${target}_vendor")
    endif(EXISTS ${MODULE}/vendor/CMakeLists.txt)

    # ----------------------------------------------------------------------------------
    #  module_lua
    # ----------------------------------------------------------------------------------
    if(EXISTS ${MODULE}/lua)
      file (GLOB MODULE_SOURCES ${MODULE}/lua/*)

      add_custom_target("${target}_lua" ALL
         COMMAND ${CMAKE_COMMAND} -E copy_directory ${MODULE}/lua ${RUBYK_SOURCE_DIR}/lib)

      add_dependencies(${target} "${target}_lua")

    endif(EXISTS ${MODULE}/lua)


  endif(EXISTS ${MODULE}/CMakeLists.txt)
endforeach (MODULE)
