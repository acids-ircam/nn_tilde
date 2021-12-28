cmake_minimum_required(VERSION 3.0)
project(nn_tilde_pd)
find_package(Torch REQUIRED)

file(GLOB SRC *.cpp)


add_library(nn SHARED ${SRC})
set_target_properties(nn PROPERTIES PREFIX "" SUFFIX "~.pd_linux")
target_link_libraries(nn PRIVATE backend)
set_property(TARGET nn PROPERTY CXX_STANDARD 14)  