
# To avoid symbol striping on load, we need to load all Qt code at once (?). So we build a single
# module with all code and use some tricks in Lua to load each module.
set(SUB_MODULE_NAME "core")

# We manually load all modules from core.so
add_definitions(-DDUB_LUA_NO_OPEN)

file(GLOB SUB_MODULE_SOURCES *.cpp core/*.cpp core/${PLAT}/*.cpp)
file(GLOB SUB_MODULE_HEADERS ../include/mimas/*.h)
list(REMOVE_ITEM SUB_MODULE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../include/mimas/mimas.h)
list(REMOVE_ITEM SUB_MODULE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../include/mimas/range_widget.h)

# ==============================================================================
#
#  Qt stuff
#
# ==============================================================================

# Qt header pre-processing
# QT4_WRAP_CPP(SUB_MODULE_HEADERS_MOC ${SUB_MODULE_HEADERS})
# Manual processing in-source so that we can build without Qt4 SDK (just headers and static libs).
set(MOC "moc")
set(SUB_MODULE_HEADERS_MOC "")
foreach(MOC_SRC ${SUB_MODULE_HEADERS})
  get_filename_component(MOC_SRC_NAME ${MOC_SRC} NAME_WE)
  add_custom_command( PRE_BUILD
    OUTPUT  ${RUBYK_SOURCE_DIR}/modules/mimas/sub/moc/${MOC_SRC_NAME}.cpp
    COMMAND ${MOC} ${MOC_SRC} -o ${RUBYK_SOURCE_DIR}/modules/mimas/sub/moc/${MOC_SRC_NAME}.cpp
    DEPENDS ${MOC_SRC}
  )
  list(APPEND SUB_MODULE_HEADERS_MOC ${RUBYK_SOURCE_DIR}/modules/mimas/sub/moc/${MOC_SRC_NAME}.cpp)
endforeach(MOC_SRC SUB_MODULE_HEADERS)

if(QT_SRC_PATH STREQUAL "")
  # build with FindQt4 (non-static build)
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
  add_definitions(${QT_DEFINITIONS})
else(QT_SRC_PATH STREQUAL "")
  if(EXISTS ${QT_SRC_PATH})
  else(EXISTS ${QT_SRC_PATH})
    message("########## Invalid QT_SRC_PATH '${QT_SRC_PATH}'")
  endif(EXISTS ${QT_SRC_PATH})
  set(QT_INCLUDE_PATH ${QT_SRC_PATH}/include)
  set(QT_LIB_PATH ${QT_SRC_PATH}/lib)
  include_directories(${QT_INCLUDE_PATH})
  set(PLAT_OBJ_FLAGS "${PLAT_OBJ_FLAGS} -framework Carbon -framework AppKit -framework ApplicationServices ${QT_LIB_PATH}/libQtGui.a ${QT_LIB_PATH}/libQtCore.a /usr/lib/libz.dylib")
endif(QT_SRC_PATH STREQUAL "")

# ==============================================================================

# create name/[SUB_MOD_NAME].so library
add_library("${target}_${SUB_MODULE_NAME}" MODULE ${SUB_MODULE_SOURCES} ${SUB_MODULE_HEADERS_MOC})
set_target_properties("${target}_${SUB_MODULE_NAME}"
  PROPERTIES OUTPUT_NAME ${SUB_MODULE_NAME}
  LINK_FLAGS ${PLAT_OBJ_FLAGS}
  LIBRARY_OUTPUT_DIRECTORY ${RUBYK_SOURCE_DIR}/lib/${MODULE_NAME}
  PREFIX ""
  SUFFIX ".so"
)
target_link_libraries("${target}_${SUB_MODULE_NAME}" ${QT_LIBRARIES})
add_dependencies(${target} "${target}_${SUB_MODULE_NAME}")


# CMake with Qt ref:
# http://developer.qt.nokia.com/quarterly/view/using_cmake_to_build_qt_projects
