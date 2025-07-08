
cmake_minimum_required(VERSION 3.10)
project(cpp_buffer)

# Option to build as static library (OFF = header-only)
option(CPP_BUFFER_STATIC "Build cpp_buffer as a static library (ON) or header-only (OFF)" OFF)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Header files (for IDEs, not strictly needed by CMake)
set(HEADERS
    include/cpp_buffer/buffer_assert.h
    include/cpp_buffer/buffer_definitions.h
    include/cpp_buffer/buffer.h
)

# create header-only library
add_library(cpp_buffer INTERFACE)
target_include_directories(cpp_buffer INTERFACE include)

# create target for optional compiled library
add_library(cpp_buffer_impl STATIC
    src/buffer_assert.cpp
)
target_link_libraries(cpp_buffer_impl PUBLIC cpp_buffer)


if(CPP_BUFFER_STATIC)
    add_library(CPPBuffer::cpp_buffer ALIAS cpp_buffer_impl)
else()
    add_library(CPPBuffer::cpp_buffer ALIAS cpp_buffer)
endif()



# Fetch CppUTest
find_package(CppUTest)
if(NOT CppUTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        CppUTest
        GIT_REPOSITORY https://github.com/cpputest/cpputest.git
        GIT_TAG        4363c47a86125c8b3557e58de0032d16a2c7f7ea
    )
    # set options on cpputest
    set(C++11 ON CACHE BOOL "Tell cpputest to build for C++11")
    set(MEMORY_LEAK_DETECTION OFF CACHE BOOL "Tell cpputest to disable memory leak tests")
    set(EXTENSIONS OFF CACHE BOOL "tell cpputest to disable extensions")
    FetchContent_MakeAvailable(CppUTest)
endif()


# Unit test executable
add_executable(cpp_buffer_tests tests/main.cpp)
target_link_libraries(cpp_buffer_tests
    PUBLIC
        cpp_buffer
    PRIVATE
        CppUTest
)
if(CPP_BUFFER_STATIC)
    target_link_libraries(cpp_buffer_tests 
    PUBLIC 
        cpp_buffer_impl 
    )
endif()

# Enable testing and add test
include(CTest)
enable_testing()
add_test(NAME cpp_buffer_tests COMMAND cpp_buffer_tests)
