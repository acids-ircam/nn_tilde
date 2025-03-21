cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(nn_tilde)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
# import curl
FetchContent_Declare(
  curl
  URL https://github.com/curl/curl/releases/download/curl-7_79_1/curl-7.79.1.tar.gz
)
FetchContent_MakeAvailable(curl)

# import json
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2  # Specify the version you want to use
)
FetchContent_MakeAvailable(json)


# file(GLOB CURL_LOCATION "../env/pkgs/libcurl*/lib")
# set(CURL_LOCATION "../env/pkgs/libcurl-8.11.1-hde089ae_0/lib")
# get_filename_component(CURL_LOCATION "${CURL_LOCATION}" ABSOLUTE)
# file(GLOB CURL_INCLUDE_LOCATION "../env/pkgs/libcurl*/include/curl")

# message("CURL LOCATION : ${CURL_LOCATION}")
# list(APPEND CMAKE_PREFIX_PATH CURL_LOCATION)
list(APPEND CMAKE_PREFIX_PATH ../env/lib/python3.12/site-packages/torch)

if (APPLE)
    set(CMAKE_CXX_FLAGS "-faligned-allocation")
    if (CMAKE_OSX_ARCHITECTURES STREQUAL "")
        set(CMAKE_OSX_ARCHITECTURES ${CMAKE_HOST_SYSTEM_PROCESSOR})
    endif()
    message("CMAKE_OSX_ARCHITECTURES: ${CMAKE_OSX_ARCHITECTURES}")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum OS X deployment version")
    add_compile_options(-std=c++20)
endif()

find_package(Torch REQUIRED)
find_package(CURL REQUIRED)

# manage CURL dependency
# file(GLOB CURL_DYLIB_PATH "../env/lib/libcurl*.dylib")
# file(GLOB CURL_INCLUDE_DIR "../env/pkgs/libcurl*/include/curl")


add_subdirectory(backend) # DEEP LEARNING BACKEND


if ((NOT ("${PUREDATA_INCLUDE_DIR}" STREQUAL "")) OR (UNIX AND NOT APPLE))
    add_subdirectory(frontend/puredata/nn_tilde) # PURE DATA EXTERNAL
endif()

if(APPLE OR MSVC)
    # add_subdirectory(frontend/maxmsp/nn.info) # MAX MSP EXTERNAL
    # add_subdirectory(frontend/maxmsp/nn_tilde) # MAX MSP EXTERNAL
    add_subdirectory(frontend/maxmsp/mc.nn_tilde) # MAX MSP EXTERNAL
    # add_subdirectory(frontend/maxmsp/mcs.nn_tilde) # MAX MSP EXTERNAL
endif()