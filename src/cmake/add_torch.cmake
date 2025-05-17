set(torch_dir ${CMAKE_CURRENT_BINARY_DIR}/../torch)
set(torch_lib_name torch)

message("first looking for lib in : ${torch_dir}")
find_library(torch_lib
  NAMES ${torch_lib_name}
  PATHS ${torch_dir}/libtorch/lib
)

function (download_library url out)
  message("download ${url} to ${out}...")
  file(DOWNLOAD
      ${url}
      ${out}/torch_cc.zip
      SHOW_PROGRESS
    )
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xf torch_cc.zip
                    WORKING_DIRECTORY ${out})
    file(REMOVE ${out}/torch_cc.zip)
endfunction()

if (DEFINED torch_version)
  message("setting torch version : ${torch_version}")
else()
  set(torch_version "2.5.1")
  message("torch version : ${torch_version}")
endif()

if (NOT torch_lib)
  set(NEEDS_DL TRUE)
else()
  set(NEEDS_DL FALSE)
  if (UNIX AND NOT APPLE)
    if (torch_lib STREQUAL "/usr/lib/libtorch.so")
      set(NEEDS_DL TRUE)
    endif()
  endif()
endif()


if (NEEDS_DL)
  message(STATUS "Downloading torch C API pre-built")

  # Download
  if (UNIX AND NOT APPLE)  # Linux
    set(torch_url "https://download.pytorch.org/libtorch/cpu/libtorch-shared-with-deps-${torch_version}%2Bcpu.zip")
    download_library(${torch_url} ${torch_dir})
  elseif (UNIX AND APPLE)  # OSX
    if (NOT IS_DIRECTORY ${torch_dir})
    if (EXISTS ${CMAKE_SOURCE_DIR}/../install/torch_ub)
      execute_process(COMMAND cp -r ${CMAKE_SOURCE_DIR}/../install/torch_ub ${torch_dir})
    else()
      if (NOT DEFINED TORCH_MAC_UB_URL)
        message(FATAL_ERROR "If not provided, please give a valid URL for Apple universal library with -DTORCH_MAC_UB_URL ")
      endif()
      download_library(${TORCH_MAC_UB_URL} /tmp)
      execute_process(
        COMMAND mkdir -p ${torch_dir}
        COMMAND mv /tmp/torch ${torch_dir}/libtorch
      )
    endif()

    # Check if architecutre == ARM64
    # if (NOT DEFINED APPLE_ARM64)
    #   set (APPLE_ARM64 (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64"))
    # endif()
    ## If ARM, download both libraries and pre-compile UB libraries
    # if (APPLE_ARM64)
      # download arm64 library
      # if (NOT IS_DIRECTORY ${torch_dir})
      #   download_library("https://anaconda.org/pytorch/pytorch/${torch_version}/download/osx-arm64/pytorch-${torch_version}-py3.10_0.tar.bz2" ${torch_dir}-arm64)
      #   execute_process(COMMAND mkdir ${torch_dir})
      #   execute_process(COMMAND cp -r ${torch_dir}-arm64/lib/python3.10/site-packages/torch ${torch_dir}/libtorch)
      # endif()
      # # download x86 library
      # if (EXISTS ${CMAKE_SOURCE_DIR}/../install/torch_x86)
      #   execute_process(COMMAND cp -r ${CMAKE_SOURCE_DIR}/../install/torch_x86 ${torch_dir}-x86)
      # else()
      #   if (NOT DEFINED TORCH_MAC_UB_URL)
      #     message(FATAL_ERROR "If not provided, please give a valid URL for Apple universal library")
      #   endif()
      #   download_library(${TORCH_MAC_X86_URL} ${torch_dir})
      # endif()
      # message("found libtorch for x86 at : " ${torch_dir}-x86)
      # # export UB libs to main path
      # execute_process(COMMAND mkdir ${torch_dir}-x86)
      # execute_process(COMMAND cp /opt/homebrew/opt/llvm/lib/libomp.dylib ${torch_dir}/libtorch/lib/)
      # execute_process(COMMAND find ${torch_dir}/libtorch/lib -maxdepth 1 -type f -execdir lipo -create ${torch_dir}/libtorch/lib/{} ${torch_dir}-x86/libtorch/lib/{} -output ${torch_dir}/libtorch/lib/{} \;)
    # else()
    #   if (EXISTS ${CMAKE_SOURCE_DIR}/../install/torch_x86)
    #     execute_process(COMMAND cp ${CMAKE_SOURCE_DIR}/../install/torch_x86 ${torch_dir})
    #   else()
    #     if (NOT DEFINED TORCH_MAC_X86_URL)
    #       message(FATAL_ERROR "If not provided, please give a valid URL for Apple x86 library")
    #     endif()
    #     download_library(${TORCH_MAC_X86_URL} ${torch_dir})
    #   endif()
    #   message("found libtorch for x86 at : " ${torch_dir})
    endif()
  else()
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${torch_dir})
    download_library("https://download.pytorch.org/libtorch/cpu/libtorch-win-shared-with-deps-${torch_version}%2Bcpu.zip" ${torch_dir})
  endif()
endif()

# Find the libraries again
message("${torch_dir}")
find_library(torch_lib
  NAMES ${torch_lib_name}
  PATHS ${torch_dir}/libtorch/lib
)

if (NOT torch_lib)
  message(FATAL_ERROR "torch could not be included")
endif()
