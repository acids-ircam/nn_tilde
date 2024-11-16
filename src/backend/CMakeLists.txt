cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(backend)

if(
    PYTORCH_VERSION VERSION_GREATER_EQUAL "2.5.0" AND
    PYTORCH_CUDA_VERSION VERSION_GREATER_EQUAL "12"
)
    message(STATUS "PyTorch NVTX headers workaround: Yes")
    # only do this if nvToolsExt is not defined and CUDA::nvtx3 exists
    if(NOT TARGET CUDA::nvToolsExt AND TARGET CUDA::nvtx3)
        add_library(CUDA::nvToolsExt INTERFACE IMPORTED)
        target_compile_definitions(
            CUDA::nvToolsExt INTERFACE
            TORCH_CUDA_USE_NVTX3
        )
        target_link_libraries(CUDA::nvToolsExt INTERFACE CUDA::nvtx3)
    endif()
else()
    message(STATUS "PyTorch NVTX headers workaround: No")
endif()

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

add_library(backend STATIC parsing_utils.cpp backend.cpp)
target_link_libraries(backend "${TORCH_LIBRARIES}")
set_property(TARGET backend PROPERTY CXX_STANDARD 17)

if(MSVC)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_CXX_FLAGS_MINSIZEREL  "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MT")
endif()