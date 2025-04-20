cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(nn_tilde)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)


add_custom_target(copy_linking_file ALL
  COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_SOURCE_DIR}/frontend/maxmsp/min-api/max-sdk-base/script/max-linker-flags.txt"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/../install/max-linker-flags.txt" "${CMAKE_SOURCE_DIR}/frontend/maxmsp/min-api/max-sdk-base/script"
  COMMENT "Patching Max linkings"
)

set(CONDA_ENV_PATH "${CMAKE_SOURCE_DIR}/../env")
if (MSVC)
  set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}:${CONDA_ENV_PATH}")
else()
  set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CONDA_ENV_PATH}")
endif()


if (APPLE)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SET CMP00076)

include(FetchContent)

# import json
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2  # Specify the version you want to use
  CMAKE_ARGS "-DCMAKE_OSX_ARCHITECTURES:STRING=${CMAKE_OSX_ARCHITECTURES}" "DCMAKE_OSX_DEPLOYMENT_TARGET=10.13"
)
FetchContent_MakeAvailable(json)


if (UNIX)
  if (APPLE)
    add_compile_options(-std=c++20)
    set(CMAKE_CXX_FLAGS "-faligned-allocation")
    if (CMAKE_OSX_ARCHITECTURES STREQUAL "")
        set(CMAKE_OSX_ARCHITECTURES ${CMAKE_HOST_SYSTEM_PROCESSOR})
    endif()
  endif()
endif()


find_package(Torch REQUIRED)
add_subdirectory(backend) # DEEP LEARNING BACKEND

if ((NOT ("${PUREDATA_INCLUDE_DIR}" STREQUAL "")) OR (UNIX AND NOT APPLE))
    add_subdirectory(frontend/puredata/nn_tilde) # PURE DATA EXTERNAL
endif()

execute_process(
		COMMAND git describe --tags
		WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
		OUTPUT_VARIABLE VERSION
		OUTPUT_STRIP_TRAILING_WHITESPACE
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../package-info.json.in "${CMAKE_CURRENT_SOURCE_DIR}/package-info.json")

if(APPLE OR MSVC)
    # MAX MSP EXTERNAL
    # add_subdirectory(frontend/maxmsp/nn.info) 
    add_subdirectory(frontend/maxmsp/nn_tilde)
    # add_subdirectory(frontend/maxmsp/mc.nn_tilde)
    # add_subdirectory(frontend/maxmsp/mcs.nn_tilde)
endif()
