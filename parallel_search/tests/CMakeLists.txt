include(GoogleTest)
include(ctest)

add_executable(tests)
target_include_directories(
    tests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/googletest/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/googletest/
)
target_link_libraries(tests ${PROJECT_NAME}_lib)

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/tests/*.cpp)
target_sources(tests PRIVATE ${TEST_SRC_FILES})
target_link_libraries(tests gtest gtest_main)

gtest_discover_tests(tests)
