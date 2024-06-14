# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /home/user/linuxcnc/cmake

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/linuxcnc/cmake/build

# Include any dependencies generated for this target.
include components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/compiler_depend.make

# Include the progress variables for this target.
include components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/progress.make

# Include the compile flags for this target's objects.
include components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/flags.make

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/kinematics/cubic.c.o: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/flags.make
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/kinematics/cubic.c.o: /home/user/linuxcnc/src/emc/kinematics/cubic.c
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/kinematics/cubic.c.o: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/linuxcnc/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/kinematics/cubic.c.o"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/kinematics/cubic.c.o -MF CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/kinematics/cubic.c.o.d -o CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/kinematics/cubic.c.o -c /home/user/linuxcnc/src/emc/kinematics/cubic.c

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/kinematics/cubic.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/kinematics/cubic.c.i"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/linuxcnc/src/emc/kinematics/cubic.c > CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/kinematics/cubic.c.i

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/kinematics/cubic.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/kinematics/cubic.c.s"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/linuxcnc/src/emc/kinematics/cubic.c -o CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/kinematics/cubic.c.s

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/axis.c.o: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/flags.make
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/axis.c.o: /home/user/linuxcnc/src/emc/motion/axis.c
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/axis.c.o: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/linuxcnc/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/axis.c.o"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/axis.c.o -MF CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/axis.c.o.d -o CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/axis.c.o -c /home/user/linuxcnc/src/emc/motion/axis.c

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/axis.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/axis.c.i"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/linuxcnc/src/emc/motion/axis.c > CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/axis.c.i

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/axis.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/axis.c.s"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/linuxcnc/src/emc/motion/axis.c -o CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/axis.c.s

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/command.c.o: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/flags.make
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/command.c.o: /home/user/linuxcnc/src/emc/motion/command.c
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/command.c.o: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/linuxcnc/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/command.c.o"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/command.c.o -MF CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/command.c.o.d -o CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/command.c.o -c /home/user/linuxcnc/src/emc/motion/command.c

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/command.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/command.c.i"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/linuxcnc/src/emc/motion/command.c > CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/command.c.i

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/command.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/command.c.s"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/linuxcnc/src/emc/motion/command.c -o CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/command.c.s

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/control.c.o: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/flags.make
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/control.c.o: /home/user/linuxcnc/src/emc/motion/control.c
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/control.c.o: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/linuxcnc/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/control.c.o"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/control.c.o -MF CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/control.c.o.d -o CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/control.c.o -c /home/user/linuxcnc/src/emc/motion/control.c

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/control.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/control.c.i"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/linuxcnc/src/emc/motion/control.c > CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/control.c.i

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/control.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/control.c.s"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/linuxcnc/src/emc/motion/control.c -o CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/control.c.s

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/emcmotutil.c.o: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/flags.make
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/emcmotutil.c.o: /home/user/linuxcnc/src/emc/motion/emcmotutil.c
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/emcmotutil.c.o: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/linuxcnc/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/emcmotutil.c.o"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/emcmotutil.c.o -MF CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/emcmotutil.c.o.d -o CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/emcmotutil.c.o -c /home/user/linuxcnc/src/emc/motion/emcmotutil.c

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/emcmotutil.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/emcmotutil.c.i"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/linuxcnc/src/emc/motion/emcmotutil.c > CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/emcmotutil.c.i

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/emcmotutil.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/emcmotutil.c.s"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/linuxcnc/src/emc/motion/emcmotutil.c -o CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/emcmotutil.c.s

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/stashf.c.o: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/flags.make
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/stashf.c.o: /home/user/linuxcnc/src/emc/motion/stashf.c
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/stashf.c.o: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/linuxcnc/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/stashf.c.o"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/stashf.c.o -MF CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/stashf.c.o.d -o CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/stashf.c.o -c /home/user/linuxcnc/src/emc/motion/stashf.c

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/stashf.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/stashf.c.i"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/linuxcnc/src/emc/motion/stashf.c > CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/stashf.c.i

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/stashf.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/stashf.c.s"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/linuxcnc/src/emc/motion/stashf.c -o CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/stashf.c.s

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/dbuf.c.o: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/flags.make
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/dbuf.c.o: /home/user/linuxcnc/src/emc/motion/dbuf.c
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/dbuf.c.o: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/linuxcnc/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/dbuf.c.o"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/dbuf.c.o -MF CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/dbuf.c.o.d -o CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/dbuf.c.o -c /home/user/linuxcnc/src/emc/motion/dbuf.c

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/dbuf.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/dbuf.c.i"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/linuxcnc/src/emc/motion/dbuf.c > CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/dbuf.c.i

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/dbuf.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/dbuf.c.s"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/linuxcnc/src/emc/motion/dbuf.c -o CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/dbuf.c.s

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/scurve_engine.cpp.o: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/flags.make
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/scurve_engine.cpp.o: /home/user/linuxcnc/cmake/vendor/scurve/scurve_engine/scurve_engine.cpp
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/scurve_engine.cpp.o: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/linuxcnc/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/scurve_engine.cpp.o"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/scurve_engine.cpp.o -MF CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/scurve_engine.cpp.o.d -o CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/scurve_engine.cpp.o -c /home/user/linuxcnc/cmake/vendor/scurve/scurve_engine/scurve_engine.cpp

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/scurve_engine.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/scurve_engine.cpp.i"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/linuxcnc/cmake/vendor/scurve/scurve_engine/scurve_engine.cpp > CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/scurve_engine.cpp.i

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/scurve_engine.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/scurve_engine.cpp.s"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/linuxcnc/cmake/vendor/scurve/scurve_engine/scurve_engine.cpp -o CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/scurve_engine.cpp.s

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/simple_tp_scurve.c.o: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/flags.make
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/simple_tp_scurve.c.o: /home/user/linuxcnc/cmake/vendor/scurve/scurve_engine/simple_tp_scurve.c
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/simple_tp_scurve.c.o: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/linuxcnc/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/simple_tp_scurve.c.o"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/simple_tp_scurve.c.o -MF CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/simple_tp_scurve.c.o.d -o CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/simple_tp_scurve.c.o -c /home/user/linuxcnc/cmake/vendor/scurve/scurve_engine/simple_tp_scurve.c

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/simple_tp_scurve.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/simple_tp_scurve.c.i"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/linuxcnc/cmake/vendor/scurve/scurve_engine/simple_tp_scurve.c > CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/simple_tp_scurve.c.i

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/simple_tp_scurve.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/simple_tp_scurve.c.s"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/linuxcnc/cmake/vendor/scurve/scurve_engine/simple_tp_scurve.c -o CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/simple_tp_scurve.c.s

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/motion_scurve.c.o: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/flags.make
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/motion_scurve.c.o: /home/user/linuxcnc/cmake/vendor/scurve/scurve_engine/motion_scurve.c
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/motion_scurve.c.o: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/linuxcnc/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/motion_scurve.c.o"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/motion_scurve.c.o -MF CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/motion_scurve.c.o.d -o CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/motion_scurve.c.o -c /home/user/linuxcnc/cmake/vendor/scurve/scurve_engine/motion_scurve.c

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/motion_scurve.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/motion_scurve.c.i"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/linuxcnc/cmake/vendor/scurve/scurve_engine/motion_scurve.c > CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/motion_scurve.c.i

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/motion_scurve.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/motion_scurve.c.s"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/linuxcnc/cmake/vendor/scurve/scurve_engine/motion_scurve.c -o CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/motion_scurve.c.s

# Object files for target motmod_scurve
motmod_scurve_OBJECTS = \
"CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/kinematics/cubic.c.o" \
"CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/axis.c.o" \
"CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/command.c.o" \
"CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/control.c.o" \
"CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/emcmotutil.c.o" \
"CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/stashf.c.o" \
"CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/dbuf.c.o" \
"CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/scurve_engine.cpp.o" \
"CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/simple_tp_scurve.c.o" \
"CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/motion_scurve.c.o"

# External object files for target motmod_scurve
motmod_scurve_EXTERNAL_OBJECTS =

components/tpmod_scurve/motmod_scurve.so: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/kinematics/cubic.c.o
components/tpmod_scurve/motmod_scurve.so: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/axis.c.o
components/tpmod_scurve/motmod_scurve.so: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/command.c.o
components/tpmod_scurve/motmod_scurve.so: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/control.c.o
components/tpmod_scurve/motmod_scurve.so: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/emcmotutil.c.o
components/tpmod_scurve/motmod_scurve.so: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/stashf.c.o
components/tpmod_scurve/motmod_scurve.so: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/home/user/linuxcnc/src/emc/motion/dbuf.c.o
components/tpmod_scurve/motmod_scurve.so: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/scurve_engine.cpp.o
components/tpmod_scurve/motmod_scurve.so: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/simple_tp_scurve.c.o
components/tpmod_scurve/motmod_scurve.so: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/__/__/vendor/scurve/scurve_engine/motion_scurve.c.o
components/tpmod_scurve/motmod_scurve.so: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/build.make
components/tpmod_scurve/motmod_scurve.so: /home/user/linuxcnc/cmake/components/tpmod_scurve/../../../lib/liblinuxcnchal.so
components/tpmod_scurve/motmod_scurve.so: components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/user/linuxcnc/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX shared library motmod_scurve.so"
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/motmod_scurve.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/build: components/tpmod_scurve/motmod_scurve.so
.PHONY : components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/build

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/clean:
	cd /home/user/linuxcnc/cmake/build/components/tpmod_scurve && $(CMAKE_COMMAND) -P CMakeFiles/motmod_scurve.dir/cmake_clean.cmake
.PHONY : components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/clean

components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/depend:
	cd /home/user/linuxcnc/cmake/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/linuxcnc/cmake /home/user/linuxcnc/cmake/components/tpmod_scurve /home/user/linuxcnc/cmake/build /home/user/linuxcnc/cmake/build/components/tpmod_scurve /home/user/linuxcnc/cmake/build/components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : components/tpmod_scurve/CMakeFiles/motmod_scurve.dir/depend

