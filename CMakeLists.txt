cmake_minimum_required(VERSION 3.22)
project(thready)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

find_package(Threads REQUIRED)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug build type detected.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g -O0")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
endif ()
# Add library
add_library(thready STATIC
        include/thready/LockFreeQueue.hpp
        include/thready/MutexQueue.hpp
        include/thready/RingBufferQueue.hpp
        include/thready/ThreadPoolBase.hpp
        include/thready/ThreadPoolBlocking.hpp
        include/thready/ThreadPoolHybrid.hpp
        include/thready/ThreadPools.hpp
        include/thready/ThreadPoolSpinning.hpp
        src/ThreadPools.cpp
        )

target_include_directories(thready
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

add_executable(example
        example.cpp
        )

target_link_libraries(example
        thready
        pthread
        )

find_package(benchmark REQUIRED)
if (NOT benchmark_FOUND)
    message(FATAL_ERROR "Google Benchmark not found. Please install it or set the path to it.")
else ()
    message(STATUS "Google Benchmark found.")

    # Add benchmark executable
    add_executable(benchmark benchmark.cpp)

    # Link Google Benchmark library
    target_link_libraries(benchmark PRIVATE benchmark::benchmark benchmark::benchmark_main)

    # Link your thread pool library
    target_link_libraries(benchmark PRIVATE thready)
endif ()


# Optional: Catch2 test suite
enable_testing()
add_subdirectory(tests)
