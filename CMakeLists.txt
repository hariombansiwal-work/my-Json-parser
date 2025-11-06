cmake_minimum_required(VERSION 3.12)
project(JsonParser VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags following Google C++ style guide
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(JSON_PARSER_SOURCES
    src/json_value.cpp
    src/json_object.cpp
    src/json_array.cpp
    src/json_parser.cpp
    src/json_writer.cpp
    src/json_visitor.cpp
    src/json_builder.cpp
    src/json_utils.cpp
)

# Create library
add_library(json_parser ${JSON_PARSER_SOURCES})

# Example executable
add_executable(json_parser_example examples/example.cpp)
target_link_libraries(json_parser_example json_parser)

# Google Test
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    # Test executable
    add_executable(json_parser_tests
        tests/test_json_value.cpp
        tests/test_json_object.cpp
        tests/test_json_array.cpp
        tests/test_json_parser.cpp
        tests/test_json_writer.cpp
        tests/test_json_visitor.cpp
        tests/test_json_builder.cpp
        tests/test_json_utils.cpp
        tests/test_main.cpp
    )
    target_link_libraries(json_parser_tests json_parser gtest_main)
    include(GoogleTest)
    gtest_discover_tests(json_parser_tests)
endif()

# Installation
install(TARGETS json_parser
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY include/json_parser
    DESTINATION include
)

