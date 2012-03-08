#-----------------------------------------------------------------------------
# Include directories for other projects installed on the system.
SET(ZMQ_INCLUDE_DIRS_SYSTEM "")

#-----------------------------------------------------------------------------
# Include directories from the build tree.
SET(ZMQ_INCLUDE_DIRS_BUILD_TREE ${ZMQ_BINARY_DIR})

# These directories are always needed.
SET(ZMQ_INCLUDE_DIRS_BUILD_TREE ${ZMQ_INCLUDE_DIRS_BUILD_TREE}
  ${ZMQ_SOURCE_DIR}/src
  ${ZMQ_SOURCE_DIR}/include
)

#-----------------------------------------------------------------------------
# Include directories needed for .cxx files in ZMQ.  These include
# directories will NOT be available to user projects.
SET(ZMQ_INCLUDE_DIRS_BUILD_TREE_CXX)

#-----------------------------------------------------------------------------
# Include directories from the install tree.
SET(ZMQ_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/ZMQ")
SET(ZMQ_INCLUDE_DIRS_INSTALL_TREE ${ZMQ_INCLUDE_DIRS_INSTALL_TREE}
  ${ZMQ_INSTALL_INCLUDE_DIR}
  ${ZMQ_INSTALL_INCLUDE_DIR}/Source
)


