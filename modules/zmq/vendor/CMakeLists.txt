##############################################################
# This CMakeLists.txt supports Windows/MSVC, Linux, and OSX
# http://github.com/pieper/CTK
##############################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

PROJECT(ZMQ)

SET(PACKAGE "ZMQ")
SET(PACKAGE_NAME "zeroMQ")
SET(PACKAGE_VERSION "2.0.6")

SET(PACKAGE_VERSION_MAJOR 2)
SET(PACKAGE_VERSION_MINOR 0)
SET(PACKAGE_VERSION_PATCH 6)

# Build with shared libraries

OPTION(BUILD_SHARED_LIBS "Build with shared libraries" OFF)

# Build "./devices"
OPTION(ZMQ_BUILD_DEVICES "Build forwarder, queue, and streamer" OFF)

# Build "./perf"
OPTION(ZMQ_BUILD_PERFORMANCE_TESTS "Build performance test programs" OFF)

SET(SRC_DIR ${ZMQ_SOURCE_DIR}/src)
SET(INCLUDE_DIR ${ZMQ_SOURCE_DIR}/include)

SET(ZMQ_SRC
  ${SRC_DIR}/app_thread.cpp
  ${SRC_DIR}/app_thread.hpp
  ${SRC_DIR}/atomic_counter.hpp
  ${SRC_DIR}/atomic_ptr.hpp
  ${SRC_DIR}/blob.hpp
  ${SRC_DIR}/command.cpp
  ${SRC_DIR}/command.hpp
  ${SRC_DIR}/config.hpp
  ${SRC_DIR}/ctx.cpp
  ${SRC_DIR}/ctx.hpp
  ${SRC_DIR}/decoder.hpp
  ${SRC_DIR}/devpoll.cpp
  ${SRC_DIR}/devpoll.hpp
  ${SRC_DIR}/downstream.cpp
  ${SRC_DIR}/downstream.hpp
  ${SRC_DIR}/encoder.hpp
  ${SRC_DIR}/epoll.cpp
  ${SRC_DIR}/epoll.hpp
  ${SRC_DIR}/err.cpp
  ${SRC_DIR}/err.hpp
  ${SRC_DIR}/fd.hpp
  ${SRC_DIR}/forwarder.cpp
  ${SRC_DIR}/forwarder.hpp
  ${SRC_DIR}/fq.cpp
  ${SRC_DIR}/fq.hpp
  ${SRC_DIR}/i_endpoint.hpp
  ${SRC_DIR}/i_engine.hpp
  ${SRC_DIR}/i_inout.hpp
  ${SRC_DIR}/i_poll_events.hpp
  ${SRC_DIR}/io_object.cpp
  ${SRC_DIR}/io_object.hpp
  ${SRC_DIR}/io_thread.cpp
  ${SRC_DIR}/io_thread.hpp
  ${SRC_DIR}/ip.cpp
  ${SRC_DIR}/ip.hpp
  ${SRC_DIR}/kqueue.cpp
  ${SRC_DIR}/kqueue.hpp
  ${SRC_DIR}/lb.cpp
  ${SRC_DIR}/lb.hpp
  ${SRC_DIR}/likely.hpp
  ${SRC_DIR}/msg_content.hpp
  ${SRC_DIR}/mutex.hpp
  ${SRC_DIR}/object.cpp
  ${SRC_DIR}/object.hpp
  ${SRC_DIR}/options.cpp
  ${SRC_DIR}/options.hpp
  ${SRC_DIR}/owned.cpp
  ${SRC_DIR}/owned.hpp
  ${SRC_DIR}/pair.cpp
  ${SRC_DIR}/pair.hpp
  ${SRC_DIR}/pgm_receiver.cpp
  ${SRC_DIR}/pgm_receiver.hpp
  ${SRC_DIR}/pgm_sender.cpp
  ${SRC_DIR}/pgm_sender.hpp
  ${SRC_DIR}/pgm_socket.cpp
  ${SRC_DIR}/pgm_socket.hpp
  ${SRC_DIR}/pipe.cpp
  ${SRC_DIR}/pipe.hpp
  ${SRC_DIR}/poll.cpp
  ${SRC_DIR}/poll.hpp
  ${SRC_DIR}/poller.hpp
  ${SRC_DIR}/prefix_tree.cpp
  ${SRC_DIR}/prefix_tree.hpp
  ${SRC_DIR}/pub.cpp
  ${SRC_DIR}/pub.hpp
  ${SRC_DIR}/queue.cpp
  ${SRC_DIR}/queue.hpp
  ${SRC_DIR}/rep.cpp
  ${SRC_DIR}/rep.hpp
  ${SRC_DIR}/req.cpp
  ${SRC_DIR}/req.hpp
  ${SRC_DIR}/select.cpp
  ${SRC_DIR}/select.hpp
  ${SRC_DIR}/session.cpp
  ${SRC_DIR}/session.hpp
  ${SRC_DIR}/signaler.cpp
  ${SRC_DIR}/signaler.hpp
  ${SRC_DIR}/socket_base.cpp
  ${SRC_DIR}/socket_base.hpp
  ${SRC_DIR}/stdint.hpp
  ${SRC_DIR}/streamer.cpp
  ${SRC_DIR}/streamer.hpp
  ${SRC_DIR}/sub.cpp
  ${SRC_DIR}/sub.hpp
  ${SRC_DIR}/tcp_connecter.cpp
  ${SRC_DIR}/tcp_connecter.hpp
  ${SRC_DIR}/tcp_listener.cpp
  ${SRC_DIR}/tcp_listener.hpp
  ${SRC_DIR}/tcp_socket.cpp
  ${SRC_DIR}/tcp_socket.hpp
  ${SRC_DIR}/thread.cpp
  ${SRC_DIR}/thread.hpp
  ${SRC_DIR}/upstream.cpp
  ${SRC_DIR}/upstream.hpp
  ${SRC_DIR}/uuid.cpp
  ${SRC_DIR}/uuid.hpp
  ${SRC_DIR}/windows.hpp
  ${SRC_DIR}/wire.hpp
  ${SRC_DIR}/xrep.cpp
  ${SRC_DIR}/xrep.hpp
  ${SRC_DIR}/xreq.cpp
  ${SRC_DIR}/xreq.hpp
  ${SRC_DIR}/yarray.hpp
  ${SRC_DIR}/yarray_item.hpp
  ${SRC_DIR}/ypipe.hpp
  ${SRC_DIR}/yqueue.hpp
  ${SRC_DIR}/zmq.cpp
  ${SRC_DIR}/zmq_connecter.cpp
  ${SRC_DIR}/zmq_connecter.hpp
  ${SRC_DIR}/zmq_decoder.cpp
  ${SRC_DIR}/zmq_decoder.hpp
  ${SRC_DIR}/zmq_encoder.cpp
  ${SRC_DIR}/zmq_encoder.hpp
  ${SRC_DIR}/zmq_engine.cpp
  ${SRC_DIR}/zmq_engine.hpp
  ${SRC_DIR}/zmq_init.cpp
  ${SRC_DIR}/zmq_init.hpp
  ${SRC_DIR}/zmq_listener.cpp
  ${SRC_DIR}/zmq_listener.hpp
  ${INCLUDE_DIR}/zmq.h
  ${INCLUDE_DIR}/zmq.hpp
)

# Create the list of include directories needed for ZMQ header files.
INCLUDE(${ZMQ_SOURCE_DIR}/ZMQIncludeDirectories.cmake)

# This should be the only INCLUDE_DIRECTORIES command in the entire
# tree, except for the Utilities and Wrapping directories.  We need to
# do this in one place to make sure the order is correct.

INCLUDE_DIRECTORIES(
  ${ZMQ_INCLUDE_DIRS_BUILD_TREE}
  ${ZMQ_INCLUDE_DIRS_BUILD_TREE_CXX}
  ${ZMQ_INCLUDE_DIRS_SYSTEM}
)

#####################################################
INCLUDE (CheckIncludeFiles)
INCLUDE (CheckFunctionExists)
INCLUDE (CheckSymbolExists)
INCLUDE (CheckLibraryExists)
INCLUDE (CheckTypeSize)

#####################################################
# Set OS
#####################################################

IF(WIN32)
  SET(ZMQ_HAVE_WINDOWS 1)
  SET(_WINSOCKAPI_ 1)
  SET(NOMINMAX 1)
  IF(BUILD_SHARED_LIBS)
    SET(DLL_EXPORT 1)
  ENDIF()
  SET(LIB_DEP ws2_32 rpcrt4)
ENDIF(WIN32)

IF(MINGW)
  SET(ZMQ_HAVE_MINGW32 1)
ENDIF(MINGW)

IF(UNIX)
  IF(APPLE)
    SET(ZMQ_HAVE_OSX 1)
  ELSE()
    SET(ZMQ_HAVE_LINUX 1)
    SET(LIB_DEP pthread uuid)
    CHECK_INCLUDE_FILES(uuid/uuid.h HAVE_UUID_H)
    IF(NOT HAVE_UUID_H)
      MESSAGE( FATAL_ERROR "This package requires uuid-dev.")
    ENDIF()
  ENDIF()
ENDIF(UNIX)

#####################################################
# Check headers
#####################################################
CHECK_INCLUDE_FILES ("arpa/inet.h" HAVE_ARPA_INET_H)
CHECK_INCLUDE_FILES(dlfcn.h HAVE_DLFCN_H)
CHECK_INCLUDE_FILES(errno.h HAVE_ERRNO_H)
CHECK_INCLUDE_FILES (ifaddrs.h HAVE_IFADDRS_H)
CHECK_INCLUDE_FILES(inttypes.h HAVE_INTTYPES_H)
CHECK_INCLUDE_FILES(limits.h HAVE_LIMITS_H)
CHECK_INCLUDE_FILES (memory.h HAVE_MEMORY_H)
CHECK_INCLUDE_FILES(stddef.h HAVE_STDDEF_H)
CHECK_INCLUDE_FILES(stdint.h HAVE_STDINT_H)
CHECK_INCLUDE_FILES (stdlib.h HAVE_STDLIB_H)
CHECK_INCLUDE_FILES(strings.h HAVE_STRINGS_H)
CHECK_INCLUDE_FILES(string.h HAVE_STRING_H)
CHECK_INCLUDE_FILES ("sys/eventfd.h" HAVE_SYS_EVENTFD_H)
CHECK_INCLUDE_FILES ("sys/socket.h" HAVE_SYS_SOCKET_H)
CHECK_INCLUDE_FILES ("sys/stat.h" HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILES ("sys/time.h" HAVE_SYS_TIME_H)
CHECK_INCLUDE_FILES ("sys/types.h" HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILES (unistd.h HAVE_UNISTD_H)
CHECK_INCLUDE_FILES(windows.h HAVE_WINDOWS_H)
CHECK_INCLUDE_FILES(ruby.h HAVE_RUBY_H)
CHECK_INCLUDE_FILES ("netinet/in.h" HAVE_NETINET_IN_H)
CHECK_INCLUDE_FILES ("netinet/tcp.h" HAVE_NETINET_TCP_H)

#####################################################
# Check functions
#####################################################
CHECK_FUNCTION_EXISTS(freeifaddrs HAVE_FREEIFADDRS)
CHECK_FUNCTION_EXISTS(getifaddrs HAVE_GETIFADDRS)
CHECK_FUNCTION_EXISTS(gettimeofday HAVE_GETTIMEOFDAY)
CHECK_FUNCTION_EXISTS(memset HAVE_MEMSET)
CHECK_FUNCTION_EXISTS(perror HAVE_PERROR)
CHECK_FUNCTION_EXISTS(socket HAVE_SOCKET)

#####################################################
# Check libraries
#####################################################


#####################################################
# Check others
#####################################################


#####################################################

# Create the header to configure the classes.
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/platform.hpp.in
               ${PROJECT_BINARY_DIR}/platform.hpp
               @ONLY IMMEDIATE)

ADD_LIBRARY(ZMQ ${ZMQ_SRC} )
TARGET_LINK_LIBRARIES(ZMQ ${LIB_DEP})

#####################################################
# Devices and performance tests
#####################################################
IF(ZMQ_BUILD_DEVICES)
  ADD_EXECUTABLE(zmq_forwarder ${ZMQ_SOURCE_DIR}/devices/zmq_forwarder/zmq_forwarder.cpp)
  TARGET_LINK_LIBRARIES(zmq_forwarder ZMQ)

  ADD_EXECUTABLE(zmq_queue ${ZMQ_SOURCE_DIR}/devices/zmq_queue/zmq_queue.cpp)
  TARGET_LINK_LIBRARIES(zmq_queue ZMQ)

  ADD_EXECUTABLE(zmq_streamer ${ZMQ_SOURCE_DIR}/devices/zmq_streamer/zmq_streamer.cpp)
  TARGET_LINK_LIBRARIES(zmq_streamer ZMQ)
ENDIF(ZMQ_BUILD_DEVICES)

IF(ZMQ_BUILD_PERFORMANCE_TESTS)
  ADD_EXECUTABLE(local_lat ${ZMQ_SOURCE_DIR}/perf/local_lat.cpp)
  TARGET_LINK_LIBRARIES(local_lat ZMQ)

  ADD_EXECUTABLE(local_thr ${ZMQ_SOURCE_DIR}/perf/local_thr.cpp)
  TARGET_LINK_LIBRARIES(local_thr ZMQ)

  ADD_EXECUTABLE(remote_lat ${ZMQ_SOURCE_DIR}/perf/remote_lat.cpp)
  TARGET_LINK_LIBRARIES(remote_lat ZMQ)

  ADD_EXECUTABLE(remote_thr ${ZMQ_SOURCE_DIR}/perf/remote_thr.cpp)
  TARGET_LINK_LIBRARIES(remote_thr ZMQ)
ENDIF(ZMQ_BUILD_PERFORMANCE_TESTS)


#####################################################
# Help other projects use ZMQ.
#####################################################

# Copy the UseZMQ.cmake file to the binary tree for backward compatability.
CONFIGURE_FILE(${ZMQ_SOURCE_DIR}/UseZMQ.cmake.in
${ZMQ_BINARY_DIR}/UseZMQ.cmake COPYONLY IMMEDIATE)

# Create the ZMQConfig.cmake file containing the ZMQ configuration.
INCLUDE (${ZMQ_SOURCE_DIR}/GenerateZMQConfig.cmake)

# Save the compiler settings so another project can import them.
#INCLUDE(${CMAKE_ROOT}/Modules/CMakeExportBuildSettings.cmake)
#CMAKE_EXPORT_BUILD_SETTINGS(${ZMQ_BINARY_DIR}/ZMQBuildSettings.cmake)

# Save library dependencies.
EXPORT_LIBRARY_DEPENDENCIES(${ZMQ_BINARY_DIR}/ZMQLibraryDepends.cmake)


#-----------------------------------------------------------------------------
# Installation vars.
# ZMQ_INSTALL_BIN_DIR          - binary dir (executables)
# ZMQ_INSTALL_LIB_DIR          - library dir (libs)
# ZMQ_INSTALL_DATA_DIR         - share dir (say, examples, data, etc)
# ZMQ_INSTALL_INCLUDE_DIR      - include dir (headers)
# ZMQ_INSTALL_PACKAGE_DIR      - package/export configuration files
# ZMQ_INSTALL_NO_DEVELOPMENT   - do not install development files
# ZMQ_INSTALL_NO_RUNTIME       - do not install runtime files
# ZMQ_INSTALL_NO_DOCUMENTATION - do not install documentation files

IF(NOT ZMQ_INSTALL_BIN_DIR)
  SET(ZMQ_INSTALL_BIN_DIR "bin")
ENDIF(NOT ZMQ_INSTALL_BIN_DIR)

IF(NOT ZMQ_INSTALL_LIB_DIR)
  SET(ZMQ_INSTALL_LIB_DIR "lib/zmq")
ENDIF(NOT ZMQ_INSTALL_LIB_DIR)

IF(NOT ZMQ_INSTALL_DATA_DIR)
  SET(ZMQ_INSTALL_DATA_DIR "share/zmq")
ENDIF(NOT ZMQ_INSTALL_DATA_DIR)

IF(NOT ZMQ_INSTALL_INCLUDE_DIR)
  SET(ZMQ_INSTALL_INCLUDE_DIR "include/zmq")
ENDIF(NOT ZMQ_INSTALL_INCLUDE_DIR)

IF(NOT ZMQ_INSTALL_PACKAGE_DIR)
  SET(ZMQ_INSTALL_PACKAGE_DIR ${ZMQ_INSTALL_LIB_DIR}
    CACHE INTERNAL "")
ENDIF(NOT ZMQ_INSTALL_PACKAGE_DIR)

IF(NOT ZMQ_INSTALL_NO_DOCUMENTATION)
  SET(ZMQ_INSTALL_NO_DOCUMENTATION 0)
ENDIF(NOT ZMQ_INSTALL_NO_DOCUMENTATION)

INSTALL(FILES
  ${ZMQ_BINARY_DIR}/ZMQBuildSettings.cmake
  ${ZMQ_BINARY_DIR}/ZMQLibraryDepends.cmake
  ${ZMQ_BINARY_DIR}/UseZMQ.cmake
  DESTINATION ${ZMQ_INSTALL_PACKAGE_DIR}
  COMPONENT Development
  )

