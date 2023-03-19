# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jt/my_im/server/src/slog

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jt/my_im/server/src/slog/build

# Include any dependencies generated for this target.
include CMakeFiles/slog.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/slog.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/slog.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/slog.dir/flags.make

CMakeFiles/slog.dir/slog_api.cpp.o: CMakeFiles/slog.dir/flags.make
CMakeFiles/slog.dir/slog_api.cpp.o: ../slog_api.cpp
CMakeFiles/slog.dir/slog_api.cpp.o: CMakeFiles/slog.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/my_im/server/src/slog/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/slog.dir/slog_api.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/slog.dir/slog_api.cpp.o -MF CMakeFiles/slog.dir/slog_api.cpp.o.d -o CMakeFiles/slog.dir/slog_api.cpp.o -c /home/jt/my_im/server/src/slog/slog_api.cpp

CMakeFiles/slog.dir/slog_api.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/slog.dir/slog_api.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/my_im/server/src/slog/slog_api.cpp > CMakeFiles/slog.dir/slog_api.cpp.i

CMakeFiles/slog.dir/slog_api.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/slog.dir/slog_api.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/my_im/server/src/slog/slog_api.cpp -o CMakeFiles/slog.dir/slog_api.cpp.s

# Object files for target slog
slog_OBJECTS = \
"CMakeFiles/slog.dir/slog_api.cpp.o"

# External object files for target slog
slog_EXTERNAL_OBJECTS =

libslog.so: CMakeFiles/slog.dir/slog_api.cpp.o
libslog.so: CMakeFiles/slog.dir/build.make
libslog.so: CMakeFiles/slog.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jt/my_im/server/src/slog/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libslog.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/slog.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/slog.dir/build: libslog.so
.PHONY : CMakeFiles/slog.dir/build

CMakeFiles/slog.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/slog.dir/cmake_clean.cmake
.PHONY : CMakeFiles/slog.dir/clean

CMakeFiles/slog.dir/depend:
	cd /home/jt/my_im/server/src/slog/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jt/my_im/server/src/slog /home/jt/my_im/server/src/slog /home/jt/my_im/server/src/slog/build /home/jt/my_im/server/src/slog/build /home/jt/my_im/server/src/slog/build/CMakeFiles/slog.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/slog.dir/depend

