# Copyright 2018 The Min-DevKit Authors. All rights reserved.
# Use of this source code is governed by the MIT License found in the License.md file.

cmake_minimum_required(VERSION 3.0)

set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../min-api)
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

if (APPLE)
	set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0")
endif()

#############################################################
# MAX EXTERNAL
#############################################################


include_directories( 
	"${C74_INCLUDES}"
)
find_package(Torch REQUIRED)

set(
SOURCE_FILES
	mc.nn_tilde.cpp
)

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

add_library( 
	${PROJECT_NAME} 
	MODULE
	${SOURCE_FILES}
)
target_link_libraries(${PROJECT_NAME} PRIVATE backend)



if (APPLE) # SEARCH FOR TORCH DYLIB IN THE LOADER FOLDER
set_target_properties(${PROJECT_NAME} PROPERTIES
BUILD_WITH_INSTALL_RPATH FALSE
LINK_FLAGS "-Wl,-rpath,@loader_path/"
)
endif()

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

if (APPLE) # COPY TORCH DYLIB IN THE LOADER FOLDER
add_custom_command( 
		TARGET ${PROJECT_NAME} 
		POST_BUILD 
		COMMAND cp "${TORCH_INSTALL_PREFIX}/lib/*.dylib" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${${PROJECT_NAME}_EXTERN_OUTPUT_NAME}.mxo/Contents/MacOS/" 
		COMMENT "Copy Torch Libraries" 
	)

if (CMAKE_OSX_ARCHITECTURES STREQUAL "arm64")
	add_custom_command( 
		TARGET ${PROJECT_NAME} 
		POST_BUILD 
		COMMAND "codesign" "--force" "--deep" "-s" "-" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${${PROJECT_NAME}_EXTERN_OUTPUT_NAME}.mxo"
		COMMENT "Codesign external" 
	)
endif()
endif()

if (MSVC)
	set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()