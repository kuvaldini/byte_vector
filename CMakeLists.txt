cmake_minimum_required(VERSION 3.10)

project(byte_vector LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(byte_vector INTERFACE)
target_include_directories(byte_vector INTERFACE .)

add_custom_target(byte_vector_amalgamate
   WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
   COMMAND ${CMAKE_CURRENT_LIST_DIR}/amalgamate.sh ${CMAKE_CURRENT_BINARY_DIR}/byte_vector.hh
   SOURCES
      byte_vector.hpp
      byte_range_view.hpp
      byte_range_ascii.hpp
      byte_range_hex.hpp
)

include(CTest)
if(BUILD_TESTING) ## By default CMake provides ON
   add_subdirectory(tests)
endif()
