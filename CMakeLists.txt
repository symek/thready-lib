cmake_minimum_required(VERSION 3.22)
project(thready)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

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

# Optional: Catch2 test suite
#enable_testing()
#add_subdirectory(tests)
