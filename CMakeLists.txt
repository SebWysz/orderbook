cmake_minimum_required(VERSION 3.20)

project(lob
    VERSION 0.1.0
    DESCRIPTION "Limit order book practice project"
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(LOB_ENABLE_WARNINGS "Enable extra compiler warnings" ON)
option(LOB_ENABLE_ASAN "Enable address sanitizer in Debug" OFF)
option(LOB_BUILD_TESTS "Build tests" ON)

add_library(lob_lib
    src/event.cpp
    src/order_book.cpp
)

target_include_directories(lob_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(lob_lib PUBLIC cxx_std_23)

if(LOB_ENABLE_WARNINGS)
    if(MSVC)
        target_compile_options(lob_lib PRIVATE /W4 /permissive-)
    else()
        target_compile_options(lob_lib PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
    endif()
endif()

if(LOB_ENABLE_ASAN AND NOT MSVC)
    target_compile_options(lob_lib PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address -fno-omit-frame-pointer>
    )
    target_link_options(lob_lib PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address>
    )
endif()

add_executable(lob_main
    src/main.cpp
)

target_link_libraries(lob_main PRIVATE lob_lib)

enable_testing()

if(LOB_BUILD_TESTS)
    add_subdirectory(tests)
endif()
