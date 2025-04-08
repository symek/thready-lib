include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(threadpool_tests test_threadpools.cpp)
target_link_libraries(threadpool_tests PRIVATE threadpool Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(threadpool_tests)
