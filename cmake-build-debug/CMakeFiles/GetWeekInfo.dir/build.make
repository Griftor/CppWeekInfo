# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /cygdrive/c/Users/Grift/.CLion2018.1/system/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/Grift/.CLion2018.1/system/cygwin_cmake/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cygdrive/c/Users/Grift/Desktop/workStuffs/GetWeekInfo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cygdrive/c/Users/Grift/Desktop/workStuffs/GetWeekInfo/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/GetWeekInfo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/GetWeekInfo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/GetWeekInfo.dir/flags.make

CMakeFiles/GetWeekInfo.dir/main.cpp.o: CMakeFiles/GetWeekInfo.dir/flags.make
CMakeFiles/GetWeekInfo.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/Users/Grift/Desktop/workStuffs/GetWeekInfo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/GetWeekInfo.dir/main.cpp.o"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/GetWeekInfo.dir/main.cpp.o -c /cygdrive/c/Users/Grift/Desktop/workStuffs/GetWeekInfo/main.cpp

CMakeFiles/GetWeekInfo.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GetWeekInfo.dir/main.cpp.i"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /cygdrive/c/Users/Grift/Desktop/workStuffs/GetWeekInfo/main.cpp > CMakeFiles/GetWeekInfo.dir/main.cpp.i

CMakeFiles/GetWeekInfo.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GetWeekInfo.dir/main.cpp.s"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /cygdrive/c/Users/Grift/Desktop/workStuffs/GetWeekInfo/main.cpp -o CMakeFiles/GetWeekInfo.dir/main.cpp.s

CMakeFiles/GetWeekInfo.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/GetWeekInfo.dir/main.cpp.o.requires

CMakeFiles/GetWeekInfo.dir/main.cpp.o.provides: CMakeFiles/GetWeekInfo.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/GetWeekInfo.dir/build.make CMakeFiles/GetWeekInfo.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/GetWeekInfo.dir/main.cpp.o.provides

CMakeFiles/GetWeekInfo.dir/main.cpp.o.provides.build: CMakeFiles/GetWeekInfo.dir/main.cpp.o


# Object files for target GetWeekInfo
GetWeekInfo_OBJECTS = \
"CMakeFiles/GetWeekInfo.dir/main.cpp.o"

# External object files for target GetWeekInfo
GetWeekInfo_EXTERNAL_OBJECTS =

GetWeekInfo.exe: CMakeFiles/GetWeekInfo.dir/main.cpp.o
GetWeekInfo.exe: CMakeFiles/GetWeekInfo.dir/build.make
GetWeekInfo.exe: CMakeFiles/GetWeekInfo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cygdrive/c/Users/Grift/Desktop/workStuffs/GetWeekInfo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable GetWeekInfo.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GetWeekInfo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/GetWeekInfo.dir/build: GetWeekInfo.exe

.PHONY : CMakeFiles/GetWeekInfo.dir/build

CMakeFiles/GetWeekInfo.dir/requires: CMakeFiles/GetWeekInfo.dir/main.cpp.o.requires

.PHONY : CMakeFiles/GetWeekInfo.dir/requires

CMakeFiles/GetWeekInfo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/GetWeekInfo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/GetWeekInfo.dir/clean

CMakeFiles/GetWeekInfo.dir/depend:
	cd /cygdrive/c/Users/Grift/Desktop/workStuffs/GetWeekInfo/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cygdrive/c/Users/Grift/Desktop/workStuffs/GetWeekInfo /cygdrive/c/Users/Grift/Desktop/workStuffs/GetWeekInfo /cygdrive/c/Users/Grift/Desktop/workStuffs/GetWeekInfo/cmake-build-debug /cygdrive/c/Users/Grift/Desktop/workStuffs/GetWeekInfo/cmake-build-debug /cygdrive/c/Users/Grift/Desktop/workStuffs/GetWeekInfo/cmake-build-debug/CMakeFiles/GetWeekInfo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/GetWeekInfo.dir/depend

