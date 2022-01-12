cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(nn_tilde)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "-faligned-allocation")

message("CMAKE_HOST_SYSTEM_PROCESSOR: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES ${CMAKE_HOST_SYSTEM_PROCESSOR})
endif()

add_subdirectory(backend) # DEEP LEARNING BACKEND

if (UNIX AND NOT APPLE)
    add_subdirectory(frontend/puredata/nn_tilde) # PURE DATA EXTERNAL
endif()

if(APPLE OR MSVC)
    add_subdirectory(frontend/maxmsp/nn_tilde) # MAX MSP EXTERNAL
endif()