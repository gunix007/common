# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/gunix/workspace/src/webserver/libubox

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gunix/workspace/src/webserver/libubox

# Include any dependencies generated for this target.
include examples/CMakeFiles/json_script-example.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/json_script-example.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/json_script-example.dir/flags.make

examples/CMakeFiles/json_script-example.dir/json_script-example.c.o: examples/CMakeFiles/json_script-example.dir/flags.make
examples/CMakeFiles/json_script-example.dir/json_script-example.c.o: examples/json_script-example.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/gunix/workspace/src/webserver/libubox/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/CMakeFiles/json_script-example.dir/json_script-example.c.o"
	cd /home/gunix/workspace/src/webserver/libubox/examples && /opt/arm-2010.09/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/json_script-example.dir/json_script-example.c.o   -c /home/gunix/workspace/src/webserver/libubox/examples/json_script-example.c

examples/CMakeFiles/json_script-example.dir/json_script-example.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/json_script-example.dir/json_script-example.c.i"
	cd /home/gunix/workspace/src/webserver/libubox/examples && /opt/arm-2010.09/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/gunix/workspace/src/webserver/libubox/examples/json_script-example.c > CMakeFiles/json_script-example.dir/json_script-example.c.i

examples/CMakeFiles/json_script-example.dir/json_script-example.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/json_script-example.dir/json_script-example.c.s"
	cd /home/gunix/workspace/src/webserver/libubox/examples && /opt/arm-2010.09/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/gunix/workspace/src/webserver/libubox/examples/json_script-example.c -o CMakeFiles/json_script-example.dir/json_script-example.c.s

examples/CMakeFiles/json_script-example.dir/json_script-example.c.o.requires:
.PHONY : examples/CMakeFiles/json_script-example.dir/json_script-example.c.o.requires

examples/CMakeFiles/json_script-example.dir/json_script-example.c.o.provides: examples/CMakeFiles/json_script-example.dir/json_script-example.c.o.requires
	$(MAKE) -f examples/CMakeFiles/json_script-example.dir/build.make examples/CMakeFiles/json_script-example.dir/json_script-example.c.o.provides.build
.PHONY : examples/CMakeFiles/json_script-example.dir/json_script-example.c.o.provides

examples/CMakeFiles/json_script-example.dir/json_script-example.c.o.provides.build: examples/CMakeFiles/json_script-example.dir/json_script-example.c.o

# Object files for target json_script-example
json_script__example_OBJECTS = \
"CMakeFiles/json_script-example.dir/json_script-example.c.o"

# External object files for target json_script-example
json_script__example_EXTERNAL_OBJECTS =

examples/json_script-example: examples/CMakeFiles/json_script-example.dir/json_script-example.c.o
examples/json_script-example: examples/CMakeFiles/json_script-example.dir/build.make
examples/json_script-example: libubox.so
examples/json_script-example: libblobmsg_json.so
examples/json_script-example: libjson_script.so
examples/json_script-example: libubox.so
examples/json_script-example: examples/CMakeFiles/json_script-example.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable json_script-example"
	cd /home/gunix/workspace/src/webserver/libubox/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/json_script-example.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/json_script-example.dir/build: examples/json_script-example
.PHONY : examples/CMakeFiles/json_script-example.dir/build

examples/CMakeFiles/json_script-example.dir/requires: examples/CMakeFiles/json_script-example.dir/json_script-example.c.o.requires
.PHONY : examples/CMakeFiles/json_script-example.dir/requires

examples/CMakeFiles/json_script-example.dir/clean:
	cd /home/gunix/workspace/src/webserver/libubox/examples && $(CMAKE_COMMAND) -P CMakeFiles/json_script-example.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/json_script-example.dir/clean

examples/CMakeFiles/json_script-example.dir/depend:
	cd /home/gunix/workspace/src/webserver/libubox && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gunix/workspace/src/webserver/libubox /home/gunix/workspace/src/webserver/libubox/examples /home/gunix/workspace/src/webserver/libubox /home/gunix/workspace/src/webserver/libubox/examples /home/gunix/workspace/src/webserver/libubox/examples/CMakeFiles/json_script-example.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/json_script-example.dir/depend

