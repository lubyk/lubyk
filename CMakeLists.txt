# ==============================================================================
#
#  Lubyk
#
# ==============================================================================
project (LUBYK)
cmake_minimum_required(VERSION 2.8)
set(LUBYK_VERSION "0.5")
set(LUBYK_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/modules)

# ==============================================================================
#
#  Options
#
# ==============================================================================
set(QT_SRC_PATH "" CACHE STRING    "Use static qt build (provide qt src path)")

# ==============================================================================
#
#  Common build settings for all modules
#
# ==============================================================================
if(NOT LUBYK)
  include(CMakeLubyk.txt)
endif(NOT LUBYK)

# ==============================================================================
#
#  Build all modules in 'modules'
#
# ==============================================================================
#add_custom_target(modules ALL)
add_custom_target(modules)
add_subdirectory(modules)

# ==============================================================================
#
#  Build lua binary (not mandatory: used to have debug info)
#
# ==============================================================================
# we do not shipt with a lua binary yet
#add_subdirectory(lua)

# ==============================================================================
#
#  Build Mac OS X dmg
#
# ==============================================================================
add_custom_target(dmg
  COMMAND hdiutil create -srcfolder ${LUBYK_SOURCE_DIR}/deploy/macosx/Lubyk_dmg -format UDZO -volname 'Install Lubyk' ${LUBYK_SOURCE_DIR}/deploy/macosx/Lubyk.dmg
  )

# ==============================================================================
#
#  Test all
#
# ==============================================================================
add_custom_target(test_all cd .. && ./ltest)
add_dependencies(test_all modules)


message (STATUS "===========================================================================")
message (STATUS "   LUBYK build configuration settings                                      ")
message (STATUS "===========================================================================")
message (STATUS "   Target folder     = ${LUBYK_LIB}")
message (STATUS "   Platform          = ${PLAT}")
message (STATUS "   Release           = ${RELEASE}")
if(QT_SRC_PATH STREQUAL "")
message (STATUS "   QT_SRC_PATH       (build using Qt frameworks)")
else(QT_SRC_PATH STREQUAL "")
message (STATUS "   QT_SRC_PATH       = ${QT_SRC_PATH}")
endif(QT_SRC_PATH STREQUAL "")
message (STATUS "   Type: 'ccmake <path to lubyk>' to change settings")
message (STATUS "===========================================================================")
