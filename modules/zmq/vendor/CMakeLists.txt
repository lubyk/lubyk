##############################################################
# This CMakeLists.txt supports Windows/MSVC, Linux, and OSX
# http://github.com/pieper/CTK
#
# CMakeList created by https://github.com/PatrickCheng/zeromq2
# and adapted for Lubyk (see [lubyk])
##############################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

PROJECT(ZMQ)

SET(PACKAGE "ZMQ")
SET(PACKAGE_NAME "zeroMQ")
SET(PACKAGE_VERSION "2.0.10")

SET(PACKAGE_VERSION_MAJOR 2)
SET(PACKAGE_VERSION_MINOR 0)
SET(PACKAGE_VERSION_PATCH 10)

SET(SRC_DIR ${ZMQ_SOURCE_DIR}/src)
SET(INCLUDE_DIR ${ZMQ_SOURCE_DIR}/include)

file(GLOB ZMQ_SRC ${SRC_DIR}/*.cpp ${SRC_DIR}/*.hpp ${INCLUDE_DIR}/zmq.h ${INCLUDE_DIR}/zmq.hpp)

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
  SET(DLL_EXPORT 1)
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

# ----------------------------------------------------------------------------------
#  [lubyk]
# ----------------------------------------------------------------------------------
#ADD_LIBRARY(ZMQ ${ZMQ_SRC} )
#TARGET_LINK_LIBRARIES(ZMQ ${LIB_DEP})
build_sub_module("vendor" ${ZMQ_SRC} ${ZMQ_SOURCE_DIR}/lubyk.cpp)

# ----------------------------------------------------------------------------------
#  [/lubyk]
# ----------------------------------------------------------------------------------
