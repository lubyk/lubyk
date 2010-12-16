cmake_minimum_required(VERSION 2.8)

project(mimas)
find_package(Qt4 REQUIRED)

file(GLOB MIMAS_SOURCES src/*.cpp)
file(GLOB MIMAS_HEADERS include/*.h)
file(GLOB MIMAS_FORMS ui/*.ui)

# CMAKE_CURRENT_BINARY_DIR (generated files are stored here)
include_directories(include ${CMAKE_CURRENT_BINARY_DIR})

# Header pre-processing
QT4_WRAP_CPP(MIMAS_HEADERS_MOC ${MIMAS_HEADERS})
QT4_WRAP_UI(MIMAS_FORMS_HEADERS ${MIMAS_FORMS})


include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

add_executable(mimas
  ${MIMAS_SOURCES}
  ${MIMAS_HEADERS_MOC}
  ${MIMAS_FORMS_HEADERS})

target_link_libraries(mimas ${QT_LIBRARIES})

# CMake with Qt ref:
# http://developer.qt.nokia.com/quarterly/view/using_cmake_to_build_qt_projects