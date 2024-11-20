cmake_minimum_required(VERSION 3.0)
project(nn_tilde_pd)
find_package(Torch REQUIRED)

file(GLOB SRC *.cpp)

if (MSVC)
   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()

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
	set_target_properties(nn PROPERTIES
		BUILD_WITH_INSTALL_RPATH FALSE
		LINK_FLAGS "-Wl,-rpath,@loader_path/"
	)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -undefined dynamic_lookup -stdlib=libc++")
endif()


if (UNIX AND NOT APPLE)
    set_target_properties(nn PROPERTIES PREFIX "" SUFFIX "~.pd_linux")
endif()

if (MSVC)
    set_target_properties(nn PROPERTIES PREFIX "" SUFFIX "~.dll")
endif()

if (APPLE) # COPY TORCH DYLIB IN THE LOADER FOLDER
	add_custom_command( 
		TARGET nn
		POST_BUILD 
		COMMAND cp "${TORCH_INSTALL_PREFIX}/lib/*.dylib" "${CMAKE_CURRENT_BINARY_DIR}/" 
		COMMENT "Copy Torch Libraries" 
	)
endif()

if (UNIX AND NOT APPLE)
    set_target_properties(nn PROPERTIES PREFIX "" SUFFIX "~.pd_linux")
    add_custom_command(
    	TARGET nn
    	POST_BUILD
        COMMAND cp "${TORCH_INSTALL_PREFIX}/lib/*.so" "${CMAKE_CURRENT_BINARY_DIR}/" 
        COMMENT "Copy Torch Libraries"
    )
endif()

if (MSVC)
    # Define base required DLLs
    set(REQUIRED_DLLS
        "torch_cpu.dll"
        "c10.dll"
        "fbgemm.dll"
        "libiomp5md.dll"
        "uv.dll"
    )

    # Multiple checks for CUDA support
    if(CUDA_FOUND OR EXISTS "${TORCH_INSTALL_PREFIX}/lib/torch_cuda.dll")
        message(STATUS "CUDA support detected - Including CUDA DLLs")
        message(STATUS "CUDA_FOUND: ${CUDA_FOUND}")
        message(STATUS "TORCH_INSTALL_PREFIX: ${TORCH_INSTALL_PREFIX}")
        
        list(APPEND REQUIRED_DLLS
            "torch_cuda.dll"
            "c10_cuda.dll"
        )
    else()
        message(STATUS "No CUDA support detected - Skipping CUDA DLLs")
    endif()

    # Copy only the required DLLs
    foreach(DLL ${REQUIRED_DLLS})
        add_custom_command(TARGET nn POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Copying ${DLL}..."
            COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                "${TORCH_INSTALL_PREFIX}/lib/${DLL}"
                "${CMAKE_CURRENT_BINARY_DIR}/Release/${DLL}"
            COMMENT "Copying ${DLL}"
        )
    endforeach()
endif()

if (CMAKE_OSX_ARCHITECTURES STREQUAL "arm64")
	add_custom_command( 
		TARGET nn
		POST_BUILD 
		COMMAND "codesign" "--force" "--deep" "-s" "-" "${CMAKE_CURRENT_BINARY_DIR}/*.dylib"
		COMMAND "codesign" "--force" "--deep" "-s" "-" "${CMAKE_CURRENT_BINARY_DIR}/*.pd_darwin"
		COMMAND "xattr" "-r" "-d" "com.apple.quarantine" "${CMAKE_CURRENT_BINARY_DIR}"
		COMMENT "Codesign external" 
	)
endif()

target_include_directories(nn PRIVATE "${PUREDATA_INCLUDE_DIR}")
target_link_libraries(nn PRIVATE backend)
if (MSVC)
	target_link_libraries(nn PRIVATE "${PUREDATA_BIN_DIR}/pd.lib")
endif()
set_property(TARGET nn PROPERTY CXX_STANDARD 17)
