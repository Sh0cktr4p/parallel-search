cmake_minimum_required(VERSION 3.29)

set(PROJECT_NAME parallel_search)
project(${PROJECT_NAME} VERSION 0.0.1)

set(CMAKE_CXX_FLAGS "-g -Wall -static-libstdc++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(${PROJECT_NAME}_lib ${SRC_FILES})

enable_testing()

message(STATUS "Looking for OpenMP")
find_package(OpenMP)
IF(OPENMP_FOUND)
    message("Found OpenMP")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_SHARED_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
ELSE()
    message("OpenMP was not found")
ENDIF()

target_link_libraries(${PROJECT_NAME}_lib ${OpenMP_CXX_FLAGS})

add_subdirectory(tests)
add_subdirectory(external/googletest)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    ${SRC_FILES}
)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib)
