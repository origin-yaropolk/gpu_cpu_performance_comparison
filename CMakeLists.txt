cmake_minimum_required(VERSION 2.8)

set(BIN PerformanceComparison)
project(${BIN})

set(SOURCES_ROOT ./src/)
set(HEADERS_ROOT ./include/ ./CpuComputing/include/ ./GpuComputing/include/)

# SOURCE - list of all sources
aux_source_directory(${SOURCES_ROOT} SOURCE)

# I use it for VS2015, in order to IDE shows me header files
file(GLOB_RECURSE HEADERS "./include/*.h")

# include files dependecies
include_directories(${HEADERS_ROOT})

# add subprojects
add_subdirectory(./CpuComputing/)
add_subdirectory(./GpuComputing/)

# build from specified sources cpu_gpu_comparison executable
add_executable(${BIN} ${SOURCE} ${HEADERS})

# linker settings
target_link_libraries(${BIN} CpuComputing GpuComputing)