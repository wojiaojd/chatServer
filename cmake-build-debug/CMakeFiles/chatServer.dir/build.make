# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /home/wojiaojd/下载/clion-2020.1.1/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/wojiaojd/下载/clion-2020.1.1/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/wojiaojd/CLionProjects/chatServer/chatServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wojiaojd/CLionProjects/chatServer/chatServer/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/chatServer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/chatServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/chatServer.dir/flags.make

CMakeFiles/chatServer.dir/main.c.o: CMakeFiles/chatServer.dir/flags.make
CMakeFiles/chatServer.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wojiaojd/CLionProjects/chatServer/chatServer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/chatServer.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/chatServer.dir/main.c.o   -c /home/wojiaojd/CLionProjects/chatServer/chatServer/main.c

CMakeFiles/chatServer.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/chatServer.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/wojiaojd/CLionProjects/chatServer/chatServer/main.c > CMakeFiles/chatServer.dir/main.c.i

CMakeFiles/chatServer.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/chatServer.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/wojiaojd/CLionProjects/chatServer/chatServer/main.c -o CMakeFiles/chatServer.dir/main.c.s

CMakeFiles/chatServer.dir/pthreadpool.c.o: CMakeFiles/chatServer.dir/flags.make
CMakeFiles/chatServer.dir/pthreadpool.c.o: ../pthreadpool.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wojiaojd/CLionProjects/chatServer/chatServer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/chatServer.dir/pthreadpool.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/chatServer.dir/pthreadpool.c.o   -c /home/wojiaojd/CLionProjects/chatServer/chatServer/pthreadpool.c

CMakeFiles/chatServer.dir/pthreadpool.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/chatServer.dir/pthreadpool.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/wojiaojd/CLionProjects/chatServer/chatServer/pthreadpool.c > CMakeFiles/chatServer.dir/pthreadpool.c.i

CMakeFiles/chatServer.dir/pthreadpool.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/chatServer.dir/pthreadpool.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/wojiaojd/CLionProjects/chatServer/chatServer/pthreadpool.c -o CMakeFiles/chatServer.dir/pthreadpool.c.s

CMakeFiles/chatServer.dir/sockqueue.c.o: CMakeFiles/chatServer.dir/flags.make
CMakeFiles/chatServer.dir/sockqueue.c.o: ../sockqueue.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wojiaojd/CLionProjects/chatServer/chatServer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/chatServer.dir/sockqueue.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/chatServer.dir/sockqueue.c.o   -c /home/wojiaojd/CLionProjects/chatServer/chatServer/sockqueue.c

CMakeFiles/chatServer.dir/sockqueue.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/chatServer.dir/sockqueue.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/wojiaojd/CLionProjects/chatServer/chatServer/sockqueue.c > CMakeFiles/chatServer.dir/sockqueue.c.i

CMakeFiles/chatServer.dir/sockqueue.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/chatServer.dir/sockqueue.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/wojiaojd/CLionProjects/chatServer/chatServer/sockqueue.c -o CMakeFiles/chatServer.dir/sockqueue.c.s

CMakeFiles/chatServer.dir/errorhandler.c.o: CMakeFiles/chatServer.dir/flags.make
CMakeFiles/chatServer.dir/errorhandler.c.o: ../errorhandler.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wojiaojd/CLionProjects/chatServer/chatServer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/chatServer.dir/errorhandler.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/chatServer.dir/errorhandler.c.o   -c /home/wojiaojd/CLionProjects/chatServer/chatServer/errorhandler.c

CMakeFiles/chatServer.dir/errorhandler.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/chatServer.dir/errorhandler.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/wojiaojd/CLionProjects/chatServer/chatServer/errorhandler.c > CMakeFiles/chatServer.dir/errorhandler.c.i

CMakeFiles/chatServer.dir/errorhandler.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/chatServer.dir/errorhandler.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/wojiaojd/CLionProjects/chatServer/chatServer/errorhandler.c -o CMakeFiles/chatServer.dir/errorhandler.c.s

CMakeFiles/chatServer.dir/database.c.o: CMakeFiles/chatServer.dir/flags.make
CMakeFiles/chatServer.dir/database.c.o: ../database.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wojiaojd/CLionProjects/chatServer/chatServer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/chatServer.dir/database.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/chatServer.dir/database.c.o   -c /home/wojiaojd/CLionProjects/chatServer/chatServer/database.c

CMakeFiles/chatServer.dir/database.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/chatServer.dir/database.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/wojiaojd/CLionProjects/chatServer/chatServer/database.c > CMakeFiles/chatServer.dir/database.c.i

CMakeFiles/chatServer.dir/database.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/chatServer.dir/database.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/wojiaojd/CLionProjects/chatServer/chatServer/database.c -o CMakeFiles/chatServer.dir/database.c.s

CMakeFiles/chatServer.dir/usrData.c.o: CMakeFiles/chatServer.dir/flags.make
CMakeFiles/chatServer.dir/usrData.c.o: ../usrData.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wojiaojd/CLionProjects/chatServer/chatServer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/chatServer.dir/usrData.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/chatServer.dir/usrData.c.o   -c /home/wojiaojd/CLionProjects/chatServer/chatServer/usrData.c

CMakeFiles/chatServer.dir/usrData.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/chatServer.dir/usrData.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/wojiaojd/CLionProjects/chatServer/chatServer/usrData.c > CMakeFiles/chatServer.dir/usrData.c.i

CMakeFiles/chatServer.dir/usrData.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/chatServer.dir/usrData.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/wojiaojd/CLionProjects/chatServer/chatServer/usrData.c -o CMakeFiles/chatServer.dir/usrData.c.s

CMakeFiles/chatServer.dir/sockHandler.c.o: CMakeFiles/chatServer.dir/flags.make
CMakeFiles/chatServer.dir/sockHandler.c.o: ../sockHandler.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wojiaojd/CLionProjects/chatServer/chatServer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/chatServer.dir/sockHandler.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/chatServer.dir/sockHandler.c.o   -c /home/wojiaojd/CLionProjects/chatServer/chatServer/sockHandler.c

CMakeFiles/chatServer.dir/sockHandler.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/chatServer.dir/sockHandler.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/wojiaojd/CLionProjects/chatServer/chatServer/sockHandler.c > CMakeFiles/chatServer.dir/sockHandler.c.i

CMakeFiles/chatServer.dir/sockHandler.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/chatServer.dir/sockHandler.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/wojiaojd/CLionProjects/chatServer/chatServer/sockHandler.c -o CMakeFiles/chatServer.dir/sockHandler.c.s

CMakeFiles/chatServer.dir/rbTree.c.o: CMakeFiles/chatServer.dir/flags.make
CMakeFiles/chatServer.dir/rbTree.c.o: ../rbTree.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wojiaojd/CLionProjects/chatServer/chatServer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/chatServer.dir/rbTree.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/chatServer.dir/rbTree.c.o   -c /home/wojiaojd/CLionProjects/chatServer/chatServer/rbTree.c

CMakeFiles/chatServer.dir/rbTree.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/chatServer.dir/rbTree.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/wojiaojd/CLionProjects/chatServer/chatServer/rbTree.c > CMakeFiles/chatServer.dir/rbTree.c.i

CMakeFiles/chatServer.dir/rbTree.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/chatServer.dir/rbTree.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/wojiaojd/CLionProjects/chatServer/chatServer/rbTree.c -o CMakeFiles/chatServer.dir/rbTree.c.s

# Object files for target chatServer
chatServer_OBJECTS = \
"CMakeFiles/chatServer.dir/main.c.o" \
"CMakeFiles/chatServer.dir/pthreadpool.c.o" \
"CMakeFiles/chatServer.dir/sockqueue.c.o" \
"CMakeFiles/chatServer.dir/errorhandler.c.o" \
"CMakeFiles/chatServer.dir/database.c.o" \
"CMakeFiles/chatServer.dir/usrData.c.o" \
"CMakeFiles/chatServer.dir/sockHandler.c.o" \
"CMakeFiles/chatServer.dir/rbTree.c.o"

# External object files for target chatServer
chatServer_EXTERNAL_OBJECTS =

chatServer: CMakeFiles/chatServer.dir/main.c.o
chatServer: CMakeFiles/chatServer.dir/pthreadpool.c.o
chatServer: CMakeFiles/chatServer.dir/sockqueue.c.o
chatServer: CMakeFiles/chatServer.dir/errorhandler.c.o
chatServer: CMakeFiles/chatServer.dir/database.c.o
chatServer: CMakeFiles/chatServer.dir/usrData.c.o
chatServer: CMakeFiles/chatServer.dir/sockHandler.c.o
chatServer: CMakeFiles/chatServer.dir/rbTree.c.o
chatServer: CMakeFiles/chatServer.dir/build.make
chatServer: CMakeFiles/chatServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wojiaojd/CLionProjects/chatServer/chatServer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking C executable chatServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/chatServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/chatServer.dir/build: chatServer

.PHONY : CMakeFiles/chatServer.dir/build

CMakeFiles/chatServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/chatServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/chatServer.dir/clean

CMakeFiles/chatServer.dir/depend:
	cd /home/wojiaojd/CLionProjects/chatServer/chatServer/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wojiaojd/CLionProjects/chatServer/chatServer /home/wojiaojd/CLionProjects/chatServer/chatServer /home/wojiaojd/CLionProjects/chatServer/chatServer/cmake-build-debug /home/wojiaojd/CLionProjects/chatServer/chatServer/cmake-build-debug /home/wojiaojd/CLionProjects/chatServer/chatServer/cmake-build-debug/CMakeFiles/chatServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/chatServer.dir/depend

