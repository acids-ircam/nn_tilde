cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(nn_tilde)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (APPLE)
set(CMAKE_OSX_ARCHITECTURES x86_64) # UNTIL M1 SUPPORTED BY MAX
endif()

add_subdirectory(backend) # DEEP LEARNING BACKEND

if (UNIX AND NOT APPLE)
add_subdirectory(frontend/puredata/nn_tilde) # PURE DATA EXTERNAL
endif()

if(APPLE OR MSVC)
add_subdirectory(frontend/maxmsp/nn_tilde) # MAX MSP EXTERNAL
endif()