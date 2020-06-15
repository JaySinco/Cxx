cmake_minimum_required(VERSION 3.2)

project(CXX)
add_custom_target(__target_root)
add_custom_target(___trash_do_nothing)

if (NOT TARGET_ABI)
    set(TARGET_ABI "x64")
endif()
if (NOT TARGET_OS)
    set(TARGET_OS "linux")
endif()
message(STATUS "ABI -> " ${TARGET_ABI})
message(STATUS "OS -> " ${TARGET_OS})

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
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mongo/include/bsoncxx/v_noabi/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mongo/include/mongocxx/v_noabi/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/glm/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/stb/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/glad/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/lua/include/)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/tinyobjloader/include/)
message(STATUS "Include dir -> " ${INCLUDE_DIR})
include_directories(${INCLUDE_DIR})

# set(LINK_DIR ${CMAKE_SOURCE_DIR})
# list(APPEND LINK_DIR ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/xxx/lib/)
# message(STATUS "Link dir -> " ${LINK_DIR})
# link_directories(${LINK_DIR})

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

macro (add_executable)
    CAR(EXECUTABLE_NAME ${ARGN})
    CDR(ARGUMENTS ${ARGN})
    _add_executable(${EXECUTABLE_NAME} ${ARGUMENTS})

    if (TARGET __target_${MODULE_NAME})
        message(STATUS "Add executable: '${EXECUTABLE_NAME}'")
        add_dependencies(__target_${MODULE_NAME} 
            ${EXECUTABLE_NAME}
            )
        message(STATUS "Copy build script to source directory.")
        file(COPY ${CMAKE_SOURCE_DIR}/build.sh DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
    else()
        message(FATAL_ERROR "missing target: __target_${MODULE_NAME}, current_dir: ${CMAKE_CURRENT_SOURCE_DIR}")
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

set(CMAKE_INSTALL_RPATH "$ORIGIN")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

add_subdirectory(external)
add_subdirectory(common)
add_subdirectory(product)

add_dependencies(__target_root
    ${SUBDIRECTORY_TARGETS}
    )