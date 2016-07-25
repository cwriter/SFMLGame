# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = /home/silvan/Programming/SFGremote/SFMLGame

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/silvan/Programming/SFGremote/SFMLGame

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/cmake-gui -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/silvan/Programming/SFGremote/SFMLGame/CMakeFiles /home/silvan/Programming/SFGremote/SFMLGame/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/silvan/Programming/SFGremote/SFMLGame/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named Game

# Build rule for target.
Game: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 Game
.PHONY : Game

# fast build rule for target.
Game/fast:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/build
.PHONY : Game/fast

src/Camera.o: src/Camera.cpp.o

.PHONY : src/Camera.o

# target to build an object file
src/Camera.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Camera.cpp.o
.PHONY : src/Camera.cpp.o

src/Camera.i: src/Camera.cpp.i

.PHONY : src/Camera.i

# target to preprocess a source file
src/Camera.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Camera.cpp.i
.PHONY : src/Camera.cpp.i

src/Camera.s: src/Camera.cpp.s

.PHONY : src/Camera.s

# target to generate assembly for a file
src/Camera.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Camera.cpp.s
.PHONY : src/Camera.cpp.s

src/Context.o: src/Context.cpp.o

.PHONY : src/Context.o

# target to build an object file
src/Context.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Context.cpp.o
.PHONY : src/Context.cpp.o

src/Context.i: src/Context.cpp.i

.PHONY : src/Context.i

# target to preprocess a source file
src/Context.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Context.cpp.i
.PHONY : src/Context.cpp.i

src/Context.s: src/Context.cpp.s

.PHONY : src/Context.s

# target to generate assembly for a file
src/Context.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Context.cpp.s
.PHONY : src/Context.cpp.s

src/DLL.o: src/DLL.cpp.o

.PHONY : src/DLL.o

# target to build an object file
src/DLL.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/DLL.cpp.o
.PHONY : src/DLL.cpp.o

src/DLL.i: src/DLL.cpp.i

.PHONY : src/DLL.i

# target to preprocess a source file
src/DLL.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/DLL.cpp.i
.PHONY : src/DLL.cpp.i

src/DLL.s: src/DLL.cpp.s

.PHONY : src/DLL.s

# target to generate assembly for a file
src/DLL.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/DLL.cpp.s
.PHONY : src/DLL.cpp.s

src/DLLWrapper.o: src/DLLWrapper.cpp.o

.PHONY : src/DLLWrapper.o

# target to build an object file
src/DLLWrapper.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/DLLWrapper.cpp.o
.PHONY : src/DLLWrapper.cpp.o

src/DLLWrapper.i: src/DLLWrapper.cpp.i

.PHONY : src/DLLWrapper.i

# target to preprocess a source file
src/DLLWrapper.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/DLLWrapper.cpp.i
.PHONY : src/DLLWrapper.cpp.i

src/DLLWrapper.s: src/DLLWrapper.cpp.s

.PHONY : src/DLLWrapper.s

# target to generate assembly for a file
src/DLLWrapper.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/DLLWrapper.cpp.s
.PHONY : src/DLLWrapper.cpp.s

src/DebugWindow.o: src/DebugWindow.cpp.o

.PHONY : src/DebugWindow.o

# target to build an object file
src/DebugWindow.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/DebugWindow.cpp.o
.PHONY : src/DebugWindow.cpp.o

src/DebugWindow.i: src/DebugWindow.cpp.i

.PHONY : src/DebugWindow.i

# target to preprocess a source file
src/DebugWindow.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/DebugWindow.cpp.i
.PHONY : src/DebugWindow.cpp.i

src/DebugWindow.s: src/DebugWindow.cpp.s

.PHONY : src/DebugWindow.s

# target to generate assembly for a file
src/DebugWindow.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/DebugWindow.cpp.s
.PHONY : src/DebugWindow.cpp.s

src/ExtensionInterface.o: src/ExtensionInterface.cpp.o

.PHONY : src/ExtensionInterface.o

# target to build an object file
src/ExtensionInterface.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/ExtensionInterface.cpp.o
.PHONY : src/ExtensionInterface.cpp.o

src/ExtensionInterface.i: src/ExtensionInterface.cpp.i

.PHONY : src/ExtensionInterface.i

# target to preprocess a source file
src/ExtensionInterface.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/ExtensionInterface.cpp.i
.PHONY : src/ExtensionInterface.cpp.i

src/ExtensionInterface.s: src/ExtensionInterface.cpp.s

.PHONY : src/ExtensionInterface.s

# target to generate assembly for a file
src/ExtensionInterface.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/ExtensionInterface.cpp.s
.PHONY : src/ExtensionInterface.cpp.s

src/Game.o: src/Game.cpp.o

.PHONY : src/Game.o

# target to build an object file
src/Game.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Game.cpp.o
.PHONY : src/Game.cpp.o

src/Game.i: src/Game.cpp.i

.PHONY : src/Game.i

# target to preprocess a source file
src/Game.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Game.cpp.i
.PHONY : src/Game.cpp.i

src/Game.s: src/Game.cpp.s

.PHONY : src/Game.s

# target to generate assembly for a file
src/Game.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Game.cpp.s
.PHONY : src/Game.cpp.s

src/GameSettings.o: src/GameSettings.cpp.o

.PHONY : src/GameSettings.o

# target to build an object file
src/GameSettings.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/GameSettings.cpp.o
.PHONY : src/GameSettings.cpp.o

src/GameSettings.i: src/GameSettings.cpp.i

.PHONY : src/GameSettings.i

# target to preprocess a source file
src/GameSettings.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/GameSettings.cpp.i
.PHONY : src/GameSettings.cpp.i

src/GameSettings.s: src/GameSettings.cpp.s

.PHONY : src/GameSettings.s

# target to generate assembly for a file
src/GameSettings.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/GameSettings.cpp.s
.PHONY : src/GameSettings.cpp.s

src/GameState.o: src/GameState.cpp.o

.PHONY : src/GameState.o

# target to build an object file
src/GameState.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/GameState.cpp.o
.PHONY : src/GameState.cpp.o

src/GameState.i: src/GameState.cpp.i

.PHONY : src/GameState.i

# target to preprocess a source file
src/GameState.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/GameState.cpp.i
.PHONY : src/GameState.cpp.i

src/GameState.s: src/GameState.cpp.s

.PHONY : src/GameState.s

# target to generate assembly for a file
src/GameState.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/GameState.cpp.s
.PHONY : src/GameState.cpp.s

src/KeyMapper.o: src/KeyMapper.cpp.o

.PHONY : src/KeyMapper.o

# target to build an object file
src/KeyMapper.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/KeyMapper.cpp.o
.PHONY : src/KeyMapper.cpp.o

src/KeyMapper.i: src/KeyMapper.cpp.i

.PHONY : src/KeyMapper.i

# target to preprocess a source file
src/KeyMapper.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/KeyMapper.cpp.i
.PHONY : src/KeyMapper.cpp.i

src/KeyMapper.s: src/KeyMapper.cpp.s

.PHONY : src/KeyMapper.s

# target to generate assembly for a file
src/KeyMapper.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/KeyMapper.cpp.s
.PHONY : src/KeyMapper.cpp.s

src/Lighting.o: src/Lighting.cpp.o

.PHONY : src/Lighting.o

# target to build an object file
src/Lighting.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Lighting.cpp.o
.PHONY : src/Lighting.cpp.o

src/Lighting.i: src/Lighting.cpp.i

.PHONY : src/Lighting.i

# target to preprocess a source file
src/Lighting.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Lighting.cpp.i
.PHONY : src/Lighting.cpp.i

src/Lighting.s: src/Lighting.cpp.s

.PHONY : src/Lighting.s

# target to generate assembly for a file
src/Lighting.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Lighting.cpp.s
.PHONY : src/Lighting.cpp.s

src/Message.o: src/Message.cpp.o

.PHONY : src/Message.o

# target to build an object file
src/Message.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Message.cpp.o
.PHONY : src/Message.cpp.o

src/Message.i: src/Message.cpp.i

.PHONY : src/Message.i

# target to preprocess a source file
src/Message.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Message.cpp.i
.PHONY : src/Message.cpp.i

src/Message.s: src/Message.cpp.s

.PHONY : src/Message.s

# target to generate assembly for a file
src/Message.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Message.cpp.s
.PHONY : src/Message.cpp.s

src/Network.o: src/Network.cpp.o

.PHONY : src/Network.o

# target to build an object file
src/Network.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Network.cpp.o
.PHONY : src/Network.cpp.o

src/Network.i: src/Network.cpp.i

.PHONY : src/Network.i

# target to preprocess a source file
src/Network.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Network.cpp.i
.PHONY : src/Network.cpp.i

src/Network.s: src/Network.cpp.s

.PHONY : src/Network.s

# target to generate assembly for a file
src/Network.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Network.cpp.s
.PHONY : src/Network.cpp.s

src/Object.o: src/Object.cpp.o

.PHONY : src/Object.o

# target to build an object file
src/Object.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Object.cpp.o
.PHONY : src/Object.cpp.o

src/Object.i: src/Object.cpp.i

.PHONY : src/Object.i

# target to preprocess a source file
src/Object.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Object.cpp.i
.PHONY : src/Object.cpp.i

src/Object.s: src/Object.cpp.s

.PHONY : src/Object.s

# target to generate assembly for a file
src/Object.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Object.cpp.s
.PHONY : src/Object.cpp.s

src/Physics.o: src/Physics.cpp.o

.PHONY : src/Physics.o

# target to build an object file
src/Physics.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Physics.cpp.o
.PHONY : src/Physics.cpp.o

src/Physics.i: src/Physics.cpp.i

.PHONY : src/Physics.i

# target to preprocess a source file
src/Physics.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Physics.cpp.i
.PHONY : src/Physics.cpp.i

src/Physics.s: src/Physics.cpp.s

.PHONY : src/Physics.s

# target to generate assembly for a file
src/Physics.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Physics.cpp.s
.PHONY : src/Physics.cpp.s

src/Pointer.o: src/Pointer.cpp.o

.PHONY : src/Pointer.o

# target to build an object file
src/Pointer.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Pointer.cpp.o
.PHONY : src/Pointer.cpp.o

src/Pointer.i: src/Pointer.cpp.i

.PHONY : src/Pointer.i

# target to preprocess a source file
src/Pointer.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Pointer.cpp.i
.PHONY : src/Pointer.cpp.i

src/Pointer.s: src/Pointer.cpp.s

.PHONY : src/Pointer.s

# target to generate assembly for a file
src/Pointer.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Pointer.cpp.s
.PHONY : src/Pointer.cpp.s

src/Polygon.o: src/Polygon.cpp.o

.PHONY : src/Polygon.o

# target to build an object file
src/Polygon.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Polygon.cpp.o
.PHONY : src/Polygon.cpp.o

src/Polygon.i: src/Polygon.cpp.i

.PHONY : src/Polygon.i

# target to preprocess a source file
src/Polygon.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Polygon.cpp.i
.PHONY : src/Polygon.cpp.i

src/Polygon.s: src/Polygon.cpp.s

.PHONY : src/Polygon.s

# target to generate assembly for a file
src/Polygon.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Polygon.cpp.s
.PHONY : src/Polygon.cpp.s

src/Profiler.o: src/Profiler.cpp.o

.PHONY : src/Profiler.o

# target to build an object file
src/Profiler.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Profiler.cpp.o
.PHONY : src/Profiler.cpp.o

src/Profiler.i: src/Profiler.cpp.i

.PHONY : src/Profiler.i

# target to preprocess a source file
src/Profiler.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Profiler.cpp.i
.PHONY : src/Profiler.cpp.i

src/Profiler.s: src/Profiler.cpp.s

.PHONY : src/Profiler.s

# target to generate assembly for a file
src/Profiler.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Profiler.cpp.s
.PHONY : src/Profiler.cpp.s

src/Rect.o: src/Rect.cpp.o

.PHONY : src/Rect.o

# target to build an object file
src/Rect.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Rect.cpp.o
.PHONY : src/Rect.cpp.o

src/Rect.i: src/Rect.cpp.i

.PHONY : src/Rect.i

# target to preprocess a source file
src/Rect.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Rect.cpp.i
.PHONY : src/Rect.cpp.i

src/Rect.s: src/Rect.cpp.s

.PHONY : src/Rect.s

# target to generate assembly for a file
src/Rect.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Rect.cpp.s
.PHONY : src/Rect.cpp.s

src/SOW_AI.o: src/SOW_AI.cpp.o

.PHONY : src/SOW_AI.o

# target to build an object file
src/SOW_AI.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SOW_AI.cpp.o
.PHONY : src/SOW_AI.cpp.o

src/SOW_AI.i: src/SOW_AI.cpp.i

.PHONY : src/SOW_AI.i

# target to preprocess a source file
src/SOW_AI.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SOW_AI.cpp.i
.PHONY : src/SOW_AI.cpp.i

src/SOW_AI.s: src/SOW_AI.cpp.s

.PHONY : src/SOW_AI.s

# target to generate assembly for a file
src/SOW_AI.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SOW_AI.cpp.s
.PHONY : src/SOW_AI.cpp.s

src/SOW_Car.o: src/SOW_Car.cpp.o

.PHONY : src/SOW_Car.o

# target to build an object file
src/SOW_Car.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SOW_Car.cpp.o
.PHONY : src/SOW_Car.cpp.o

src/SOW_Car.i: src/SOW_Car.cpp.i

.PHONY : src/SOW_Car.i

# target to preprocess a source file
src/SOW_Car.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SOW_Car.cpp.i
.PHONY : src/SOW_Car.cpp.i

src/SOW_Car.s: src/SOW_Car.cpp.s

.PHONY : src/SOW_Car.s

# target to generate assembly for a file
src/SOW_Car.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SOW_Car.cpp.s
.PHONY : src/SOW_Car.cpp.s

src/SOW_Game.o: src/SOW_Game.cpp.o

.PHONY : src/SOW_Game.o

# target to build an object file
src/SOW_Game.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SOW_Game.cpp.o
.PHONY : src/SOW_Game.cpp.o

src/SOW_Game.i: src/SOW_Game.cpp.i

.PHONY : src/SOW_Game.i

# target to preprocess a source file
src/SOW_Game.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SOW_Game.cpp.i
.PHONY : src/SOW_Game.cpp.i

src/SOW_Game.s: src/SOW_Game.cpp.s

.PHONY : src/SOW_Game.s

# target to generate assembly for a file
src/SOW_Game.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SOW_Game.cpp.s
.PHONY : src/SOW_Game.cpp.s

src/SOW_Road.o: src/SOW_Road.cpp.o

.PHONY : src/SOW_Road.o

# target to build an object file
src/SOW_Road.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SOW_Road.cpp.o
.PHONY : src/SOW_Road.cpp.o

src/SOW_Road.i: src/SOW_Road.cpp.i

.PHONY : src/SOW_Road.i

# target to preprocess a source file
src/SOW_Road.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SOW_Road.cpp.i
.PHONY : src/SOW_Road.cpp.i

src/SOW_Road.s: src/SOW_Road.cpp.s

.PHONY : src/SOW_Road.s

# target to generate assembly for a file
src/SOW_Road.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SOW_Road.cpp.s
.PHONY : src/SOW_Road.cpp.s

src/SOW_World.o: src/SOW_World.cpp.o

.PHONY : src/SOW_World.o

# target to build an object file
src/SOW_World.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SOW_World.cpp.o
.PHONY : src/SOW_World.cpp.o

src/SOW_World.i: src/SOW_World.cpp.i

.PHONY : src/SOW_World.i

# target to preprocess a source file
src/SOW_World.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SOW_World.cpp.i
.PHONY : src/SOW_World.cpp.i

src/SOW_World.s: src/SOW_World.cpp.s

.PHONY : src/SOW_World.s

# target to generate assembly for a file
src/SOW_World.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SOW_World.cpp.s
.PHONY : src/SOW_World.cpp.s

src/SOW_main.o: src/SOW_main.cpp.o

.PHONY : src/SOW_main.o

# target to build an object file
src/SOW_main.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SOW_main.cpp.o
.PHONY : src/SOW_main.cpp.o

src/SOW_main.i: src/SOW_main.cpp.i

.PHONY : src/SOW_main.i

# target to preprocess a source file
src/SOW_main.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SOW_main.cpp.i
.PHONY : src/SOW_main.cpp.i

src/SOW_main.s: src/SOW_main.cpp.s

.PHONY : src/SOW_main.s

# target to generate assembly for a file
src/SOW_main.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SOW_main.cpp.s
.PHONY : src/SOW_main.cpp.s

src/SSG_BuildOverlay.o: src/SSG_BuildOverlay.cpp.o

.PHONY : src/SSG_BuildOverlay.o

# target to build an object file
src/SSG_BuildOverlay.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_BuildOverlay.cpp.o
.PHONY : src/SSG_BuildOverlay.cpp.o

src/SSG_BuildOverlay.i: src/SSG_BuildOverlay.cpp.i

.PHONY : src/SSG_BuildOverlay.i

# target to preprocess a source file
src/SSG_BuildOverlay.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_BuildOverlay.cpp.i
.PHONY : src/SSG_BuildOverlay.cpp.i

src/SSG_BuildOverlay.s: src/SSG_BuildOverlay.cpp.s

.PHONY : src/SSG_BuildOverlay.s

# target to generate assembly for a file
src/SSG_BuildOverlay.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_BuildOverlay.cpp.s
.PHONY : src/SSG_BuildOverlay.cpp.s

src/SSG_Galaxy.o: src/SSG_Galaxy.cpp.o

.PHONY : src/SSG_Galaxy.o

# target to build an object file
src/SSG_Galaxy.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_Galaxy.cpp.o
.PHONY : src/SSG_Galaxy.cpp.o

src/SSG_Galaxy.i: src/SSG_Galaxy.cpp.i

.PHONY : src/SSG_Galaxy.i

# target to preprocess a source file
src/SSG_Galaxy.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_Galaxy.cpp.i
.PHONY : src/SSG_Galaxy.cpp.i

src/SSG_Galaxy.s: src/SSG_Galaxy.cpp.s

.PHONY : src/SSG_Galaxy.s

# target to generate assembly for a file
src/SSG_Galaxy.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_Galaxy.cpp.s
.PHONY : src/SSG_Galaxy.cpp.s

src/SSG_Game.o: src/SSG_Game.cpp.o

.PHONY : src/SSG_Game.o

# target to build an object file
src/SSG_Game.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_Game.cpp.o
.PHONY : src/SSG_Game.cpp.o

src/SSG_Game.i: src/SSG_Game.cpp.i

.PHONY : src/SSG_Game.i

# target to preprocess a source file
src/SSG_Game.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_Game.cpp.i
.PHONY : src/SSG_Game.cpp.i

src/SSG_Game.s: src/SSG_Game.cpp.s

.PHONY : src/SSG_Game.s

# target to generate assembly for a file
src/SSG_Game.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_Game.cpp.s
.PHONY : src/SSG_Game.cpp.s

src/SSG_Part.o: src/SSG_Part.cpp.o

.PHONY : src/SSG_Part.o

# target to build an object file
src/SSG_Part.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_Part.cpp.o
.PHONY : src/SSG_Part.cpp.o

src/SSG_Part.i: src/SSG_Part.cpp.i

.PHONY : src/SSG_Part.i

# target to preprocess a source file
src/SSG_Part.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_Part.cpp.i
.PHONY : src/SSG_Part.cpp.i

src/SSG_Part.s: src/SSG_Part.cpp.s

.PHONY : src/SSG_Part.s

# target to generate assembly for a file
src/SSG_Part.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_Part.cpp.s
.PHONY : src/SSG_Part.cpp.s

src/SSG_Planet.o: src/SSG_Planet.cpp.o

.PHONY : src/SSG_Planet.o

# target to build an object file
src/SSG_Planet.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_Planet.cpp.o
.PHONY : src/SSG_Planet.cpp.o

src/SSG_Planet.i: src/SSG_Planet.cpp.i

.PHONY : src/SSG_Planet.i

# target to preprocess a source file
src/SSG_Planet.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_Planet.cpp.i
.PHONY : src/SSG_Planet.cpp.i

src/SSG_Planet.s: src/SSG_Planet.cpp.s

.PHONY : src/SSG_Planet.s

# target to generate assembly for a file
src/SSG_Planet.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_Planet.cpp.s
.PHONY : src/SSG_Planet.cpp.s

src/SSG_Ship.o: src/SSG_Ship.cpp.o

.PHONY : src/SSG_Ship.o

# target to build an object file
src/SSG_Ship.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_Ship.cpp.o
.PHONY : src/SSG_Ship.cpp.o

src/SSG_Ship.i: src/SSG_Ship.cpp.i

.PHONY : src/SSG_Ship.i

# target to preprocess a source file
src/SSG_Ship.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_Ship.cpp.i
.PHONY : src/SSG_Ship.cpp.i

src/SSG_Ship.s: src/SSG_Ship.cpp.s

.PHONY : src/SSG_Ship.s

# target to generate assembly for a file
src/SSG_Ship.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_Ship.cpp.s
.PHONY : src/SSG_Ship.cpp.s

src/SSG_SolarSystem.o: src/SSG_SolarSystem.cpp.o

.PHONY : src/SSG_SolarSystem.o

# target to build an object file
src/SSG_SolarSystem.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_SolarSystem.cpp.o
.PHONY : src/SSG_SolarSystem.cpp.o

src/SSG_SolarSystem.i: src/SSG_SolarSystem.cpp.i

.PHONY : src/SSG_SolarSystem.i

# target to preprocess a source file
src/SSG_SolarSystem.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_SolarSystem.cpp.i
.PHONY : src/SSG_SolarSystem.cpp.i

src/SSG_SolarSystem.s: src/SSG_SolarSystem.cpp.s

.PHONY : src/SSG_SolarSystem.s

# target to generate assembly for a file
src/SSG_SolarSystem.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_SolarSystem.cpp.s
.PHONY : src/SSG_SolarSystem.cpp.s

src/SSG_Universe.o: src/SSG_Universe.cpp.o

.PHONY : src/SSG_Universe.o

# target to build an object file
src/SSG_Universe.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_Universe.cpp.o
.PHONY : src/SSG_Universe.cpp.o

src/SSG_Universe.i: src/SSG_Universe.cpp.i

.PHONY : src/SSG_Universe.i

# target to preprocess a source file
src/SSG_Universe.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_Universe.cpp.i
.PHONY : src/SSG_Universe.cpp.i

src/SSG_Universe.s: src/SSG_Universe.cpp.s

.PHONY : src/SSG_Universe.s

# target to generate assembly for a file
src/SSG_Universe.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_Universe.cpp.s
.PHONY : src/SSG_Universe.cpp.s

src/SSG_main.o: src/SSG_main.cpp.o

.PHONY : src/SSG_main.o

# target to build an object file
src/SSG_main.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_main.cpp.o
.PHONY : src/SSG_main.cpp.o

src/SSG_main.i: src/SSG_main.cpp.i

.PHONY : src/SSG_main.i

# target to preprocess a source file
src/SSG_main.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_main.cpp.i
.PHONY : src/SSG_main.cpp.i

src/SSG_main.s: src/SSG_main.cpp.s

.PHONY : src/SSG_main.s

# target to generate assembly for a file
src/SSG_main.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/SSG_main.cpp.s
.PHONY : src/SSG_main.cpp.s

src/StringManager.o: src/StringManager.cpp.o

.PHONY : src/StringManager.o

# target to build an object file
src/StringManager.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/StringManager.cpp.o
.PHONY : src/StringManager.cpp.o

src/StringManager.i: src/StringManager.cpp.i

.PHONY : src/StringManager.i

# target to preprocess a source file
src/StringManager.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/StringManager.cpp.i
.PHONY : src/StringManager.cpp.i

src/StringManager.s: src/StringManager.cpp.s

.PHONY : src/StringManager.s

# target to generate assembly for a file
src/StringManager.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/StringManager.cpp.s
.PHONY : src/StringManager.cpp.s

src/System.o: src/System.cpp.o

.PHONY : src/System.o

# target to build an object file
src/System.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/System.cpp.o
.PHONY : src/System.cpp.o

src/System.i: src/System.cpp.i

.PHONY : src/System.i

# target to preprocess a source file
src/System.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/System.cpp.i
.PHONY : src/System.cpp.i

src/System.s: src/System.cpp.s

.PHONY : src/System.s

# target to generate assembly for a file
src/System.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/System.cpp.s
.PHONY : src/System.cpp.s

src/TopView.o: src/TopView.cpp.o

.PHONY : src/TopView.o

# target to build an object file
src/TopView.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/TopView.cpp.o
.PHONY : src/TopView.cpp.o

src/TopView.i: src/TopView.cpp.i

.PHONY : src/TopView.i

# target to preprocess a source file
src/TopView.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/TopView.cpp.i
.PHONY : src/TopView.cpp.i

src/TopView.s: src/TopView.cpp.s

.PHONY : src/TopView.s

# target to generate assembly for a file
src/TopView.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/TopView.cpp.s
.PHONY : src/TopView.cpp.s

src/UI.o: src/UI.cpp.o

.PHONY : src/UI.o

# target to build an object file
src/UI.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/UI.cpp.o
.PHONY : src/UI.cpp.o

src/UI.i: src/UI.cpp.i

.PHONY : src/UI.i

# target to preprocess a source file
src/UI.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/UI.cpp.i
.PHONY : src/UI.cpp.i

src/UI.s: src/UI.cpp.s

.PHONY : src/UI.s

# target to generate assembly for a file
src/UI.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/UI.cpp.s
.PHONY : src/UI.cpp.s

src/Util.o: src/Util.cpp.o

.PHONY : src/Util.o

# target to build an object file
src/Util.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Util.cpp.o
.PHONY : src/Util.cpp.o

src/Util.i: src/Util.cpp.i

.PHONY : src/Util.i

# target to preprocess a source file
src/Util.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Util.cpp.i
.PHONY : src/Util.cpp.i

src/Util.s: src/Util.cpp.s

.PHONY : src/Util.s

# target to generate assembly for a file
src/Util.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Util.cpp.s
.PHONY : src/Util.cpp.s

src/Window.o: src/Window.cpp.o

.PHONY : src/Window.o

# target to build an object file
src/Window.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Window.cpp.o
.PHONY : src/Window.cpp.o

src/Window.i: src/Window.cpp.i

.PHONY : src/Window.i

# target to preprocess a source file
src/Window.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Window.cpp.i
.PHONY : src/Window.cpp.i

src/Window.s: src/Window.cpp.s

.PHONY : src/Window.s

# target to generate assembly for a file
src/Window.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/Window.cpp.s
.PHONY : src/Window.cpp.s

src/XML.o: src/XML.cpp.o

.PHONY : src/XML.o

# target to build an object file
src/XML.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/XML.cpp.o
.PHONY : src/XML.cpp.o

src/XML.i: src/XML.cpp.i

.PHONY : src/XML.i

# target to preprocess a source file
src/XML.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/XML.cpp.i
.PHONY : src/XML.cpp.i

src/XML.s: src/XML.cpp.s

.PHONY : src/XML.s

# target to generate assembly for a file
src/XML.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/XML.cpp.s
.PHONY : src/XML.cpp.s

src/main.o: src/main.cpp.o

.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i

.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s

.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

src/stdafx.o: src/stdafx.cpp.o

.PHONY : src/stdafx.o

# target to build an object file
src/stdafx.cpp.o:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/stdafx.cpp.o
.PHONY : src/stdafx.cpp.o

src/stdafx.i: src/stdafx.cpp.i

.PHONY : src/stdafx.i

# target to preprocess a source file
src/stdafx.cpp.i:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/stdafx.cpp.i
.PHONY : src/stdafx.cpp.i

src/stdafx.s: src/stdafx.cpp.s

.PHONY : src/stdafx.s

# target to generate assembly for a file
src/stdafx.cpp.s:
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/stdafx.cpp.s
.PHONY : src/stdafx.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... Game"
	@echo "... src/Camera.o"
	@echo "... src/Camera.i"
	@echo "... src/Camera.s"
	@echo "... src/Context.o"
	@echo "... src/Context.i"
	@echo "... src/Context.s"
	@echo "... src/DLL.o"
	@echo "... src/DLL.i"
	@echo "... src/DLL.s"
	@echo "... src/DLLWrapper.o"
	@echo "... src/DLLWrapper.i"
	@echo "... src/DLLWrapper.s"
	@echo "... src/DebugWindow.o"
	@echo "... src/DebugWindow.i"
	@echo "... src/DebugWindow.s"
	@echo "... src/ExtensionInterface.o"
	@echo "... src/ExtensionInterface.i"
	@echo "... src/ExtensionInterface.s"
	@echo "... src/Game.o"
	@echo "... src/Game.i"
	@echo "... src/Game.s"
	@echo "... src/GameSettings.o"
	@echo "... src/GameSettings.i"
	@echo "... src/GameSettings.s"
	@echo "... src/GameState.o"
	@echo "... src/GameState.i"
	@echo "... src/GameState.s"
	@echo "... src/KeyMapper.o"
	@echo "... src/KeyMapper.i"
	@echo "... src/KeyMapper.s"
	@echo "... src/Lighting.o"
	@echo "... src/Lighting.i"
	@echo "... src/Lighting.s"
	@echo "... src/Message.o"
	@echo "... src/Message.i"
	@echo "... src/Message.s"
	@echo "... src/Network.o"
	@echo "... src/Network.i"
	@echo "... src/Network.s"
	@echo "... src/Object.o"
	@echo "... src/Object.i"
	@echo "... src/Object.s"
	@echo "... src/Physics.o"
	@echo "... src/Physics.i"
	@echo "... src/Physics.s"
	@echo "... src/Pointer.o"
	@echo "... src/Pointer.i"
	@echo "... src/Pointer.s"
	@echo "... src/Polygon.o"
	@echo "... src/Polygon.i"
	@echo "... src/Polygon.s"
	@echo "... src/Profiler.o"
	@echo "... src/Profiler.i"
	@echo "... src/Profiler.s"
	@echo "... src/Rect.o"
	@echo "... src/Rect.i"
	@echo "... src/Rect.s"
	@echo "... src/SOW_AI.o"
	@echo "... src/SOW_AI.i"
	@echo "... src/SOW_AI.s"
	@echo "... src/SOW_Car.o"
	@echo "... src/SOW_Car.i"
	@echo "... src/SOW_Car.s"
	@echo "... src/SOW_Game.o"
	@echo "... src/SOW_Game.i"
	@echo "... src/SOW_Game.s"
	@echo "... src/SOW_Road.o"
	@echo "... src/SOW_Road.i"
	@echo "... src/SOW_Road.s"
	@echo "... src/SOW_World.o"
	@echo "... src/SOW_World.i"
	@echo "... src/SOW_World.s"
	@echo "... src/SOW_main.o"
	@echo "... src/SOW_main.i"
	@echo "... src/SOW_main.s"
	@echo "... src/SSG_BuildOverlay.o"
	@echo "... src/SSG_BuildOverlay.i"
	@echo "... src/SSG_BuildOverlay.s"
	@echo "... src/SSG_Galaxy.o"
	@echo "... src/SSG_Galaxy.i"
	@echo "... src/SSG_Galaxy.s"
	@echo "... src/SSG_Game.o"
	@echo "... src/SSG_Game.i"
	@echo "... src/SSG_Game.s"
	@echo "... src/SSG_Part.o"
	@echo "... src/SSG_Part.i"
	@echo "... src/SSG_Part.s"
	@echo "... src/SSG_Planet.o"
	@echo "... src/SSG_Planet.i"
	@echo "... src/SSG_Planet.s"
	@echo "... src/SSG_Ship.o"
	@echo "... src/SSG_Ship.i"
	@echo "... src/SSG_Ship.s"
	@echo "... src/SSG_SolarSystem.o"
	@echo "... src/SSG_SolarSystem.i"
	@echo "... src/SSG_SolarSystem.s"
	@echo "... src/SSG_Universe.o"
	@echo "... src/SSG_Universe.i"
	@echo "... src/SSG_Universe.s"
	@echo "... src/SSG_main.o"
	@echo "... src/SSG_main.i"
	@echo "... src/SSG_main.s"
	@echo "... src/StringManager.o"
	@echo "... src/StringManager.i"
	@echo "... src/StringManager.s"
	@echo "... src/System.o"
	@echo "... src/System.i"
	@echo "... src/System.s"
	@echo "... src/TopView.o"
	@echo "... src/TopView.i"
	@echo "... src/TopView.s"
	@echo "... src/UI.o"
	@echo "... src/UI.i"
	@echo "... src/UI.s"
	@echo "... src/Util.o"
	@echo "... src/Util.i"
	@echo "... src/Util.s"
	@echo "... src/Window.o"
	@echo "... src/Window.i"
	@echo "... src/Window.s"
	@echo "... src/XML.o"
	@echo "... src/XML.i"
	@echo "... src/XML.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/stdafx.o"
	@echo "... src/stdafx.i"
	@echo "... src/stdafx.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

