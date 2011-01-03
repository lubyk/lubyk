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
      #  Build each sub-module defined by a single cpp file
      # ----------------------------------------------------------------------------------
      file(GLOB SUB_MODULES ${MODULE}/sub/*)
      file (GLOB SUB_MODULES_CPP ${MODULE}/sub/*.cpp)
      foreach (SUB_MODULE_CPP ${SUB_MODULES_CPP})
        list(REMOVE_ITEM SUB_MODULES "${SUB_MODULE_CPP}")
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
        add_dependencies(${target} "${target}_${SUB_MODULE_NAME}")
      endforeach (SUB_MODULE_CPP ${SUB_MODULES_CPP})


      # ----------------------------------------------------------------------------------
      #  Build each sub-module defined by a folder
      # ----------------------------------------------------------------------------------
      list(REMOVE_ITEM SUB_MODULES ${MODULE}/sub/.DS_Store)

      foreach (SUB_MODULE ${SUB_MODULES})
        if(EXISTS ${SUB_MODULE}/CMakeLists.txt)
          add_subdirectory(${SUB_MODULE})
        else(EXISTS ${SUB_MODULE}/CMakeLists.txt)
          get_filename_component (SUB_MODULE_NAME ${SUB_MODULE} NAME_WE)
          file(GLOB SUB_MODULE_SOURCES ${SUB_MODULE}/*.cpp ${SUB_MODULE}/${PLAT}/*.cpp ${SUB_MODULE}/${PLAT}/*.mm)

          # create name/[SUB_MOD_NAME].so library
          add_library("${target}_${SUB_MODULE_NAME}" MODULE ${SUB_MODULE_SOURCES})
          set_target_properties("${target}_${SUB_MODULE_NAME}"
            PROPERTIES OUTPUT_NAME ${SUB_MODULE_NAME}
            LINK_FLAGS ${PLAT_OBJ_FLAGS}
            LIBRARY_OUTPUT_DIRECTORY ${RUBYK_SOURCE_DIR}/lib/${MODULE_NAME}
            PREFIX ""
            SUFFIX ".so"
          )
          add_dependencies(${target} "${target}_${SUB_MODULE_NAME}")
        endif(EXISTS ${SUB_MODULE}/CMakeLists.txt)
      endforeach (SUB_MODULE ${SUB_MODULES})
    endif(EXISTS ${MODULE}/sub)

    if(EXISTS ${MODULE}/vendor/CMakeLists.txt)
      add_subdirectory(${MODULE}/vendor)
      add_dependencies(${target} "${target}_vendor")
    endif(EXISTS ${MODULE}/vendor/CMakeLists.txt)

    if(EXISTS ${MODULE}/lua)
      file (GLOB MODULE_SOURCES ${MODULE}/lua/*)

      add_custom_target("${target}_lua" ALL
         COMMAND ${CMAKE_COMMAND} -E copy_directory ${MODULE}/lua ${RUBYK_SOURCE_DIR}/lib)

      add_dependencies(${target} "${target}_lua")

    endif(EXISTS ${MODULE}/lua)


  endif(EXISTS ${MODULE}/CMakeLists.txt)
endforeach (MODULE)
