include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

add_executable(matrix_tests test_matrix.cpp)
target_link_libraries(matrix_tests
    PRIVATE
        lin_alg
        GTest::gtest_main
)

add_executable(rational_tests test_rational.cpp)
target_link_libraries(rational_tests
    PRIVATE
        lin_alg
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(matrix_tests)
gtest_discover_tests(rational_tests)
