cmake_minimum_required(VERSION 3.10)
project(nn_tilde_pd)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Torch REQUIRED)

file(GLOB SRC *.cpp)

add_library(nn SHARED ${SRC})

if (MSVC)
	set_property(TARGET nn PROPERTY CXX_STANDARD 20)
	target_compile_features(nn PUBLIC "cxx_std_20")
endif()

# Get version from git for all platforms
execute_process(
    COMMAND git describe --tags
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(VERSION)
    message(STATUS "Building version: ${VERSION}")
    add_definitions(-DVERSION="${VERSION}")
endif()

# COPY HELP FILES
add_custom_command(TARGET nn POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/frontend/puredata/nn_tilde/nn~-help.pd"
        "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/nn~-help.pd"
    COMMENT "Copy Help File"
)

if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")

    add_custom_command(
        TARGET nn
        POST_BUILD
        COMMAND cp "${TORCH_INSTALL_PREFIX}/lib/*.dylib" "${CMAKE_CURRENT_BINARY_DIR}/"
        COMMENT "Copy Torch Libraries"
    )

    set_target_properties(nn PROPERTIES
        PREFIX ""
        SUFFIX "~.pd_darwin"
        BUILD_WITH_INSTALL_RPATH FALSE
        LINK_FLAGS "-Wl,-rpath,@loader_path/"
    )

    add_custom_command(
        TARGET nn
        POST_BUILD 
        COMMAND ${CMAKE_SOURCE_DIR}/../env/bin/python ${CMAKE_SOURCE_DIR}/../install/dylib_fix.py -p "${CMAKE_CURRENT_BINARY_DIR}/*.pd_darwin" -l "${CMAKE_BINARY_DIR}/_deps"  "${CMAKE_SOURCE_DIR}/../env" 
        COMMENT "Fixing libraries and codesigning"
    )

endif()


function(resolve_symlink symlink_path resolved_path)
    execute_process(
        COMMAND readlink -f ${symlink_path}
        OUTPUT_VARIABLE resolved
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(${resolved_path} ${resolved} PARENT_SCOPE)
endfunction()


if (UNIX AND NOT APPLE)
    set(TORCH_ESSENTIAL_LIBS
        "libtorch.so*"
        "libtorch_cpu.so*"
        "libc10.so*"
        "libgomp*so*"
        "libtorch_global_deps.so*"
    )
    set(CURL_ESSENTIAL_LIBS
        "libnghttp2.so*"
        "libssh2.so*"
        "libssl.so*" 
        "libkrb5.so*"
        "libk5crypto.so*"
        "libkrb5support.so*"
        "libcrypto.so*" 
        "libgssapi_krb5.so*" 
        "libzstd.so*"
        "libcom_err.so*"
        "libz.so*"
        "libcurl.so*"
    )

    # Copy essential Torch libraries
    add_custom_target(copy_torch_libs)
    foreach(LIB_PATTR ${TORCH_ESSENTIAL_LIBS})
    file(GLOB CURRENT_PATHS "${CMAKE_SOURCE_DIR}/../env/lib/python3.12/site-packages/torch/lib/${LIB_PATTR}")
    list(LENGTH CURRENT_PATHS N_PATHS)
    if (NOT ${N_PATHS} EQUAL 0)
        list(GET CURRENT_PATHS 0 LIB)
        get_filename_component(LIB_NAME ${LIB} NAME)
        add_custom_command(
            TARGET copy_torch_libs
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${LIB}"
                "${CMAKE_CURRENT_BINARY_DIR}/$(basename ${LIB})"
            COMMENT "Copying ${LIB_NAME}"
        )
    else() 
        message("${LIB_PATTR} not found")
    endif()
    endforeach()

    # Copy essential curl libs
    foreach(LIB_PATTR ${CURL_ESSENTIAL_LIBS})
        file(GLOB CURRENT_PATHS "${CMAKE_SOURCE_DIR}/../env/lib/${LIB_PATTR}")
        list(LENGTH CURRENT_PATHS N_PATHS)
        if (NOT ${N_PATHS} EQUAL 0)
            list(GET CURRENT_PATHS 0 LIB)
            resolve_symlink("${LIB}" original_path)
            get_filename_component(LIB_NAME ${original_path} NAME)
            add_custom_command(
                TARGET copy_torch_libs
                PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${original_path}"
                    "${CMAKE_CURRENT_BINARY_DIR}/${LIB}"
                COMMENT "Copying ${LIB_NAME}"
            )
        else() 
            message("${LIB_PATTR} not found")
        endif()
    endforeach()

    add_dependencies(nn copy_torch_libs)

    set_target_properties(nn PROPERTIES 
        PREFIX ""
        SUFFIX "~.pd_linux"
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH_USE_LINK_PATH TRUE
        INSTALL_RPATH "$ORIGIN"
    )

    # Add libgomp as a link dependency
    target_link_libraries(nn PRIVATE gomp)
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

    # Base DLLs (these work for CPU version)
    set(REQUIRED_DLLS
        "torch_cpu.dll"
        "c10.dll"
        "fbgemm.dll"
        "libiomp5md.dll"
        "libiompstubs5md.dll"
        "uv.dll"
        "asmjit.dll"
        "torch.dll"
        "torch_global_deps.dll"
    )

    # CUDA DLL setup
    if(CUDA_FOUND OR EXISTS "${TORCH_INSTALL_PREFIX}/lib/torch_cuda.dll")
        list(APPEND REQUIRED_DLLS
            # PyTorch CUDA DLLs
            "torch_cuda.dll"
            "c10_cuda.dll"
            # Core CUDA Runtime DLLs
            "cudart64_12.dll"
            # Additional CUDA DLLs
            "cudnn64_9.dll"
            "cudnn_graph64_9.dll"
            "cudnn_engines_precompiled64_9.dll"
            "cudnn_engines_runtime_compiled64_9.dll"
            "cudnn_heuristic64_9.dll"
            "nvrtc-builtins64_120.dll"
            "cudadevrt.dll"
        )

        # VC Runtime handling
        if(DEFINED ENV{VCREDIST_PATH} AND EXISTS "$ENV{VCREDIST_PATH}/vcruntime140_1.dll")
            message(STATUS "Found VC Runtime at: $ENV{VCREDIST_PATH}")
            list(APPEND REQUIRED_DLLS "$ENV{VCREDIST_PATH}/vcruntime140_1.dll")
        else()
            message(WARNING "VC Runtime not found in VCREDIST_PATH")
        endif()

        # CUDA runtime DLL handling
        if(DEFINED ENV{CUDA_PATH})
            file(GLOB CUDA_RUNTIME_DLLS "$ENV{CUDA_PATH}/bin/*.dll")
            foreach(CUDA_DLL ${CUDA_RUNTIME_DLLS})
                get_filename_component(DLL_NAME ${CUDA_DLL} NAME)
                foreach(REQUIRED_DLL ${REQUIRED_DLLS})
                    if(DLL_NAME STREQUAL REQUIRED_DLL)
                        add_custom_command(TARGET nn POST_BUILD
                            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                                "${CUDA_DLL}"
                                "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${DLL_NAME}"
                            COMMENT "Copying CUDA runtime DLL: ${DLL_NAME}"
                        )
                    endif()
                endforeach()
            endforeach()
        endif()
    endif()

    # Copy libtorch DLLs
    foreach(DLL ${REQUIRED_DLLS})
        if(EXISTS "${TORCH_INSTALL_PREFIX}/lib/${DLL}")
            add_custom_command(TARGET nn POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${TORCH_INSTALL_PREFIX}/lib/${DLL}"
                    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${DLL}"
                COMMENT "Copying ${DLL}"
            )
        endif()
    endforeach()

    set_target_properties(nn PROPERTIES PREFIX "" SUFFIX "~.dll")
endif()

if(NOT $ENV{PD_EXTERNAL_PATH} STREQUAL "")
    add_custom_command(
        TARGET nn
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory  "${CMAKE_CURRENT_BINARY_DIR}" "$ENV{PD_EXTERNAL_PATH}/nn_tilde"
        COMMENT "Copying ${CMAKE_CURRENT_BINARY_DIR} to $ENV{PD_EXTERNAL_PATH}/nn_tilde"
    )
endif()

target_link_libraries(nn PRIVATE backend)
target_include_directories(nn PRIVATE "${PUREDATA_INCLUDE_DIR}")

set(CONDA_ENV_PATH "${CMAKE_SOURCE_DIR}/../env")
set(CURL_INCLUDE_DIR "${CONDA_ENV_PATH}/include")

if (UNIX)
    if (APPLE)
        set(CURL_LIBRARY "${CONDA_ENV_PATH}/lib/libcurl.dylib")
    else()
        set(CURL_LIBRARY "${CONDA_ENV_PATH}/lib/libcurl.so")
    endif()
endif()

include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(nn PRIVATE ${CURL_LIBRARY})

target_link_libraries(nn PRIVATE nlohmann_json::nlohmann_json)
if (MSVC)
    target_link_libraries(nn PRIVATE "${PUREDATA_BIN_DIR}/pd.lib" shlwapi.lib)
endif()