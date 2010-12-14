# ----------------------------------------------------------------------------
#  CMake file to build all bindings for projects in 'modules'.
#  See root CMakeLists.txt
#
# ----------------------------------------------------------------------------
project(modules)

# ----------------------------------------------------------------------------------
#  Include common directories
# ----------------------------------------------------------------------------------
include_directories(rubyk/include lua/include rubyk/include)

# ----------------------------------------------------------------------------------
#  Build each module
# ----------------------------------------------------------------------------------

file (GLOB MODULES ${CMAKE_CURRENT_SOURCE_DIR}/*)
list(REMOVE_ITEM MODULES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)

# set(CMAKE_CXX_FLAGS "-fno-common")
#
# g++ -O2 -fno-common -c -o test.o -I../../include/rubyk/lua MODULE.cpp
# gcc -bundle -undefined dynamic_lookup -o test.so test.o -lstdc++

# replace luaopen_xx by luaload_xx
# add_definitions(-DDUB_LUA_NO_OPEN)

foreach (MODULE ${MODULES})
  if(EXISTS ${MODULE}/CMakeLists.txt)
    add_subdirectory(${MODULE})
  else(EXISTS ${MODULE}/CMakeLists.txt)
    # Simple build
    # get name
    get_filename_component (MODULE_NAME ${MODULE} NAME_WE)
    set(target "${MODULE_NAME}")

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
      add_dependencies (modules ${target})
    endif(EXISTS ${MODULE}/src)

    if(EXISTS ${MODULE}/core/include)
      include_directories(${MODULE}/rubyk/include)
    endif(EXISTS ${MODULE}/core/include)

    if(EXISTS ${MODULE}/core/src)
      file (GLOB MODULE_SOURCES ${MODULE}/core/src/*.cpp ${MODULE}/core/src/${PLAT}/*.cpp)

      # create name/core.so library
      add_library("${target}_core" MODULE ${MODULE_SOURCES})
      set_target_properties("${target}_core"
        PROPERTIES OUTPUT_NAME core
        LINK_FLAGS ${PLAT_OBJ_FLAGS}
        LIBRARY_OUTPUT_DIRECTORY ${RUBYK_SOURCE_DIR}/lib/${MODULE_NAME}
        PREFIX ""
        SUFFIX ".so"
      )
      add_dependencies (modules "${target}_core")
    endif(EXISTS ${MODULE}/core/src)

    if(EXISTS ${MODULE}/lua)
      file (GLOB MODULE_SOURCES ${MODULE}/lua/*)

      add_custom_target("${target}_lua" ALL
         COMMAND ${CMAKE_COMMAND} -E copy_directory ${MODULE}/lua ${RUBYK_SOURCE_DIR}/lib)

      add_dependencies(modules "${target}_lua")

    endif(EXISTS ${MODULE}/lua)


  endif(EXISTS ${MODULE}/CMakeLists.txt)
endforeach (MODULE)
