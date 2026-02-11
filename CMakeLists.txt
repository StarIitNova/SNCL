cmake_minimum_required(VERSION 3.14)
project(SNCL C CXX)

option(SNCL_BUILD_TESTS "Enable test executable building" ON)

set(SNCL_C_STANDARD "99" CACHE STRING "C standard to use (e.g., 99, 11, 17)")
set(SNCL_CXX_STANDARD "20" CACHE STRING "C++ standard to use (e.g., 17, 20, 23)")

option(SNCL_C_ARRAYLISTS "Enable C Arraylists tool" ON)
option(SNCL_C_LINKEDLIST "Enable C Linkedlists tool" ON)
option(SNCL_C_LEXER "Enable C lexer" ON)
option(SNCL_C_CLI_OPTIONS "Enable C CLI Options tool" ON)
option(SNCL_CPP_YOUTUBE_TOOLS "Enable C++ Youtube tools" ON)

set(SNCL_SOURCES)

message(STATUS "Detecting enabled SNCL features:")
if(SNCL_C_ARRAYLISTS)
    message(STATUS " - [C]   Arraylists tool enabled")
    list(APPEND SNCL_SOURCES source/sncl_arraylist.c)
endif()

if(SNCL_C_LINKEDLIST)
    message(STATUS " - [C]   linkedlists tool enabled")
    list(APPEND SNCL_SOURCES source/sncl_linkedlist.c)
endif()

if(SNCL_C_LEXER)
    message(STATUS " - [C]   Arraylists tool enabled")
    list(APPEND SNCL_SOURCES source/sncl_clex.c)
endif()

if(SNCL_C_CLI_OPTIONS)
    message(STATUS " - [C]   Cli options tool enabled")
    list(APPEND SNCL_SOURCES source/sncl_clioptions.c)
endif()

if(SNCL_CPP_YOUTUBE_TOOLS)
    message(STATUS " - [C++] Youtube tools enabled")
    list(APPEND SNCL_SOURCES source/sncl_youtube.cpp)
endif()

add_library(sncl STATIC ${SNCL_SOURCES})
target_include_directories(sncl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(sncl PROPERTIES
    C_STANDARD ${SNCL_C_STANDARD}
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
    CXX_STANDARD ${SNCL_CXX_STANDARD}
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if(SNCL_BUILD_TESTS)
    message(STATUS "SNCL tests enabled!")
    enable_testing()
    add_subdirectory(tests)
endif()
