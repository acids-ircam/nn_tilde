# Copyright 2018 The Min-DevKit Authors. All rights reserved.
# Use of this source code is governed by the MIT License found in the License.md file.

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../min-api)
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

if (APPLE)
	set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0")
endif()

#############################################################
# MAX EXTERNAL
#############################################################


include_directories( 
	"${C74_INCLUDES}", 
	"${CMAKE_CURRENT_SOURCE_DIR}/../shared",
	"${CMAKE_CURRENT_SOURCE_DIR}/../../shared",
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../shared",
	# "${CURL_INCLUDE_LOCATION}"
)

find_package(Torch REQUIRED)

# find_package(CURL REQUIRED HINTS ${CURL_LOCATION} PATHS ${CURL_LOCATION})

set(
SOURCE_FILES
	nn_tilde.cpp
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
target_link_libraries(${PROJECT_NAME} PRIVATE CURL::libcurl)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

if (APPLE) # SEARCH FOR TORCH DYLIB IN THE LOADER FOLDER
	set_target_properties(${PROJECT_NAME} PROPERTIES
	INSTALL_RPATH "@executable_path"
	BUILD_WITH_INSTALL_RPATH FALSE 
	LINK_FLAGS "-Wl,-rpath,@loader_path/"
	)
endif()

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

if (APPLE) # COPY TORCH DYLIB IN THE LOADER FOLDER
	add_custom_command( 
			TARGET ${PROJECT_NAME} 
			POST_BUILD 
			COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
			COMMAND cp "${TORCH_INSTALL_PREFIX}/lib/*.dylib" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
			COMMENT "Copy Torch Libraries" 
		)

	add_custom_command( 
		TARGET ${PROJECT_NAME} 
		POST_BUILD 
		COMMAND cp "${CMAKE_CURRENT_BINARY_DIR}/../../../_deps/curl-build/lib/libcurl.dylib" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
		COMMAND cp "${CMAKE_CURRENT_BINARY_DIR}/../../../_deps/curl-build/lib/libcurl-d.dylib" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
		COMMENT "Copy libcurl" 
	)

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND /bin/sh ${CMAKE_SOURCE_DIR}/otool_fix.sh "$<TARGET_FILE:${PROJECT_NAME}>"
		COMMAND install_name_tool -delete_rpath "${CMAKE_BINARY_DIR}/_deps/curl-build/lib" "$<TARGET_FILE:${PROJECT_NAME}>" | True
		COMMAND install_name_tool -delete_rpath "${TORCH_INSTALL_PREFIX}/lib" "$<TARGET_FILE:${PROJECT_NAME}>" | True
		COMMENT "copying additional libraries"
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

if (MSVC) # COPY TORCH DLL IN THE LOADER FOLDER
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${TORCH_INSTALL_PREFIX}/lib/" ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/
)
endif()

if (MSVC)
	set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()