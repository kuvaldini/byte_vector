cmake_minimum_required(VERSION 3.10)

project(byte_vector LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(byte_vector INTERFACE)
target_include_directories(byte_vector INTERFACE .)
# byte_vector.hpp
# byte_vector_view.hpp
# byte_range_ascii.hpp
# bytes_hex.hpp
# local_type_traits.hpp
# std_to_ostream.hpp
# )

include(CTest)
if(BUILD_TESTING) ## By default CMake provides ON
  add_subdirectory(tests)
endif()
