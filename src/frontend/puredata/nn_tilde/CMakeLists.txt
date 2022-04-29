cmake_minimum_required(VERSION 3.0)
project(nn_tilde_pd)
find_package(Torch REQUIRED)

file(GLOB SRC *.cpp)

add_library(nn SHARED ${SRC})

if (UNIX)
    execute_process(
			COMMAND git describe --tags
			WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
			OUTPUT_VARIABLE VERSION
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	message(${VERSION})
	add_definitions(-DVERSION="${VERSION}")
endif()

if (APPLE)
    set_target_properties(nn PROPERTIES PREFIX "" SUFFIX "~.pd_darwin")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -undefined dynamic_lookup -stdlib=libc++")
endif()

if (UNIX AND NOT APPLE)
    set_target_properties(nn PROPERTIES PREFIX "" SUFFIX "~.pd_linux")
endif()

target_include_directories(nn PRIVATE "${PUREDATA_INCLUDE_DIR}")
target_link_libraries(nn PRIVATE backend)
set_property(TARGET nn PROPERTY CXX_STANDARD 20)  