add_executable(arc_test
    main_test.cpp
)

target_link_libraries(arc_test PRIVATE engine)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1 
)
FetchContent_MakeAvailable(googletest)

add_executable(engine_tests gtest_main.cpp)

target_link_libraries(engine_tests PRIVATE engine gtest_main)

include(GoogleTest)
gtest_discover_tests(engine_tests)