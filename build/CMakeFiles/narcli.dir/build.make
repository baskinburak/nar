# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/baskin/nar/nar/narnode

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/baskin/nar/nar/narnode/build

# Include any dependencies generated for this target.
include CMakeFiles/narcli.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/narcli.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/narcli.dir/flags.make

CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.o: CMakeFiles/narcli.dir/flags.make
CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.o: /home/baskin/nar/nar/lib/USocket/Packet.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/baskin/nar/nar/narnode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.o -c /home/baskin/nar/nar/lib/USocket/Packet.cpp

CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/baskin/nar/nar/lib/USocket/Packet.cpp > CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.i

CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/baskin/nar/nar/lib/USocket/Packet.cpp -o CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.s

CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.o.requires:

.PHONY : CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.o.requires

CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.o.provides: CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.o.requires
	$(MAKE) -f CMakeFiles/narcli.dir/build.make CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.o.provides.build
.PHONY : CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.o.provides

CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.o.provides.build: CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.o


CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.o: CMakeFiles/narcli.dir/flags.make
CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.o: /home/baskin/nar/nar/lib/USocket/USocket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/baskin/nar/nar/narnode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.o -c /home/baskin/nar/nar/lib/USocket/USocket.cpp

CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/baskin/nar/nar/lib/USocket/USocket.cpp > CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.i

CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/baskin/nar/nar/lib/USocket/USocket.cpp -o CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.s

CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.o.requires:

.PHONY : CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.o.requires

CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.o.provides: CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.o.requires
	$(MAKE) -f CMakeFiles/narcli.dir/build.make CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.o.provides.build
.PHONY : CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.o.provides

CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.o.provides.build: CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.o


CMakeFiles/narcli.dir/cli.cpp.o: CMakeFiles/narcli.dir/flags.make
CMakeFiles/narcli.dir/cli.cpp.o: ../cli.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/baskin/nar/nar/narnode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/narcli.dir/cli.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/narcli.dir/cli.cpp.o -c /home/baskin/nar/nar/narnode/cli.cpp

CMakeFiles/narcli.dir/cli.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/narcli.dir/cli.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/baskin/nar/nar/narnode/cli.cpp > CMakeFiles/narcli.dir/cli.cpp.i

CMakeFiles/narcli.dir/cli.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/narcli.dir/cli.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/baskin/nar/nar/narnode/cli.cpp -o CMakeFiles/narcli.dir/cli.cpp.s

CMakeFiles/narcli.dir/cli.cpp.o.requires:

.PHONY : CMakeFiles/narcli.dir/cli.cpp.o.requires

CMakeFiles/narcli.dir/cli.cpp.o.provides: CMakeFiles/narcli.dir/cli.cpp.o.requires
	$(MAKE) -f CMakeFiles/narcli.dir/build.make CMakeFiles/narcli.dir/cli.cpp.o.provides.build
.PHONY : CMakeFiles/narcli.dir/cli.cpp.o.provides

CMakeFiles/narcli.dir/cli.cpp.o.provides.build: CMakeFiles/narcli.dir/cli.cpp.o


CMakeFiles/narcli.dir/utility.cpp.o: CMakeFiles/narcli.dir/flags.make
CMakeFiles/narcli.dir/utility.cpp.o: ../utility.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/baskin/nar/nar/narnode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/narcli.dir/utility.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/narcli.dir/utility.cpp.o -c /home/baskin/nar/nar/narnode/utility.cpp

CMakeFiles/narcli.dir/utility.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/narcli.dir/utility.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/baskin/nar/nar/narnode/utility.cpp > CMakeFiles/narcli.dir/utility.cpp.i

CMakeFiles/narcli.dir/utility.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/narcli.dir/utility.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/baskin/nar/nar/narnode/utility.cpp -o CMakeFiles/narcli.dir/utility.cpp.s

CMakeFiles/narcli.dir/utility.cpp.o.requires:

.PHONY : CMakeFiles/narcli.dir/utility.cpp.o.requires

CMakeFiles/narcli.dir/utility.cpp.o.provides: CMakeFiles/narcli.dir/utility.cpp.o.requires
	$(MAKE) -f CMakeFiles/narcli.dir/build.make CMakeFiles/narcli.dir/utility.cpp.o.provides.build
.PHONY : CMakeFiles/narcli.dir/utility.cpp.o.provides

CMakeFiles/narcli.dir/utility.cpp.o.provides.build: CMakeFiles/narcli.dir/utility.cpp.o


CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.o: CMakeFiles/narcli.dir/flags.make
CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.o: /home/baskin/nar/nar/lib/Socket/Socket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/baskin/nar/nar/narnode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.o -c /home/baskin/nar/nar/lib/Socket/Socket.cpp

CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/baskin/nar/nar/lib/Socket/Socket.cpp > CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.i

CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/baskin/nar/nar/lib/Socket/Socket.cpp -o CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.s

CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.o.requires:

.PHONY : CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.o.requires

CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.o.provides: CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.o.requires
	$(MAKE) -f CMakeFiles/narcli.dir/build.make CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.o.provides.build
.PHONY : CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.o.provides

CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.o.provides.build: CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.o


# Object files for target narcli
narcli_OBJECTS = \
"CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.o" \
"CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.o" \
"CMakeFiles/narcli.dir/cli.cpp.o" \
"CMakeFiles/narcli.dir/utility.cpp.o" \
"CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.o"

# External object files for target narcli
narcli_EXTERNAL_OBJECTS =

narcli: CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.o
narcli: CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.o
narcli: CMakeFiles/narcli.dir/cli.cpp.o
narcli: CMakeFiles/narcli.dir/utility.cpp.o
narcli: CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.o
narcli: CMakeFiles/narcli.dir/build.make
narcli: CMakeFiles/narcli.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/baskin/nar/nar/narnode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable narcli"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/narcli.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/narcli.dir/build: narcli

.PHONY : CMakeFiles/narcli.dir/build

CMakeFiles/narcli.dir/requires: CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/Packet.cpp.o.requires
CMakeFiles/narcli.dir/requires: CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/USocket/USocket.cpp.o.requires
CMakeFiles/narcli.dir/requires: CMakeFiles/narcli.dir/cli.cpp.o.requires
CMakeFiles/narcli.dir/requires: CMakeFiles/narcli.dir/utility.cpp.o.requires
CMakeFiles/narcli.dir/requires: CMakeFiles/narcli.dir/home/baskin/nar/nar/lib/Socket/Socket.cpp.o.requires

.PHONY : CMakeFiles/narcli.dir/requires

CMakeFiles/narcli.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/narcli.dir/cmake_clean.cmake
.PHONY : CMakeFiles/narcli.dir/clean

CMakeFiles/narcli.dir/depend:
	cd /home/baskin/nar/nar/narnode/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/baskin/nar/nar/narnode /home/baskin/nar/nar/narnode /home/baskin/nar/nar/narnode/build /home/baskin/nar/nar/narnode/build /home/baskin/nar/nar/narnode/build/CMakeFiles/narcli.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/narcli.dir/depend

