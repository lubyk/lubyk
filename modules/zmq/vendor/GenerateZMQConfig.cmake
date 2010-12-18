# Generate the ZMQConfig.cmake file in the build tree.  Also configure
# one for installation.  The file tells external projects how to use
# ZMQ.

#-----------------------------------------------------------------------------
# Settings specific to the build tree.

# Generate CMake lines that will define the ZMQ_SOURCE_DIR in the ZMQConfig.cmake.
# We want this to happen only in the ZMQConfig.cmake of the build dir, not in the
# installed or relocatable one.
SET(ZMQ_CONFIG_CODE "
# The ZMQ source tree.
# For backward compatibility issues we still need to define this variable, although
# it is highly probable that it will cause more harm than being useful. 
# Use ZMQ_INCLUDE_DIRS instead, since ZMQ_SOURCE_DIR may point to non-existent directory
IF(NOT ZMQ_LEGACY_REMOVE)
  SET(ZMQ_SOURCE_DIR \"${ZMQ_SOURCE_DIR}\")
ENDIF(NOT ZMQ_LEGACY_REMOVE)"
)

# The "use" file.
SET(ZMQ_USE_FILE ${ZMQ_BINARY_DIR}/UseZMQ.cmake)

# The build settings file.
SET(ZMQ_BUILD_SETTINGS_FILE ${ZMQ_BINARY_DIR}/ZMQBuildSettings.cmake)

# Library directory.
SET(ZMQ_LIBRARY_DIRS_CONFIG ${ZMQ_LIBRARY_PATH})

# Determine the include directories needed.
SET(ZMQ_INCLUDE_DIRS_CONFIG
  ${ZMQ_INCLUDE_DIRS_BUILD_TREE}
  ${ZMQ_INCLUDE_DIRS_SOURCE_TREE}
  ${ZMQ_INCLUDE_DIRS_SYSTEM}
)

#-----------------------------------------------------------------------------
# Configure ZMQConfig.cmake for the build tree.
CONFIGURE_FILE(${ZMQ_SOURCE_DIR}/ZMQConfig.cmake.in
               ${ZMQ_BINARY_DIR}/ZMQConfig.cmake @ONLY IMMEDIATE)

#-----------------------------------------------------------------------------
# Settings specific to the install tree.

# The library dependencies file.
SET(ZMQ_LIBRARY_DEPENDS_FILE "\${ZMQ_INSTALL_PREFIX}${ZMQ_INSTALL_PACKAGE_DIR}/ZMQLibraryDepends.cmake")

# The "use" file.
SET(ZMQ_USE_FILE \${ZMQ_INSTALL_PREFIX}${ZMQ_INSTALL_PACKAGE_DIR}/UseZMQ.cmake)

# The build settings file.
SET(ZMQ_BUILD_SETTINGS_FILE \${ZMQ_INSTALL_PREFIX}${ZMQ_INSTALL_PACKAGE_DIR}/ZMQBuildSettings.cmake)

# Include directories.
SET(ZMQ_INCLUDE_DIRS_CONFIG \${ZMQ_INSTALL_PREFIX}${ZMQ_INSTALL_INCLUDE_DIR})
FOREACH(DIR ${ZMQ_INCLUDE_RELATIVE_DIRS})
  LIST(APPEND ZMQ_INCLUDE_DIRS_CONFIG \${ZMQ_INSTALL_PREFIX}${ZMQ_INSTALL_INCLUDE_DIR}/${DIR})
ENDFOREACH(DIR)
IF(ZMQ_INCLUDE_DIRS_SYSTEM)
  LIST(APPEND ZMQ_INCLUDE_DIRS_CONFIG ${ZMQ_INCLUDE_DIRS_SYSTEM})
ENDIF(ZMQ_INCLUDE_DIRS_SYSTEM)

# Link directories.
SET(ZMQ_LIBRARY_DIRS_CONFIG "\${ZMQ_INSTALL_PREFIX}${ZMQ_INSTALL_LIB_DIR}")

#-----------------------------------------------------------------------------
# Configure ZMQConfig.cmake for the install tree.

# Construct the proper number of GET_FILENAME_COMPONENT(... PATH)
# calls to compute the installation prefix.
STRING(REGEX REPLACE "/" ";" ZMQ_INSTALL_PACKAGE_DIR_COUNT
  "${ZMQ_INSTALL_PACKAGE_DIR}")
SET(ZMQ_CONFIG_CODE "
# Compute the installation prefix from this ZMQConfig.cmake file location.
GET_FILENAME_COMPONENT(ZMQ_INSTALL_PREFIX \"\${CMAKE_CURRENT_LIST_FILE}\" PATH)")
FOREACH(p ${ZMQ_INSTALL_PACKAGE_DIR_COUNT})
  SET(ZMQ_CONFIG_CODE
    "${ZMQ_CONFIG_CODE}\nGET_FILENAME_COMPONENT(ZMQ_INSTALL_PREFIX \"\${ZMQ_INSTALL_PREFIX}\" PATH)"
    )
ENDFOREACH(p)


CONFIGURE_FILE(${ZMQ_SOURCE_DIR}/ZMQConfig.cmake.in
               ${ZMQ_BINARY_DIR}/Utilities/ZMQConfig.cmake @ONLY IMMEDIATE)

