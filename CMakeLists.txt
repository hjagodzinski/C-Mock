
cmake_minimum_required(VERSION 3.14)

if( POLICY CMP0048 )
    cmake_policy(SET CMP0048 NEW) # project version
endif()

if( POLICY CMP0076 )
    cmake_policy(SET CMP0076 NEW) # full paths
endif()

if( POLICY CMP0077 )
    cmake_policy(SET CMP0077 NEW) # options do nothing when defined as variable.
endif()

option(BUILD_TESTING "Build tests for C-Mock" OFF)

project(cmock VERSION 0.5.0 LANGUAGES CXX)

# -------------------------------------------------------------------
# Workaround for cmake 3.24.1
if(NOT CMAKE_C_COMPILE_OBJECT)
    set(CMAKE_C_COMPILE_OBJECT ${CMAKE_C_COMPILE_SHARED} CACHE STRING "Don't know why this is missing")
endif()
if(NOT CMAKE_C_ARCHIVE_CREATE)
    set(CMAKE_C_ARCHIVE_CREATE ${CMAKE_C_CREATE_STATIC_LIBRARY} CACHE STRING "Don't know why this is missing")
endif()
if(NOT CMAKE_C_ARCHIVE_FINISH)
    set(CMAKE_C_ARCHIVE_FINISH "" CACHE STRING "Don't know why this is missing")
endif()


# -------------------------------------------------------------------
# If not already declared - get gmock.
if (BUILD_TESTING AND NOT TARGET gmock)
    include(FetchContent)

    FetchContent_Declare( googletest
        GIT_REPOSITORY    https://github.com/google/googletest.git
        GIT_TAG           release-1.12.1
    )

    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) 
    FetchContent_MakeAvailable(googletest)
    set(GTEST_ROOT ${gootletest_SOURCE_DIR} CACHE STRING "Root dir for find(GTest)")
endif()

# -------------------------------------------------------------------
add_library(gmock_c INTERFACE)

target_sources(gmock_c
    INTERFACE
        include/cmock/cmock-function-class-mockers.h
        include/cmock/cmock-function-mockers.h
        include/cmock/cmock-internal.h
        include/cmock/cmock-spec-builders.h
        include/cmock/cmock.h
)

target_include_directories(gmock_c SYSTEM
    INTERFACE
        include
)

target_link_libraries( gmock_c
    INTERFACE
        gmock
        ${CMAKE_DL_LIBS}
)

# -------------------------------------------------------------------
if (BUILD_TESTING)
    add_subdirectory(test)
endif()