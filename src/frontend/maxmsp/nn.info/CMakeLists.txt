# Copyright 2018 The Min-DevKit Authors. All rights reserved.
# Use of this source code is governed by the MIT License found in the License.md file.

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../min-api)
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

if (APPLE)
	set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12")
endif()


#############################################################
# MAX EXTERNAL
#############################################################

if (UNIX)
	execute_process(
			COMMAND git describe --tags
			WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
			OUTPUT_VARIABLE VERSION
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	message("version : ${VERSION}")
	add_definitions(-DVERSION="${VERSION}")
endif()



set(
SOURCE_FILES
	nn.info.cpp
)

add_library( 
	${PROJECT_NAME} 
	MODULE
	${SOURCE_FILES}
)

include_directories( 
	"${C74_INCLUDES}", 
	"${CMAKE_CURRENT_SOURCE_DIR}/../shared",
	"${CMAKE_CURRENT_SOURCE_DIR}/../../shared",
)

if (MSVC)
	include_directories(${VCPKG_INCLUDE_DIR})
	link_directories(${VCPKG_LIB_DIR})
endif()


target_link_libraries(${PROJECT_NAME} PRIVATE backend)

if (UNIX)
	set(CONDA_ENV_PATH "${CMAKE_SOURCE_DIR}/../env")
	set(CURL_INCLUDE_DIR "${CONDA_ENV_PATH}/include")
	set(CURL_LIBRARY "${CONDA_ENV_PATH}/lib/libcurl.dylib")
	include_directories(${CURL_INCLUDE_DIR})
elseif(MSVC)
	set(VCPKG_PATH "${CMAKE_SOURCE_DIR}/../vcpkg")
	set(CURL_INCLUDE_DIR "${VCPKG_PATH}/packages/curl_x64-windows/include")
	set(CURL_LIBRARY "${VCPKG_PATH}/packages/curl_x64-windows/lib/libcurl.lib")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CURL_LIBRARY})



include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

if (MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
	set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
	target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
endif()


if (APPLE) # SEARCH FOR TORCH DYLIB IN THE LOADER FOLDER
set_target_properties(${PROJECT_NAME} PROPERTIES
	BUILD_WITH_INSTALL_RPATH FALSE
	LINK_FLAGS "-Wl,-rpath,@loader_path/"
)
endif()




if (APPLE) # COPY DYLIBS IN THE LOADER FOLDER

	add_custom_command(
		TARGET ${PROJECT_NAME}
		POST_BUILD 
		COMMAND ${CMAKE_COMMAND} -E copy  "${CMAKE_SOURCE_DIR}/../env/ssl/cert.pem" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
		COMMAND ${CMAKE_SOURCE_DIR}/../env/bin/python ${CMAKE_SOURCE_DIR}/../install/dylib_fix.py -p "$<TARGET_FILE:${PROJECT_NAME}>" -l "${CMAKE_BINARY_DIR}/_deps"  "${CMAKE_SOURCE_DIR}/../env" "${HOMEBREW_PREFIX}"
		COMMENT "Fixing libraries, certificates, permissions, codesigning, quarantine"
	)
	
endif()
