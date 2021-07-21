cmake_minimum_required(VERSION 3.1.0)
project(consolebuffer 
    VERSION 0.1.0
    DESCRIPTION ""
)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
endif()

if(CMAKE_CXX_STANDARD LESS 17)
    message(FATAL_ERROR " C++ incompatible standard ${CMAKE_CXX_STANDARD}. Required C++17")
endif()



if (NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CXX_COMPILER "g++")
    #I prefer executable files in bin/ instead build/. Change it if you like.
    #set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin") # build/bin/
    #set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin) # bin/
endif()


#Compiler options
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /permissive-")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")# -Werror")
endif()



#OS options
option(MINGW_STATIC_LINK "Link MinGW dependencies statically." ON)
option(MSVC_STATIC_LINK "Link MSVC static C++ runtime library." OFF)
if(WIN32)
    message("Windows OS detected.")
    if(MSVC AND MSVC_STATIC_LINK)
        message("Linking MSVC C++ runtime statically...")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MD /MT")
    endif()
    if(NOT MSVC AND MINGW_STATIC_LINK)
        message("Linking MinGW dependencies statically...")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static -pthread")
        endif()
endif()


include_directories ("${PROJECT_SOURCE_DIR}/include")

#Build type options
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building Debug...")
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Building Release...") 
endif()

include_directories(src include)
add_executable(benchmark_80x20 
    test/benchmark_80x20.cpp
)
add_executable(benchmark_triple 
    test/benchmark_triple.cpp
)
add_executable(benchmark_triple_utf8
    test/benchmark_triple_utf8.cpp
)
add_executable(test_2d_buffer 
    test/test_2d_buffer.cpp
)

add_executable(BORRAME 
    test/BORRAME.cpp
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
