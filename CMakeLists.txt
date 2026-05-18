cmake_minimum_required(VERSION 3.16)
project(sts C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(sts_lib
    src/card.c
    src/deck.c
    src/game.c
    src/random.c
    src/sts_io.c
)
target_include_directories(sts_lib PUBLIC include)

add_executable(sts main.c)
target_link_libraries(sts PRIVATE sts_lib)

include(CTest)

if(BUILD_TESTING)
    macro(add_sts_test name src_file)
        add_executable(${name} ${src_file})
        target_link_libraries(${name} PRIVATE sts_lib)
        add_test(NAME ${name} COMMAND $<TARGET_FILE:${name}>)
        set_tests_properties(${name} PROPERTIES LABELS "unit" TIMEOUT 5)
    endmacro()

    add_sts_test(check_card tests/test_card.c)
    add_sts_test(check_deck tests/test_deck.c)
    add_sts_test(check_game tests/test_game.c)
endif()
