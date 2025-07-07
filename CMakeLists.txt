
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



# Unit test executable

# Fetch CppUTest
include(FetchContent)
FetchContent_Declare(
    cpputest
    GIT_REPOSITORY https://github.com/cpputest/cpputest.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(cpputest)


# Unit test executable
add_executable(cpp_buffer_tests test/main.cpp)
target_link_libraries(cpp_buffer_tests PUBLIC cpp_buffer_impl cpp_buffer PRIVATE CppUTest CppUTestExt)
# target_include_directories(cpp_buffer_tests PUBLIC include)

# Enable testing and add test
include(CTest)
enable_testing()
add_test(NAME cpp_buffer_tests COMMAND cpp_buffer_tests)
