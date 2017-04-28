cmake_minimum_required(VERSION 2.8)

set(CPU_LIB cpu_computing)
project(${CPU_LIB})

# list of all source files
set(SOURCES_ROOT ./src/)
set(CPU_HEADERS_ROOT ./include/ ../include/)

# SOURCE - list of all sources
aux_source_directory(${SOURCES_ROOT} DLL_SOURCES)

# I use it for VS2015, in order to IDE shows me header files
file(GLOB_RECURSE HEADERS "./include/*.h")

include_directories(${CPU_HEADERS_ROOT})

add_library(${CPU_LIB} STATIC ${DLL_SOURCES} ${HEADERS})

target_link_libraries(${CPU_LIB})