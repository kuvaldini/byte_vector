CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

enable_testing()

execute_process(
    COMMAND sh -c "git -C ${CMAKE_CURRENT_LIST_DIR}/googletest rev-parse \
       || git clone https://github.com/google/googletest ${CMAKE_CURRENT_LIST_DIR}/googletest --single-branch --depth 1"
    RESULTS_VARIABLE STATUS
    ## TODO with CMake version 3.19 COMMAND_ERROR_IS_FATAL ANY
)
if(STATUS AND NOT ${STATUS} EQUAL 0)
    message(FATAL_ERROR "Failed to clone googletest")
endif()
add_subdirectory(googletest)

add_executable(test-byte_vector
    test-byte_range_ascii.cpp
    test-byte_range_hex.cpp
    test-byte_vector.cpp
    test-byte_range.cpp
    )
target_compile_features(test-byte_vector PRIVATE cxx_std_17)
target_link_libraries(test-byte_vector PRIVATE 
    gtest_main gtest 
    byte_vector)
add_test(NAME test-byte_vector COMMAND test-byte_vector)
# target_include_directories(test-byte_vector PRIVATE ${CMAKE_BINARY_DIR}/..) ## For rev_label.hpp

##maybe todo for CTest
#include(GoogleTest)
#gtest_discover_tests(test-byte_vector)
