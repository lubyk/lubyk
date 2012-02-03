PROJECT(qt4_lion_ogl32)
cmake_minimum_required(VERSION 2.8)
 
FIND_PACKAGE(Qt4)
SET( QT_USE_QTOPENGL true )
INCLUDE( ${QT_USE_FILE} )
 
FIND_PACKAGE(OpenGL)
 
 
ADD_EXECUTABLE( test_ogl_core_context
    test_ogl_core_context.cpp
    core_profile_attributes.mm
)
TARGET_LINK_LIBRARIES( test_ogl_core_context
  ${QT_LIBRARIES}
  ${OPENGL_LIBRARIES}
  "-framework Foundation"
  "-framework Cocoa"
)
