cmake_minimum_required(VERSION 2.8)

#Set project name
project(Game)

#Set include dirs
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers)

#Set files
file(GLOB Game_sources
	"headers/*.h"
	"src/*.cpp"
)

#Set compiler flags
SET(CCFLAGS "-g -fopenmp -rdynamic -O3 -std=c++11 -march=native")

add_executable(Game ${Game_sources})

target_link_libraries(Game sfml-system sfml-window sfml-graphics sfml-audio sfml-network pthread dl X11 gmpxx gmp gomp)


add_definitions(${CCFLAGS})

