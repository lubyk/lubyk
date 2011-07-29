# ----------------------------------------------------------------------------
#  CMake file to build all bindings for projects in 'modules'.
#  See root CMakeLists.txt
#
# ----------------------------------------------------------------------------
project(modules)

# ----------------------------------------------------------------------------------
#  Include common directories
# ----------------------------------------------------------------------------------
include_directories(lubyk/include lubyk/include/lua ${CMAKE_CURRENT_SOURCE_DIR})

# ----------------------------------------------------------------------------------
#  Build each module
# ----------------------------------------------------------------------------------

file (GLOB MODULES ${CMAKE_CURRENT_SOURCE_DIR}/*)
list(REMOVE_ITEM MODULES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)
list(REMOVE_ITEM MODULES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeSub.txt)
list(REMOVE_ITEM MODULES ${CMAKE_CURRENT_SOURCE_DIR}/bind.rb)
list(REMOVE_ITEM MODULES ${CMAKE_CURRENT_SOURCE_DIR}/.DS_Store)


# ==============================================================================
#
#  Update bindings with 'dub'
#
# ==============================================================================


# Build files from dub
add_custom_target(dub ALL
   COMMAND ruby ${LUBYK_SOURCE_DIR}/modules/bind.rb
   DEPENDS ${DOXYGEN_OUTPUT})

add_dependencies(modules dub)


# ==============================================================================
#
#  Macro to create a single module.
#  This macro expects the following variables to be set:
#  MODULE_NAME: main module name ('video')
#  MODULE: main module path ('.../modules/video')
#  PLAT: current platform (macosx, linux, windows)
#  PLAT_OBJ_FLAGS: cflags for the given platform
#
#  The only parameter is the sub module's name (MODULE/SUB_MODULE_NAME must be a folder)
#
# ==============================================================================


macro(build_sub_module SUB_MODULE_NAME)
  # create lib/name directory
  file(MAKE_DIRECTORY ${LUBYK_SOURCE_DIR}/lib/${MODULE_NAME})

  # create name/[SUB_MOD_NAME].so library
  add_library("${MODULE_NAME}_${SUB_MODULE_NAME}" MODULE ${ARGN}) # ARGN = list of arguments passed after SUB_MODULE_NAME
  set_target_properties("${MODULE_NAME}_${SUB_MODULE_NAME}"
    PROPERTIES OUTPUT_NAME ${SUB_MODULE_NAME}
    LINK_FLAGS ${PLAT_OBJ_FLAGS}
    LIBRARY_OUTPUT_DIRECTORY ${LUBYK_SOURCE_DIR}/lib/${MODULE_NAME}
    PREFIX ""
    SUFFIX ".so"
  )
  add_dependencies(${MODULE_NAME} "${MODULE_NAME}_${SUB_MODULE_NAME}")
endmacro(build_sub_module)

# Use this if SUB_MODULE path is defined and you can use the default file sources and filename
macro(build_sub_module_with_defaults)
  get_filename_component(SUB_MODULE_NAME ${SUB_MODULE} NAME_WE)
  file(GLOB SUB_MODULE_SOURCES ${SUB_MODULE}/*.cpp ${SUB_MODULE}/${PLAT}/*.cpp ${SUB_MODULE}/${PLAT}/*.mm)
  build_sub_module(${SUB_MODULE_NAME} ${SUB_MODULE_SOURCES})
endmacro(build_sub_module_with_defaults)


# ==============================================================================
#
#  Macro to create a single module.
#
#  This macro expects the following variables to be set:
#  MODULES: modules path ('.../modules')
#  PLAT: current platform (macosx, linux, windows)
#  PLAT_OBJ_FLAGS: cflags for the given platform
#
#  The only parameter is the module's name (MODULES/MODULE_NAME must be a folder)
#
# ==============================================================================
macro(build_module MODULE_NAME)
  # make lib directory
  file(MAKE_DIRECTORY ${LUBYK_SOURCE_DIR}/lib)

  # create name.so library
  add_library(${MODULE_NAME} MODULE ${ARGN})
  set_target_properties(${MODULE_NAME}
    PROPERTIES OUTPUT_NAME ${MODULE_NAME}
    LINK_FLAGS ${PLAT_OBJ_FLAGS}
    LIBRARY_OUTPUT_DIRECTORY ${LUBYK_SOURCE_DIR}/lib
    PREFIX ""
    SUFFIX ".so"
  )
  add_dependencies(modules ${MODULE_NAME})
endmacro(build_module)

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
    set(MODULE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_NAME}")

    if(EXISTS ${MODULE}/include)
      include_directories(${MODULE}/include)
    endif(EXISTS ${MODULE}/include)

    if(EXISTS ${MODULE}/src)
      file (GLOB MODULE_SOURCES ${MODULE}/src/*.cpp ${MODULE}/src/${PLAT}/*.cpp ${MODULE}/src/*.c ${MODULE}/src/${PLAT}/*.c ${MODULE}/src/*.mm ${MODULE}/src/${PLAT}/*.mm)
      build_module(${MODULE_NAME} ${MODULE_SOURCES})
    else(EXISTS ${MODULE}/src)
      add_custom_target(${MODULE_NAME} true) # noop
      add_dependencies(modules "${MODULE_NAME}")
    endif(EXISTS ${MODULE}/src)

    if(EXISTS ${MODULE}/sub)
      # ----------------------------------------------------------------------------------
      #  sub: module_xxx, module_yyy, ...
      # ----------------------------------------------------------------------------------
      if(EXISTS ${MODULE}/sub/CMakeLists.txt)
        add_subdirectory(${MODULE}/sub)
      else(EXISTS ${MODULE}/sub/CMakeLists.txt)
        include(${LUBYK_SOURCE_DIR}/modules/CMakeSub.txt)
      endif(EXISTS ${MODULE}/sub/CMakeLists.txt)
    endif(EXISTS ${MODULE}/sub)

    # ----------------------------------------------------------------------------------
    #  module_vendor
    # ----------------------------------------------------------------------------------
    if(EXISTS ${MODULE}/vendor/CMakeLists.txt)
      add_subdirectory(${MODULE}/vendor)
      add_dependencies(${MODULE_NAME} "${MODULE_NAME}_vendor")
    endif(EXISTS ${MODULE}/vendor/CMakeLists.txt)

    # ----------------------------------------------------------------------------------
    #  module_bind
    # ----------------------------------------------------------------------------------
    file(GLOB MODULE_HEADERS ${MODULE}/include/${MODULE}/*.h)
    add_custom_target("${MODULE_NAME}_bind"
       COMMAND ruby ${LUBYK_SOURCE_DIR}/modules/bind.rb ${MODULE_NAME}
       DEPENDS ${MODULE_HEADERS})

    # run manually
    #add_dependencies(${MODULE_NAME} "${MODULE_NAME}_bind")


    # ----------------------------------------------------------------------------------
    #  module_lua
    # ----------------------------------------------------------------------------------
    if(EXISTS ${MODULE}/lua)
      file (GLOB MODULE_SOURCES ${MODULE}/lua/*)

      add_custom_target("${MODULE_NAME}_lua" ALL
         COMMAND ${CMAKE_COMMAND} -E copy_directory ${MODULE}/lua ${LUBYK_SOURCE_DIR}/lib)

      add_dependencies(${MODULE_NAME} "${MODULE_NAME}_lua")

    endif(EXISTS ${MODULE}/lua)


  endif(EXISTS ${MODULE}/CMakeLists.txt)
endforeach (MODULE)
