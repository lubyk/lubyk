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

    if(EXISTS ${MODULE}/sub)

      # ----------------------------------------------------------------------------------
      #  Build each sub-module defined by a single cpp file
      # ----------------------------------------------------------------------------------
      file (GLOB SUB_MODULES_CPP ${MODULE}/sub/*.cpp)
      foreach (SUB_MODULE_CPP ${SUB_MODULES_CPP})
        get_filename_component (SUB_MODULE_NAME ${SUB_MODULE_CPP} NAME_WE)

        # create name/[SUB_MOD_NAME].so library
        add_library("${target}_${SUB_MODULE_NAME}" MODULE ${SUB_MODULE_CPP})
        set_target_properties("${target}_${SUB_MODULE_NAME}"
          PROPERTIES OUTPUT_NAME ${SUB_MODULE_NAME}
          LINK_FLAGS ${PLAT_OBJ_FLAGS}
          LIBRARY_OUTPUT_DIRECTORY ${RUBYK_SOURCE_DIR}/lib/${MODULE_NAME}
          PREFIX ""
          SUFFIX ".so"
        )
        add_dependencies (modules "${target}_${SUB_MODULE_NAME}")
      endforeach (SUB_MODULE_CPP ${SUB_MODULES_CPP})


      # ----------------------------------------------------------------------------------
      #  Build each sub-module defined by a folder
      # ----------------------------------------------------------------------------------
      file(GLOB SUB_MODULES ${MODULE}/sub/*)
      if(NOT ${SUB_MODULES_CPP} STREQUAL "")
        list(REMOVE_ITEM SUB_MODULES ${SUB_MODULES_CPP})
      endif(NOT ${SUB_MODULES_CPP} STREQUAL "")

      foreach (SUB_MODULE ${SUB_MODULES})
        get_filename_component (SUB_MODULE_NAME ${SUB_MODULE} NAME_WE)
        file(GLOB SUB_MODULE_SOURCES ${SUB_MODULE}/*.cpp)

        # create name/[SUB_MOD_NAME].so library
        add_library("${target}_${SUB_MODULE_NAME}" MODULE ${SUB_MODULE_SOURCES})
        set_target_properties("${target}_${SUB_MODULE_NAME}"
          PROPERTIES OUTPUT_NAME ${SUB_MODULE_NAME}
          LINK_FLAGS ${PLAT_OBJ_FLAGS}
          LIBRARY_OUTPUT_DIRECTORY ${RUBYK_SOURCE_DIR}/lib/${MODULE_NAME}
          PREFIX ""
          SUFFIX ".so"
        )
        add_dependencies (modules "${target}_${SUB_MODULE_NAME}")
      endforeach (SUB_MODULE ${SUB_MODULES})
    endif(EXISTS ${MODULE}/sub)

    if(EXISTS ${MODULE}/lua)
      file (GLOB MODULE_SOURCES ${MODULE}/lua/*)

      add_custom_target("${target}_lua" ALL
         COMMAND ${CMAKE_COMMAND} -E copy_directory ${MODULE}/lua ${RUBYK_SOURCE_DIR}/lib)

      add_dependencies(modules "${target}_lua")

    endif(EXISTS ${MODULE}/lua)


  endif(EXISTS ${MODULE}/CMakeLists.txt)
endforeach (MODULE)
