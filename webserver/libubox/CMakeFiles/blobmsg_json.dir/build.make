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
include CMakeFiles/blobmsg_json.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/blobmsg_json.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/blobmsg_json.dir/flags.make

CMakeFiles/blobmsg_json.dir/blobmsg_json.c.o: CMakeFiles/blobmsg_json.dir/flags.make
CMakeFiles/blobmsg_json.dir/blobmsg_json.c.o: blobmsg_json.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/gunix/workspace/src/webserver/libubox/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/blobmsg_json.dir/blobmsg_json.c.o"
	/opt/arm-2010.09/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/blobmsg_json.dir/blobmsg_json.c.o   -c /home/gunix/workspace/src/webserver/libubox/blobmsg_json.c

CMakeFiles/blobmsg_json.dir/blobmsg_json.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blobmsg_json.dir/blobmsg_json.c.i"
	/opt/arm-2010.09/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/gunix/workspace/src/webserver/libubox/blobmsg_json.c > CMakeFiles/blobmsg_json.dir/blobmsg_json.c.i

CMakeFiles/blobmsg_json.dir/blobmsg_json.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blobmsg_json.dir/blobmsg_json.c.s"
	/opt/arm-2010.09/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/gunix/workspace/src/webserver/libubox/blobmsg_json.c -o CMakeFiles/blobmsg_json.dir/blobmsg_json.c.s

CMakeFiles/blobmsg_json.dir/blobmsg_json.c.o.requires:
.PHONY : CMakeFiles/blobmsg_json.dir/blobmsg_json.c.o.requires

CMakeFiles/blobmsg_json.dir/blobmsg_json.c.o.provides: CMakeFiles/blobmsg_json.dir/blobmsg_json.c.o.requires
	$(MAKE) -f CMakeFiles/blobmsg_json.dir/build.make CMakeFiles/blobmsg_json.dir/blobmsg_json.c.o.provides.build
.PHONY : CMakeFiles/blobmsg_json.dir/blobmsg_json.c.o.provides

CMakeFiles/blobmsg_json.dir/blobmsg_json.c.o.provides.build: CMakeFiles/blobmsg_json.dir/blobmsg_json.c.o

# Object files for target blobmsg_json
blobmsg_json_OBJECTS = \
"CMakeFiles/blobmsg_json.dir/blobmsg_json.c.o"

# External object files for target blobmsg_json
blobmsg_json_EXTERNAL_OBJECTS =

libblobmsg_json.so: CMakeFiles/blobmsg_json.dir/blobmsg_json.c.o
libblobmsg_json.so: CMakeFiles/blobmsg_json.dir/build.make
libblobmsg_json.so: libubox.so
libblobmsg_json.so: CMakeFiles/blobmsg_json.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C shared library libblobmsg_json.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/blobmsg_json.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/blobmsg_json.dir/build: libblobmsg_json.so
.PHONY : CMakeFiles/blobmsg_json.dir/build

# Object files for target blobmsg_json
blobmsg_json_OBJECTS = \
"CMakeFiles/blobmsg_json.dir/blobmsg_json.c.o"

# External object files for target blobmsg_json
blobmsg_json_EXTERNAL_OBJECTS =

CMakeFiles/CMakeRelink.dir/libblobmsg_json.so: CMakeFiles/blobmsg_json.dir/blobmsg_json.c.o
CMakeFiles/CMakeRelink.dir/libblobmsg_json.so: CMakeFiles/blobmsg_json.dir/build.make
CMakeFiles/CMakeRelink.dir/libblobmsg_json.so: libubox.so
CMakeFiles/CMakeRelink.dir/libblobmsg_json.so: CMakeFiles/blobmsg_json.dir/relink.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C shared library CMakeFiles/CMakeRelink.dir/libblobmsg_json.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/blobmsg_json.dir/relink.txt --verbose=$(VERBOSE)

# Rule to relink during preinstall.
CMakeFiles/blobmsg_json.dir/preinstall: CMakeFiles/CMakeRelink.dir/libblobmsg_json.so
.PHONY : CMakeFiles/blobmsg_json.dir/preinstall

CMakeFiles/blobmsg_json.dir/requires: CMakeFiles/blobmsg_json.dir/blobmsg_json.c.o.requires
.PHONY : CMakeFiles/blobmsg_json.dir/requires

CMakeFiles/blobmsg_json.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/blobmsg_json.dir/cmake_clean.cmake
.PHONY : CMakeFiles/blobmsg_json.dir/clean

CMakeFiles/blobmsg_json.dir/depend:
	cd /home/gunix/workspace/src/webserver/libubox && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gunix/workspace/src/webserver/libubox /home/gunix/workspace/src/webserver/libubox /home/gunix/workspace/src/webserver/libubox /home/gunix/workspace/src/webserver/libubox /home/gunix/workspace/src/webserver/libubox/CMakeFiles/blobmsg_json.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/blobmsg_json.dir/depend

