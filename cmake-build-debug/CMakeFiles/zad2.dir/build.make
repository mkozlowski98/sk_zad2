# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /snap/clion/73/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/73/bin/cmake/linux/bin/cmake -E remove -f


# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/koksu/studies/4_sem/sik/zad2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/koksu/studies/4_sem/sik/zad2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/zad2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/zad2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/zad2.dir/flags.make

CMakeFiles/zad2.dir/netstore-server.cpp.o: CMakeFiles/zad2.dir/flags.make
CMakeFiles/zad2.dir/netstore-server.cpp.o: ../netstore-server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/koksu/studies/4_sem/sik/zad2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/zad2.dir/netstore-server.cpp.o"
	/usr/bin/g++-8  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/zad2.dir/netstore-server.cpp.o -c /home/koksu/studies/4_sem/sik/zad2/netstore-server.cpp

CMakeFiles/zad2.dir/netstore-server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zad2.dir/netstore-server.cpp.i"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/koksu/studies/4_sem/sik/zad2/netstore-server.cpp > CMakeFiles/zad2.dir/netstore-server.cpp.i

CMakeFiles/zad2.dir/netstore-server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zad2.dir/netstore-server.cpp.s"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/koksu/studies/4_sem/sik/zad2/netstore-server.cpp -o CMakeFiles/zad2.dir/netstore-server.cpp.s

CMakeFiles/zad2.dir/sock.cpp.o: CMakeFiles/zad2.dir/flags.make
CMakeFiles/zad2.dir/sock.cpp.o: ../sock.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/koksu/studies/4_sem/sik/zad2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/zad2.dir/sock.cpp.o"
	/usr/bin/g++-8  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/zad2.dir/sock.cpp.o -c /home/koksu/studies/4_sem/sik/zad2/sock.cpp

CMakeFiles/zad2.dir/sock.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zad2.dir/sock.cpp.i"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/koksu/studies/4_sem/sik/zad2/sock.cpp > CMakeFiles/zad2.dir/sock.cpp.i

CMakeFiles/zad2.dir/sock.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zad2.dir/sock.cpp.s"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/koksu/studies/4_sem/sik/zad2/sock.cpp -o CMakeFiles/zad2.dir/sock.cpp.s

CMakeFiles/zad2.dir/netstore-client.cpp.o: CMakeFiles/zad2.dir/flags.make
CMakeFiles/zad2.dir/netstore-client.cpp.o: ../netstore-client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/koksu/studies/4_sem/sik/zad2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/zad2.dir/netstore-client.cpp.o"
	/usr/bin/g++-8  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/zad2.dir/netstore-client.cpp.o -c /home/koksu/studies/4_sem/sik/zad2/netstore-client.cpp

CMakeFiles/zad2.dir/netstore-client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zad2.dir/netstore-client.cpp.i"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/koksu/studies/4_sem/sik/zad2/netstore-client.cpp > CMakeFiles/zad2.dir/netstore-client.cpp.i

CMakeFiles/zad2.dir/netstore-client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zad2.dir/netstore-client.cpp.s"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/koksu/studies/4_sem/sik/zad2/netstore-client.cpp -o CMakeFiles/zad2.dir/netstore-client.cpp.s

CMakeFiles/zad2.dir/messages.cpp.o: CMakeFiles/zad2.dir/flags.make
CMakeFiles/zad2.dir/messages.cpp.o: ../messages.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/koksu/studies/4_sem/sik/zad2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/zad2.dir/messages.cpp.o"
	/usr/bin/g++-8  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/zad2.dir/messages.cpp.o -c /home/koksu/studies/4_sem/sik/zad2/messages.cpp

CMakeFiles/zad2.dir/messages.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zad2.dir/messages.cpp.i"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/koksu/studies/4_sem/sik/zad2/messages.cpp > CMakeFiles/zad2.dir/messages.cpp.i

CMakeFiles/zad2.dir/messages.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zad2.dir/messages.cpp.s"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/koksu/studies/4_sem/sik/zad2/messages.cpp -o CMakeFiles/zad2.dir/messages.cpp.s

CMakeFiles/zad2.dir/globals.cpp.o: CMakeFiles/zad2.dir/flags.make
CMakeFiles/zad2.dir/globals.cpp.o: ../globals.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/koksu/studies/4_sem/sik/zad2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/zad2.dir/globals.cpp.o"
	/usr/bin/g++-8  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/zad2.dir/globals.cpp.o -c /home/koksu/studies/4_sem/sik/zad2/globals.cpp

CMakeFiles/zad2.dir/globals.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zad2.dir/globals.cpp.i"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/koksu/studies/4_sem/sik/zad2/globals.cpp > CMakeFiles/zad2.dir/globals.cpp.i

CMakeFiles/zad2.dir/globals.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zad2.dir/globals.cpp.s"
	/usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/koksu/studies/4_sem/sik/zad2/globals.cpp -o CMakeFiles/zad2.dir/globals.cpp.s
=======
CMakeFiles/zad2.dir/netstore-server.c.o: CMakeFiles/zad2.dir/flags.make
CMakeFiles/zad2.dir/netstore-server.c.o: ../netstore-server.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/koksu/studies/4_sem/sik/zad2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/zad2.dir/netstore-server.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zad2.dir/netstore-server.c.o   -c /home/koksu/studies/4_sem/sik/zad2/netstore-server.c

CMakeFiles/zad2.dir/netstore-server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zad2.dir/netstore-server.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/koksu/studies/4_sem/sik/zad2/netstore-server.c > CMakeFiles/zad2.dir/netstore-server.c.i

CMakeFiles/zad2.dir/netstore-server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zad2.dir/netstore-server.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/koksu/studies/4_sem/sik/zad2/netstore-server.c -o CMakeFiles/zad2.dir/netstore-server.c.s

CMakeFiles/zad2.dir/netstore-client.c.o: CMakeFiles/zad2.dir/flags.make
CMakeFiles/zad2.dir/netstore-client.c.o: ../netstore-client.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/koksu/studies/4_sem/sik/zad2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/zad2.dir/netstore-client.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zad2.dir/netstore-client.c.o   -c /home/koksu/studies/4_sem/sik/zad2/netstore-client.c

CMakeFiles/zad2.dir/netstore-client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zad2.dir/netstore-client.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/koksu/studies/4_sem/sik/zad2/netstore-client.c > CMakeFiles/zad2.dir/netstore-client.c.i

CMakeFiles/zad2.dir/netstore-client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zad2.dir/netstore-client.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/koksu/studies/4_sem/sik/zad2/netstore-client.c -o CMakeFiles/zad2.dir/netstore-client.c.s

# Object files for target zad2
zad2_OBJECTS = \
"CMakeFiles/zad2.dir/netstore-server.cpp.o" \
"CMakeFiles/zad2.dir/sock.cpp.o" \
"CMakeFiles/zad2.dir/netstore-client.cpp.o" \
"CMakeFiles/zad2.dir/messages.cpp.o" \
"CMakeFiles/zad2.dir/globals.cpp.o"

# External object files for target zad2
zad2_EXTERNAL_OBJECTS =

zad2: CMakeFiles/zad2.dir/netstore-server.cpp.o
zad2: CMakeFiles/zad2.dir/sock.cpp.o
zad2: CMakeFiles/zad2.dir/netstore-client.cpp.o
zad2: CMakeFiles/zad2.dir/messages.cpp.o
zad2: CMakeFiles/zad2.dir/globals.cpp.o
zad2: CMakeFiles/zad2.dir/build.make
zad2: CMakeFiles/zad2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/koksu/studies/4_sem/sik/zad2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable zad2"

	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/zad2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/zad2.dir/build: zad2

.PHONY : CMakeFiles/zad2.dir/build

CMakeFiles/zad2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/zad2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/zad2.dir/clean

CMakeFiles/zad2.dir/depend:
	cd /home/koksu/studies/4_sem/sik/zad2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/koksu/studies/4_sem/sik/zad2 /home/koksu/studies/4_sem/sik/zad2 /home/koksu/studies/4_sem/sik/zad2/cmake-build-debug /home/koksu/studies/4_sem/sik/zad2/cmake-build-debug /home/koksu/studies/4_sem/sik/zad2/cmake-build-debug/CMakeFiles/zad2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/zad2.dir/depend

