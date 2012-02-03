
# To avoid symbol striping on load, we need to load all Qt code at once (?). So we build a single
# module with all code and use some tricks in Lua to load each module.

# We manually load all modules from core.so
add_definitions(-DDUB_LUA_NO_OPEN)

file(GLOB SUB_MODULE_SOURCES *.cpp core/*.cpp core/${PLAT}/*.cpp core/${PLAT}/*.mm)
file(GLOB SUB_MODULE_HEADERS ../include/mimas/*.h)
list(REMOVE_ITEM SUB_MODULE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../include/mimas/Brush.h)
list(REMOVE_ITEM SUB_MODULE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../include/mimas/Color.h)
list(REMOVE_ITEM SUB_MODULE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../include/mimas/constants.h)
list(REMOVE_ITEM SUB_MODULE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../include/mimas/mimas.h)
list(REMOVE_ITEM SUB_MODULE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../include/mimas/Pen.h)
list(REMOVE_ITEM SUB_MODULE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../include/mimas/Painter.h)
list(REMOVE_ITEM SUB_MODULE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../include/mimas/Path.h)
list(REMOVE_ITEM SUB_MODULE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../include/mimas/qt_config.h)
list(REMOVE_ITEM SUB_MODULE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../include/mimas/range_widget.h)

# ==============================================================================
#
#  Qt stuff
#
# ==============================================================================

# Qt header pre-processing
# QT4_WRAP_CPP(SUB_MODULE_HEADERS_MOC ${SUB_MODULE_HEADERS})
# Manual processing in-source so that we can build without Qt4 SDK (just headers and static libs).
if(QT_SRC_PATH STREQUAL "")
  set(MOC "moc")
else(QT_SRC_PATH STREQUAL "")
  set(MOC "${QT_SRC_PATH}/bin/moc")
endif(QT_SRC_PATH STREQUAL "")

set(SUB_MODULE_HEADERS_MOC "")
foreach(MOC_SRC ${SUB_MODULE_HEADERS})
  get_filename_component(MOC_SRC_NAME ${MOC_SRC} NAME_WE)
  add_custom_command( PRE_BUILD
    OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/moc/${MOC_SRC_NAME}.cpp
    COMMAND ${MOC} ${MOC_SRC} -o ${CMAKE_CURRENT_SOURCE_DIR}/moc/${MOC_SRC_NAME}.cpp
    DEPENDS ${MOC_SRC}
  )
  list(APPEND SUB_MODULE_HEADERS_MOC ${CMAKE_CURRENT_SOURCE_DIR}/moc/${MOC_SRC_NAME}.cpp)
endforeach(MOC_SRC SUB_MODULE_HEADERS)

if(QT_SRC_PATH STREQUAL "")
  # build with FindQt4 (non-static build)
  find_package(Qt4 REQUIRED)
  set(QT_USE_QTGUI TRUE)
  set(QT_USE_QTOPENGL TRUE)
  SET(QT_USE_QTOPENGL TRUE)
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
  set(QT_LIBRARIES "-framework Carbon -framework AppKit -framework ApplicationServices ${QT_LIB_PATH}/libQtGui.a ${QT_LIB_PATH}/libQtOpenGL.a ${QT_LIB_PATH}/libQtCore.a /usr/lib/libz.dylib")
endif(QT_SRC_PATH STREQUAL "")

# --=============================================== OpenGL


 
FIND_PACKAGE(OpenGL)
# ==============================================================================

# create name/[SUB_MOD_NAME].so library
build_sub_module("core" ${SUB_MODULE_SOURCES} ${SUB_MODULE_HEADERS_MOC})
target_link_libraries("${MODULE_NAME}_core"
  ${QT_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${LUBYK_SOURCE_DIR}/modules/mimas/vendor/glew/lib/libglew.a
  )
add_dependencies("${MODULE_NAME}_core" glew_make)

# CMake with Qt ref:
# http://developer.qt.nokia.com/quarterly/view/using_cmake_to_build_qt_projects
