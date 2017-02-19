cmake_minimum_required(VERSION 2.8)

set(GPU_LIB GpuComputing)
project(${GPU_LIB})

find_package(CUDA QUIET REQUIRED)

# list of all source files
set(SOURCES_ROOT ./src/)
set(GPU_HEADERS_ROOT ./include/)

# SOURCE - list of all sources
aux_source_directory(${SOURCES_ROOT} DLL_SOURCES)

# I use it for VS2015, in order to IDE shows me header files
file(GLOB_RECURSE HEADERS "./include/*.h")

include_directories(${GPU_HEADERS_ROOT})

cuda_add_library(${GPU_LIB} STATIC ${DLL_SOURCES} ./src/kernel.cu ${HEADERS})

target_link_libraries(${GPU_LIB})