cmake_minimum_required(VERSION 2.8)

project(Mimas)

file(GLOB MIMAS_SOURCES   src/*.cpp src/${PLAT}/*.cpp)

if("${PLAT}" STREQUAL "macosx")
  file(GLOB MIMAS_RESOURCES Resources/*)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
  add_executable(Mimas  MACOSX_BUNDLE  ${MIMAS_SOURCES})
  add_custom_target(Mimas_rsrs
    COMMAND ${CMAKE_COMMAND} -E make_directory "${EXECUTABLE_OUTPUT_PATH}/Mimas.app/Contents/MacOS"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${EXECUTABLE_OUTPUT_PATH}/Mimas.app/Contents/Resources"
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/Resources/* ${EXECUTABLE_OUTPUT_PATH}/Mimas.app/Contents/Resources/)
  add_dependencies(Mimas Mimas_rsrs)
endif("${PLAT}" STREQUAL "macosx")

