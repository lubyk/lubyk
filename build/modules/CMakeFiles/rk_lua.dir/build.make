# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /opt/local/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/gaspard/git/rubyk

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/gaspard/git/rubyk/build

# Utility rule file for rk_lua.

modules/CMakeFiles/rk_lua:
	cd /Users/gaspard/git/rubyk/build/modules && /opt/local/bin/cmake -E copy_directory /Users/gaspard/git/rubyk/modules/rk/lua /Users/gaspard/git/rubyk/lib

rk_lua: modules/CMakeFiles/rk_lua
rk_lua: modules/CMakeFiles/rk_lua.dir/build.make
.PHONY : rk_lua

# Rule to build all files generated by this target.
modules/CMakeFiles/rk_lua.dir/build: rk_lua
.PHONY : modules/CMakeFiles/rk_lua.dir/build

modules/CMakeFiles/rk_lua.dir/clean:
	cd /Users/gaspard/git/rubyk/build/modules && $(CMAKE_COMMAND) -P CMakeFiles/rk_lua.dir/cmake_clean.cmake
.PHONY : modules/CMakeFiles/rk_lua.dir/clean

modules/CMakeFiles/rk_lua.dir/depend:
	cd /Users/gaspard/git/rubyk/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/gaspard/git/rubyk /Users/gaspard/git/rubyk/modules /Users/gaspard/git/rubyk/build /Users/gaspard/git/rubyk/build/modules /Users/gaspard/git/rubyk/build/modules/CMakeFiles/rk_lua.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : modules/CMakeFiles/rk_lua.dir/depend

