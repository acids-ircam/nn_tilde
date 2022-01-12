cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(nn_tilde)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "-faligned-allocation")

message("CMAKE_HOST_SYSTEM_PROCESSOR: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
if (APPLE)
    if (CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64")
        set(CMAKE_OSX_ARCHITECTURES arm64)
    endif()
endif()

add_subdirectory(backend) # DEEP LEARNING BACKEND

if (UNIX AND NOT APPLE)
    add_subdirectory(frontend/puredata/nn_tilde) # PURE DATA EXTERNAL
endif()

if(APPLE OR MSVC)
    add_subdirectory(frontend/maxmsp/nn_tilde) # MAX MSP EXTERNAL
endif()