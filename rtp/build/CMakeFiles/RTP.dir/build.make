# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /opt/cmake-3.27.9-linux-x86_64/bin/cmake

# The command to remove a file.
RM = /opt/cmake-3.27.9-linux-x86_64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/james/project/RTPDevice/RTP

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/james/project/RTPDevice/RTP/build

# Include any dependencies generated for this target.
include CMakeFiles/RTP.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/RTP.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/RTP.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RTP.dir/flags.make

CMakeFiles/RTP.dir/Codec/src/VideoCodec.cpp.o: CMakeFiles/RTP.dir/flags.make
CMakeFiles/RTP.dir/Codec/src/VideoCodec.cpp.o: /home/james/project/RTPDevice/RTP/Codec/src/VideoCodec.cpp
CMakeFiles/RTP.dir/Codec/src/VideoCodec.cpp.o: CMakeFiles/RTP.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/james/project/RTPDevice/RTP/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/RTP.dir/Codec/src/VideoCodec.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RTP.dir/Codec/src/VideoCodec.cpp.o -MF CMakeFiles/RTP.dir/Codec/src/VideoCodec.cpp.o.d -o CMakeFiles/RTP.dir/Codec/src/VideoCodec.cpp.o -c /home/james/project/RTPDevice/RTP/Codec/src/VideoCodec.cpp

CMakeFiles/RTP.dir/Codec/src/VideoCodec.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/RTP.dir/Codec/src/VideoCodec.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/james/project/RTPDevice/RTP/Codec/src/VideoCodec.cpp > CMakeFiles/RTP.dir/Codec/src/VideoCodec.cpp.i

CMakeFiles/RTP.dir/Codec/src/VideoCodec.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/RTP.dir/Codec/src/VideoCodec.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/james/project/RTPDevice/RTP/Codec/src/VideoCodec.cpp -o CMakeFiles/RTP.dir/Codec/src/VideoCodec.cpp.s

CMakeFiles/RTP.dir/Codec/src/DepthImageCodec.cpp.o: CMakeFiles/RTP.dir/flags.make
CMakeFiles/RTP.dir/Codec/src/DepthImageCodec.cpp.o: /home/james/project/RTPDevice/RTP/Codec/src/DepthImageCodec.cpp
CMakeFiles/RTP.dir/Codec/src/DepthImageCodec.cpp.o: CMakeFiles/RTP.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/james/project/RTPDevice/RTP/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/RTP.dir/Codec/src/DepthImageCodec.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RTP.dir/Codec/src/DepthImageCodec.cpp.o -MF CMakeFiles/RTP.dir/Codec/src/DepthImageCodec.cpp.o.d -o CMakeFiles/RTP.dir/Codec/src/DepthImageCodec.cpp.o -c /home/james/project/RTPDevice/RTP/Codec/src/DepthImageCodec.cpp

CMakeFiles/RTP.dir/Codec/src/DepthImageCodec.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/RTP.dir/Codec/src/DepthImageCodec.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/james/project/RTPDevice/RTP/Codec/src/DepthImageCodec.cpp > CMakeFiles/RTP.dir/Codec/src/DepthImageCodec.cpp.i

CMakeFiles/RTP.dir/Codec/src/DepthImageCodec.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/RTP.dir/Codec/src/DepthImageCodec.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/james/project/RTPDevice/RTP/Codec/src/DepthImageCodec.cpp -o CMakeFiles/RTP.dir/Codec/src/DepthImageCodec.cpp.s

CMakeFiles/RTP.dir/Codec/src/PointCloudCodec.cpp.o: CMakeFiles/RTP.dir/flags.make
CMakeFiles/RTP.dir/Codec/src/PointCloudCodec.cpp.o: /home/james/project/RTPDevice/RTP/Codec/src/PointCloudCodec.cpp
CMakeFiles/RTP.dir/Codec/src/PointCloudCodec.cpp.o: CMakeFiles/RTP.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/james/project/RTPDevice/RTP/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/RTP.dir/Codec/src/PointCloudCodec.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RTP.dir/Codec/src/PointCloudCodec.cpp.o -MF CMakeFiles/RTP.dir/Codec/src/PointCloudCodec.cpp.o.d -o CMakeFiles/RTP.dir/Codec/src/PointCloudCodec.cpp.o -c /home/james/project/RTPDevice/RTP/Codec/src/PointCloudCodec.cpp

CMakeFiles/RTP.dir/Codec/src/PointCloudCodec.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/RTP.dir/Codec/src/PointCloudCodec.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/james/project/RTPDevice/RTP/Codec/src/PointCloudCodec.cpp > CMakeFiles/RTP.dir/Codec/src/PointCloudCodec.cpp.i

CMakeFiles/RTP.dir/Codec/src/PointCloudCodec.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/RTP.dir/Codec/src/PointCloudCodec.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/james/project/RTPDevice/RTP/Codec/src/PointCloudCodec.cpp -o CMakeFiles/RTP.dir/Codec/src/PointCloudCodec.cpp.s

CMakeFiles/RTP.dir/Codec/src/Data.cpp.o: CMakeFiles/RTP.dir/flags.make
CMakeFiles/RTP.dir/Codec/src/Data.cpp.o: /home/james/project/RTPDevice/RTP/Codec/src/Data.cpp
CMakeFiles/RTP.dir/Codec/src/Data.cpp.o: CMakeFiles/RTP.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/james/project/RTPDevice/RTP/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/RTP.dir/Codec/src/Data.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RTP.dir/Codec/src/Data.cpp.o -MF CMakeFiles/RTP.dir/Codec/src/Data.cpp.o.d -o CMakeFiles/RTP.dir/Codec/src/Data.cpp.o -c /home/james/project/RTPDevice/RTP/Codec/src/Data.cpp

CMakeFiles/RTP.dir/Codec/src/Data.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/RTP.dir/Codec/src/Data.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/james/project/RTPDevice/RTP/Codec/src/Data.cpp > CMakeFiles/RTP.dir/Codec/src/Data.cpp.i

CMakeFiles/RTP.dir/Codec/src/Data.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/RTP.dir/Codec/src/Data.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/james/project/RTPDevice/RTP/Codec/src/Data.cpp -o CMakeFiles/RTP.dir/Codec/src/Data.cpp.s

CMakeFiles/RTP.dir/src/Stream.cpp.o: CMakeFiles/RTP.dir/flags.make
CMakeFiles/RTP.dir/src/Stream.cpp.o: /home/james/project/RTPDevice/RTP/src/Stream.cpp
CMakeFiles/RTP.dir/src/Stream.cpp.o: CMakeFiles/RTP.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/james/project/RTPDevice/RTP/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/RTP.dir/src/Stream.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RTP.dir/src/Stream.cpp.o -MF CMakeFiles/RTP.dir/src/Stream.cpp.o.d -o CMakeFiles/RTP.dir/src/Stream.cpp.o -c /home/james/project/RTPDevice/RTP/src/Stream.cpp

CMakeFiles/RTP.dir/src/Stream.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/RTP.dir/src/Stream.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/james/project/RTPDevice/RTP/src/Stream.cpp > CMakeFiles/RTP.dir/src/Stream.cpp.i

CMakeFiles/RTP.dir/src/Stream.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/RTP.dir/src/Stream.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/james/project/RTPDevice/RTP/src/Stream.cpp -o CMakeFiles/RTP.dir/src/Stream.cpp.s

CMakeFiles/RTP.dir/src/RTPSession.cpp.o: CMakeFiles/RTP.dir/flags.make
CMakeFiles/RTP.dir/src/RTPSession.cpp.o: /home/james/project/RTPDevice/RTP/src/RTPSession.cpp
CMakeFiles/RTP.dir/src/RTPSession.cpp.o: CMakeFiles/RTP.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/james/project/RTPDevice/RTP/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/RTP.dir/src/RTPSession.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RTP.dir/src/RTPSession.cpp.o -MF CMakeFiles/RTP.dir/src/RTPSession.cpp.o.d -o CMakeFiles/RTP.dir/src/RTPSession.cpp.o -c /home/james/project/RTPDevice/RTP/src/RTPSession.cpp

CMakeFiles/RTP.dir/src/RTPSession.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/RTP.dir/src/RTPSession.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/james/project/RTPDevice/RTP/src/RTPSession.cpp > CMakeFiles/RTP.dir/src/RTPSession.cpp.i

CMakeFiles/RTP.dir/src/RTPSession.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/RTP.dir/src/RTPSession.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/james/project/RTPDevice/RTP/src/RTPSession.cpp -o CMakeFiles/RTP.dir/src/RTPSession.cpp.s

# Object files for target RTP
RTP_OBJECTS = \
"CMakeFiles/RTP.dir/Codec/src/VideoCodec.cpp.o" \
"CMakeFiles/RTP.dir/Codec/src/DepthImageCodec.cpp.o" \
"CMakeFiles/RTP.dir/Codec/src/PointCloudCodec.cpp.o" \
"CMakeFiles/RTP.dir/Codec/src/Data.cpp.o" \
"CMakeFiles/RTP.dir/src/Stream.cpp.o" \
"CMakeFiles/RTP.dir/src/RTPSession.cpp.o"

# External object files for target RTP
RTP_EXTERNAL_OBJECTS =

libRTP.a: CMakeFiles/RTP.dir/Codec/src/VideoCodec.cpp.o
libRTP.a: CMakeFiles/RTP.dir/Codec/src/DepthImageCodec.cpp.o
libRTP.a: CMakeFiles/RTP.dir/Codec/src/PointCloudCodec.cpp.o
libRTP.a: CMakeFiles/RTP.dir/Codec/src/Data.cpp.o
libRTP.a: CMakeFiles/RTP.dir/src/Stream.cpp.o
libRTP.a: CMakeFiles/RTP.dir/src/RTPSession.cpp.o
libRTP.a: CMakeFiles/RTP.dir/build.make
libRTP.a: CMakeFiles/RTP.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/james/project/RTPDevice/RTP/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX static library libRTP.a"
	$(CMAKE_COMMAND) -P CMakeFiles/RTP.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RTP.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RTP.dir/build: libRTP.a
.PHONY : CMakeFiles/RTP.dir/build

CMakeFiles/RTP.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RTP.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RTP.dir/clean

CMakeFiles/RTP.dir/depend:
	cd /home/james/project/RTPDevice/RTP/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/james/project/RTPDevice/RTP /home/james/project/RTPDevice/RTP /home/james/project/RTPDevice/RTP/build /home/james/project/RTPDevice/RTP/build /home/james/project/RTPDevice/RTP/build/CMakeFiles/RTP.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/RTP.dir/depend
