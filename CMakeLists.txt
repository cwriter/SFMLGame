cmake_minimum_required(VERSION 2.8)

#Set project name
project(Game)

#Set include dirs
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tpl)

#Set files
file(GLOB Game_sources
	"headers/*.h"
	"src/*.cpp"
)

#Set compiler flags
SET(CCFLAGS -g -Wall -O3 -rdynamic -std=c++11 -march=native)

#Set libraries
SET(LIBLIST sfml-system sfml-window sfml-graphics sfml-audio sfml-network sfgui pthread dl X11 gmpxx gmp gomp)

add_executable(Game ${Game_sources})

target_link_libraries(Game "${LIBLIST}")

#add_definitions(${CCFLAGS})
target_compile_options(Game PRIVATE "${CCFLAGS}" -fopenmp)

#Add a singlethreaded version (=no openmp)
add_executable(GameST  ${Game_sources})
target_link_libraries(GameST "${LIBLIST}")
target_compile_options(GameST PRIVATE "${CCFLAGS}")
set_target_properties(GameST PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
