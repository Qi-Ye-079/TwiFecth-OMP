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
CMAKE_SOURCE_DIR = /home/nanaya/OpenMP-SearchTweet

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nanaya/OpenMP-SearchTweet/build/Debug

# Include any dependencies generated for this target.
include CMakeFiles/OpenMP-SearchTweet.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/OpenMP-SearchTweet.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/OpenMP-SearchTweet.dir/flags.make

CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.o: CMakeFiles/OpenMP-SearchTweet.dir/flags.make
CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.o: ../../src/utility.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nanaya/OpenMP-SearchTweet/build/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.o"
	/usr/bin/g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.o -c /home/nanaya/OpenMP-SearchTweet/src/utility.cpp

CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.i"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nanaya/OpenMP-SearchTweet/src/utility.cpp > CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.i

CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.s"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nanaya/OpenMP-SearchTweet/src/utility.cpp -o CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.s

CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.o.requires:

.PHONY : CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.o.requires

CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.o.provides: CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.o.requires
	$(MAKE) -f CMakeFiles/OpenMP-SearchTweet.dir/build.make CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.o.provides.build
.PHONY : CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.o.provides

CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.o.provides.build: CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.o


CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.o: CMakeFiles/OpenMP-SearchTweet.dir/flags.make
CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.o: ../../src/TweetFetcher.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nanaya/OpenMP-SearchTweet/build/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.o"
	/usr/bin/g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.o -c /home/nanaya/OpenMP-SearchTweet/src/TweetFetcher.cpp

CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.i"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nanaya/OpenMP-SearchTweet/src/TweetFetcher.cpp > CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.i

CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.s"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nanaya/OpenMP-SearchTweet/src/TweetFetcher.cpp -o CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.s

CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.o.requires:

.PHONY : CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.o.requires

CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.o.provides: CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.o.requires
	$(MAKE) -f CMakeFiles/OpenMP-SearchTweet.dir/build.make CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.o.provides.build
.PHONY : CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.o.provides

CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.o.provides.build: CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.o


CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.o: CMakeFiles/OpenMP-SearchTweet.dir/flags.make
CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.o: ../../src/example.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nanaya/OpenMP-SearchTweet/build/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.o"
	/usr/bin/g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.o -c /home/nanaya/OpenMP-SearchTweet/src/example.cpp

CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.i"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nanaya/OpenMP-SearchTweet/src/example.cpp > CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.i

CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.s"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nanaya/OpenMP-SearchTweet/src/example.cpp -o CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.s

CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.o.requires:

.PHONY : CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.o.requires

CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.o.provides: CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.o.requires
	$(MAKE) -f CMakeFiles/OpenMP-SearchTweet.dir/build.make CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.o.provides.build
.PHONY : CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.o.provides

CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.o.provides.build: CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.o


# Object files for target OpenMP-SearchTweet
OpenMP__SearchTweet_OBJECTS = \
"CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.o" \
"CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.o" \
"CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.o"

# External object files for target OpenMP-SearchTweet
OpenMP__SearchTweet_EXTERNAL_OBJECTS =

OpenMP-SearchTweet: CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.o
OpenMP-SearchTweet: CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.o
OpenMP-SearchTweet: CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.o
OpenMP-SearchTweet: CMakeFiles/OpenMP-SearchTweet.dir/build.make
OpenMP-SearchTweet: CMakeFiles/OpenMP-SearchTweet.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nanaya/OpenMP-SearchTweet/build/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable OpenMP-SearchTweet"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/OpenMP-SearchTweet.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/OpenMP-SearchTweet.dir/build: OpenMP-SearchTweet

.PHONY : CMakeFiles/OpenMP-SearchTweet.dir/build

CMakeFiles/OpenMP-SearchTweet.dir/requires: CMakeFiles/OpenMP-SearchTweet.dir/src/utility.cpp.o.requires
CMakeFiles/OpenMP-SearchTweet.dir/requires: CMakeFiles/OpenMP-SearchTweet.dir/src/TweetFetcher.cpp.o.requires
CMakeFiles/OpenMP-SearchTweet.dir/requires: CMakeFiles/OpenMP-SearchTweet.dir/src/example.cpp.o.requires

.PHONY : CMakeFiles/OpenMP-SearchTweet.dir/requires

CMakeFiles/OpenMP-SearchTweet.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/OpenMP-SearchTweet.dir/cmake_clean.cmake
.PHONY : CMakeFiles/OpenMP-SearchTweet.dir/clean

CMakeFiles/OpenMP-SearchTweet.dir/depend:
	cd /home/nanaya/OpenMP-SearchTweet/build/Debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nanaya/OpenMP-SearchTweet /home/nanaya/OpenMP-SearchTweet /home/nanaya/OpenMP-SearchTweet/build/Debug /home/nanaya/OpenMP-SearchTweet/build/Debug /home/nanaya/OpenMP-SearchTweet/build/Debug/CMakeFiles/OpenMP-SearchTweet.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/OpenMP-SearchTweet.dir/depend

