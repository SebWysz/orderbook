include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.4
)

FetchContent_MakeAvailable(Catch2)

add_executable(lob_tests
    test_order_book.cpp
)

target_link_libraries(lob_tests
    PRIVATE
        lob_lib
        Catch2::Catch2WithMain
)

include(CTest)
include(Catch)

catch_discover_tests(lob_tests)
