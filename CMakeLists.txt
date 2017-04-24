cmake_minimum_required(VERSION 2.8)

set(BIN gpu_cpu_performance_comparison)
project(${BIN})

set(SOURCES_ROOT ./src/)
set(HEADERS_ROOT ./include/ ./cpu_computing/include/ ./gpu_computing/include/)

# SOURCE - list of all sources
aux_source_directory(${SOURCES_ROOT} SOURCE)

# I use it for VS2015, in order to IDE shows me header files
file(GLOB_RECURSE HEADERS "./include/*.h")

# include files dependecies
include_directories(${HEADERS_ROOT})

# add subprojects
add_subdirectory(./cpu_computing/)
add_subdirectory(./gpu_computing/)

# build from specified sources cpu_gpu_comparison executable
add_executable(${BIN} ${SOURCE} ${HEADERS})

# linker settings
target_link_libraries(${BIN} cpu_computing gpu_computing)