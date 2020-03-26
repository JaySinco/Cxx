cmake_minimum_required(VERSION 3.2)

project(CXX)
add_custom_target(__target_root)
add_custom_target(___trash_do_nothing)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

if (NOT TARGET_ABI)
    set(TARGET_ABI "x64")
endif()
if (NOT TARGET_OS)
    set(TARGET_OS "windows")
endif()
message(STATUS "ABI -> " ${TARGET_ABI})
message(STATUS "OS -> " ${TARGET_OS})

set(WINDOWS_DLL_LIST
    ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/openssl/bin/libssl-1_1-x64.dll
    ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/openssl/bin/libcrypto-1_1-x64.dll
)
message(STATUS "Windows dll -> " ${WINDOWS_DLL_LIST})

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR})
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/gflags/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/glog/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/gtest/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/openssl/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mxnet/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/glfw/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/thrift/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/boost/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/curl/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/zlib/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/leveldb/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mongo/include/bsoncxx/v_noabi/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mongo/include/mongocxx/v_noabi/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/json/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/glm/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/pprint/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/stb/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/glad/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/imgui/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/lua/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/tinyobjloader/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/tinyxml/include/)
message(STATUS "Include dir -> " ${INCLUDE_DIR})
include_directories(${INCLUDE_DIR})

macro(CAR var)
    SET(${var} ${ARGV1})
ENDMACRO(CAR)

macro(CDR var junk)
    SET(${var} ${ARGN})
endmacro(CDR)

macro (add_custom_target)
    _add_custom_target(${ARGN})

    if (${ARGV0} MATCHES "^__target_.*")
        message(STATUS "Add target: '${ARGV0}'")
        list(APPEND SUBDIRECTORY_TARGETS ${CUSTOM_TARGET} ${ARGN})
        set(SUBDIRECTORY_TARGETS "${SUBDIRECTORY_TARGETS}" PARENT_SCOPE)
        set(SUBDIRECTORY_TARGETS "___trash_do_nothing")
    endif()
endmacro()

macro(configure_msvc_runtime)
  if(MSVC)

    # Default to statically-linked runtime.
    if("${MSVC_RUNTIME}" STREQUAL "")
      set(MSVC_RUNTIME "static")
    endif()

    # Set compiler options.
    set(variables
      CMAKE_C_FLAGS_DEBUG
      CMAKE_C_FLAGS_MINSIZEREL
      CMAKE_C_FLAGS_RELEASE
      CMAKE_C_FLAGS_RELWITHDEBINFO
      CMAKE_CXX_FLAGS_DEBUG
      CMAKE_CXX_FLAGS_MINSIZEREL
      CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_RELWITHDEBINFO
    )
    if(${MSVC_RUNTIME} STREQUAL "static")
      message(STATUS
        "MSVC -> forcing use of statically-linked runtime."
      )
      foreach(variable ${variables})
        if(${variable} MATCHES "/MD")
          string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
        endif()
      endforeach()
    else()
      message(STATUS
        "MSVC -> forcing use of dynamically-linked runtime."
      )
      foreach(variable ${variables})
        if(${variable} MATCHES "/MT")
          string(REGEX REPLACE "/MT" "/MD" ${variable} "${${variable}}")
        endif()
      endforeach()
    endif()
  endif()
endmacro()

macro (add_executable)
    CAR(EXECUTABLE_NAME ${ARGN})
    CDR(ARGUMENTS ${ARGN})
    _add_executable(${EXECUTABLE_NAME} ${ARGUMENTS})

    if (TARGET __target_${MODULE_NAME})
        message(STATUS "Add executable: '${EXECUTABLE_NAME}'")
        add_dependencies(__target_${MODULE_NAME} 
            ${EXECUTABLE_NAME}
            )
    else()
        message(FATAL_ERROR "missing target: __target_${MODULE_NAME}, current_dir: ${CMAKE_CURRENT_SOURCE_DIR}")
    endif()

    if(MSVC)
        message(STATUS "Copy dll dependencies to bin directory.")
        # Copy DLL files to output directory
        add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${WINDOWS_DLL_LIST}
            $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
    endif()

    if (MSVC_DEBUG)
      target_compile_options(${EXECUTABLE_NAME} PRIVATE /Z7)
      target_link_options(${EXECUTABLE_NAME} PRIVATE /INCREMENTAL:NO /DEBUG /OPT:REF /OPT:ICF)
    endif()
endmacro()

macro (add_library)
    CAR(TARGET_NAME_ ${ARGN})
    CDR(ARGUMENTS ${ARGN})
    _add_library(${TARGET_NAME_} ${ARGUMENTS})
    set_target_properties(${TARGET_NAME_} PROPERTIES PREFIX "lib")

    if (TARGET __target_${MODULE_NAME})
        message("Adding ${TARGET_NAME_} to target: ${MODULE_NAME}")
        add_dependencies(__target_${MODULE_NAME} 
            ${TARGET_NAME_}
            )
    else()
        message(FATAL_ERROR "missing target: __target_${MODULE_NAME}, current_dir: ${CMAKE_CURRENT_SOURCE_DIR}")
    endif()
endmacro()

macro (add_subdirectory)
    CAR(DIRNAME ${ARGN})
    CDR(ARGUMENTS ${ARGN})
    _add_subdirectory (${DIRNAME})
endmacro()

configure_msvc_runtime()

add_subdirectory(external)
add_subdirectory(common)
add_subdirectory(product)

add_dependencies(__target_root
    ${SUBDIRECTORY_TARGETS}
    )