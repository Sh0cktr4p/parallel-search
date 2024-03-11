include(GoogleTest)
include(ctest)

add_executable(tests)
target_include_directories(
    tests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/googletest/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/googletest/
)

target_sources(tests PRIVATE hello_test.cpp)
target_link_libraries(tests gtest gtest_main)

gtest_discover_tests(tests)
