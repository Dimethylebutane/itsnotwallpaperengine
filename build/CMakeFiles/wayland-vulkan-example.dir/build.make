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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/greg/prgrm/c++/wallpaperVulkan

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/greg/prgrm/c++/wallpaperVulkan/build

# Include any dependencies generated for this target.
include CMakeFiles/wayland-vulkan-example.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/wayland-vulkan-example.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/wayland-vulkan-example.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/wayland-vulkan-example.dir/flags.make

CMakeFiles/wayland-vulkan-example.dir/src/main.cpp.o: CMakeFiles/wayland-vulkan-example.dir/flags.make
CMakeFiles/wayland-vulkan-example.dir/src/main.cpp.o: /home/greg/prgrm/c++/wallpaperVulkan/src/main.cpp
CMakeFiles/wayland-vulkan-example.dir/src/main.cpp.o: CMakeFiles/wayland-vulkan-example.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/greg/prgrm/c++/wallpaperVulkan/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/wayland-vulkan-example.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/wayland-vulkan-example.dir/src/main.cpp.o -MF CMakeFiles/wayland-vulkan-example.dir/src/main.cpp.o.d -o CMakeFiles/wayland-vulkan-example.dir/src/main.cpp.o -c /home/greg/prgrm/c++/wallpaperVulkan/src/main.cpp

CMakeFiles/wayland-vulkan-example.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/wayland-vulkan-example.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/greg/prgrm/c++/wallpaperVulkan/src/main.cpp > CMakeFiles/wayland-vulkan-example.dir/src/main.cpp.i

CMakeFiles/wayland-vulkan-example.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/wayland-vulkan-example.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/greg/prgrm/c++/wallpaperVulkan/src/main.cpp -o CMakeFiles/wayland-vulkan-example.dir/src/main.cpp.s

CMakeFiles/wayland-vulkan-example.dir/src/xdg-shell.c.o: CMakeFiles/wayland-vulkan-example.dir/flags.make
CMakeFiles/wayland-vulkan-example.dir/src/xdg-shell.c.o: /home/greg/prgrm/c++/wallpaperVulkan/src/xdg-shell.c
CMakeFiles/wayland-vulkan-example.dir/src/xdg-shell.c.o: CMakeFiles/wayland-vulkan-example.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/greg/prgrm/c++/wallpaperVulkan/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/wayland-vulkan-example.dir/src/xdg-shell.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/wayland-vulkan-example.dir/src/xdg-shell.c.o -MF CMakeFiles/wayland-vulkan-example.dir/src/xdg-shell.c.o.d -o CMakeFiles/wayland-vulkan-example.dir/src/xdg-shell.c.o -c /home/greg/prgrm/c++/wallpaperVulkan/src/xdg-shell.c

CMakeFiles/wayland-vulkan-example.dir/src/xdg-shell.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/wayland-vulkan-example.dir/src/xdg-shell.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/greg/prgrm/c++/wallpaperVulkan/src/xdg-shell.c > CMakeFiles/wayland-vulkan-example.dir/src/xdg-shell.c.i

CMakeFiles/wayland-vulkan-example.dir/src/xdg-shell.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/wayland-vulkan-example.dir/src/xdg-shell.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/greg/prgrm/c++/wallpaperVulkan/src/xdg-shell.c -o CMakeFiles/wayland-vulkan-example.dir/src/xdg-shell.c.s

# Object files for target wayland-vulkan-example
wayland__vulkan__example_OBJECTS = \
"CMakeFiles/wayland-vulkan-example.dir/src/main.cpp.o" \
"CMakeFiles/wayland-vulkan-example.dir/src/xdg-shell.c.o"

# External object files for target wayland-vulkan-example
wayland__vulkan__example_EXTERNAL_OBJECTS =

libwayland-vulkan-example.so: CMakeFiles/wayland-vulkan-example.dir/src/main.cpp.o
libwayland-vulkan-example.so: CMakeFiles/wayland-vulkan-example.dir/src/xdg-shell.c.o
libwayland-vulkan-example.so: CMakeFiles/wayland-vulkan-example.dir/build.make
libwayland-vulkan-example.so: CMakeFiles/wayland-vulkan-example.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/greg/prgrm/c++/wallpaperVulkan/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library libwayland-vulkan-example.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wayland-vulkan-example.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/wayland-vulkan-example.dir/build: libwayland-vulkan-example.so
.PHONY : CMakeFiles/wayland-vulkan-example.dir/build

CMakeFiles/wayland-vulkan-example.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/wayland-vulkan-example.dir/cmake_clean.cmake
.PHONY : CMakeFiles/wayland-vulkan-example.dir/clean

CMakeFiles/wayland-vulkan-example.dir/depend:
	cd /home/greg/prgrm/c++/wallpaperVulkan/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/greg/prgrm/c++/wallpaperVulkan /home/greg/prgrm/c++/wallpaperVulkan /home/greg/prgrm/c++/wallpaperVulkan/build /home/greg/prgrm/c++/wallpaperVulkan/build /home/greg/prgrm/c++/wallpaperVulkan/build/CMakeFiles/wayland-vulkan-example.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/wayland-vulkan-example.dir/depend

