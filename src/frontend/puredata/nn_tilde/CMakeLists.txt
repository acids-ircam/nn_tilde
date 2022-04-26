cmake_minimum_required(VERSION 3.0)
project(nn_tilde_pd)
find_package(Torch REQUIRED)

file(GLOB SRC *.cpp)

add_library(nn SHARED ${SRC})
set_target_properties(nn PROPERTIES PREFIX "" SUFFIX "~.pd_darwin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -undefined dynamic_lookup -stdlib=libc++")
target_include_directories(nn PRIVATE "${PUREDATA_INCLUDE_DIR}")
target_link_libraries(nn PRIVATE backend)
set_property(TARGET nn PROPERTY CXX_STANDARD 20)  