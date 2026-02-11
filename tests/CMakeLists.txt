# tests/CMakeLists.txt
cmake_minimum_required(VERSION 3.14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

set(TO_TEST
    arraylist
    clioptions
    linkedlist
)

set(TO_TEST_CPP
    youtube
)

add_library(sncltest ../source/sncl_test.c)

foreach(TEST_TOOL IN LISTS TO_TEST)
    set(TEST_NAME test_${TEST_TOOL})
    set(SRC
        test_${TEST_TOOL}.c
        ../source/sncl_${TEST_TOOL}.c
    )

    add_executable(${TEST_NAME} ${SRC})

    target_compile_options(${TEST_NAME} PRIVATE -std=c99 -Wall -Wextra -O0 -g)
    target_include_directories(${TEST_NAME} PRIVATE ../include)
    target_link_libraries(${TEST_NAME} PRIVATE sncltest)

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

foreach(TEST_TOOL IN LISTS TO_TEST_CPP)
    set(TEST_NAME test_${TEST_TOOL})
    set(SRC
        test_${TEST_TOOL}.cpp
        ../source/sncl_${TEST_TOOL}.cpp
    )

    add_executable(${TEST_NAME} ${SRC})

    target_compile_options(${TEST_NAME} PRIVATE -std=c++20 -Wall -Wextra -O0 -g)
    target_include_directories(${TEST_NAME} PRIVATE ../include)
    target_link_libraries(${TEST_NAME} PRIVATE sncltest)

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
