# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /opt/clion-2018.2.6/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion-2018.2.6/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/maciek/studies/4_sem/sik/zad2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/maciek/studies/4_sem/sik/zad2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/zad2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/zad2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/zad2.dir/flags.make

CMakeFiles/zad2.dir/netstore-server.cpp.o: CMakeFiles/zad2.dir/flags.make
CMakeFiles/zad2.dir/netstore-server.cpp.o: ../netstore-server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maciek/studies/4_sem/sik/zad2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/zad2.dir/netstore-server.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/zad2.dir/netstore-server.cpp.o -c /home/maciek/studies/4_sem/sik/zad2/netstore-server.cpp

CMakeFiles/zad2.dir/netstore-server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zad2.dir/netstore-server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maciek/studies/4_sem/sik/zad2/netstore-server.cpp > CMakeFiles/zad2.dir/netstore-server.cpp.i

CMakeFiles/zad2.dir/netstore-server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zad2.dir/netstore-server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maciek/studies/4_sem/sik/zad2/netstore-server.cpp -o CMakeFiles/zad2.dir/netstore-server.cpp.s

CMakeFiles/zad2.dir/structures.cpp.o: CMakeFiles/zad2.dir/flags.make
CMakeFiles/zad2.dir/structures.cpp.o: ../structures.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maciek/studies/4_sem/sik/zad2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/zad2.dir/structures.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/zad2.dir/structures.cpp.o -c /home/maciek/studies/4_sem/sik/zad2/structures.cpp

CMakeFiles/zad2.dir/structures.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zad2.dir/structures.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maciek/studies/4_sem/sik/zad2/structures.cpp > CMakeFiles/zad2.dir/structures.cpp.i

CMakeFiles/zad2.dir/structures.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zad2.dir/structures.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maciek/studies/4_sem/sik/zad2/structures.cpp -o CMakeFiles/zad2.dir/structures.cpp.s

CMakeFiles/zad2.dir/netstore-client.cpp.o: CMakeFiles/zad2.dir/flags.make
CMakeFiles/zad2.dir/netstore-client.cpp.o: ../netstore-client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maciek/studies/4_sem/sik/zad2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/zad2.dir/netstore-client.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/zad2.dir/netstore-client.cpp.o -c /home/maciek/studies/4_sem/sik/zad2/netstore-client.cpp

CMakeFiles/zad2.dir/netstore-client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zad2.dir/netstore-client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maciek/studies/4_sem/sik/zad2/netstore-client.cpp > CMakeFiles/zad2.dir/netstore-client.cpp.i

CMakeFiles/zad2.dir/netstore-client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zad2.dir/netstore-client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maciek/studies/4_sem/sik/zad2/netstore-client.cpp -o CMakeFiles/zad2.dir/netstore-client.cpp.s

# Object files for target zad2
zad2_OBJECTS = \
"CMakeFiles/zad2.dir/netstore-server.cpp.o" \
"CMakeFiles/zad2.dir/structures.cpp.o" \
"CMakeFiles/zad2.dir/netstore-client.cpp.o"

# External object files for target zad2
zad2_EXTERNAL_OBJECTS =

zad2: CMakeFiles/zad2.dir/netstore-server.cpp.o
zad2: CMakeFiles/zad2.dir/structures.cpp.o
zad2: CMakeFiles/zad2.dir/netstore-client.cpp.o
zad2: CMakeFiles/zad2.dir/build.make
zad2: CMakeFiles/zad2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/maciek/studies/4_sem/sik/zad2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable zad2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/zad2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/zad2.dir/build: zad2

.PHONY : CMakeFiles/zad2.dir/build

CMakeFiles/zad2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/zad2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/zad2.dir/clean

CMakeFiles/zad2.dir/depend:
	cd /home/maciek/studies/4_sem/sik/zad2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maciek/studies/4_sem/sik/zad2 /home/maciek/studies/4_sem/sik/zad2 /home/maciek/studies/4_sem/sik/zad2/cmake-build-debug /home/maciek/studies/4_sem/sik/zad2/cmake-build-debug /home/maciek/studies/4_sem/sik/zad2/cmake-build-debug/CMakeFiles/zad2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/zad2.dir/depend

