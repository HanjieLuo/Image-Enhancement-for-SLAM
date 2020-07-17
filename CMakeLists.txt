cmake_minimum_required(VERSION 2.8.3)
project(image_enhancement)

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O3")
add_definitions(-DCOMPILEDWITHC11)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(
  ${PROJECT_SOURCE_DIR}/include
)


link_directories(
  ${PROJECT_BINARY_DIR}/lib
)


add_subdirectory(src)
add_subdirectory(tests)

