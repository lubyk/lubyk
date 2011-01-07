set(SUB_MODULE_NAME "core")


file(GLOB SUB_MODULE_SOURCES *.cpp ${PLAT}/*.cpp)
file(GLOB SUB_MODULE_HEADERS ../../include/mimas/*.h)
list(REMOVE_ITEM SUB_MODULE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../../include/mimas/mimas.h)
list(REMOVE_ITEM SUB_MODULE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../../include/mimas/range_widget.h)

# ==============================================================================
#
#  Qt stuff
#
# ==============================================================================

#find_package(Qt4 REQUIRED)
find_package(Qt4)

# CMAKE_CURRENT_BINARY_DIR (generated files are stored here)
include_directories(include ${CMAKE_CURRENT_BINARY_DIR})

# Qt header pre-processing
QT4_WRAP_CPP(SUB_MODULE_HEADERS_MOC ${SUB_MODULE_HEADERS})

set(QT_SRC "/Users/gaspard/git/qt")

if(QT_SRC)
  message("STATIC...")
  set(QT_INCLUDE_PATH ${QT_SRC}/include)
  set(QT_LIB_PATH ${QT_SRC}/lib)
  include_directories(${QT_INCLUDE_PATH})
  message(">>>${SUB_MODULE_SOURCES}")
  set(PLAT_OBJ_FLAGS "${PLAT_OBJ_FLAGS} -framework Carbon -framework AppKit -framework ApplicationServices ${QT_LIB_PATH}/libQtGui.a ${QT_LIB_PATH}/libQtCore.a /usr/lib/libz.dylib")
else(QT_SRC)
  include(${QT_USE_FILE})
  add_definitions(${QT_DEFINITIONS})
endif(QT_SRC)

add_definitions(-DDUB_LUA_NO_OPEN)
# ==============================================================================

message("FLAGS: ${PLAT_OBJ_FLAGS}")
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
