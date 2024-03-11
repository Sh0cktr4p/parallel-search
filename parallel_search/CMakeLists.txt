cmake_minimum_required(VERSION 3.29)

set(PROJECT_NAME parallel_search)
project(${PROJECT_NAME} VERSION 0.0.1)

set(CMAKE_CXX_FLAGS "-g -Wall -static-libstdc++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(${PROJECT_NAME}_lib ${SRC_FILES})

enable_testing()

add_subdirectory(tests)
add_subdirectory(external/googletest)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    src/test_inputs.cpp
)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib)
