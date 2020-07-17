cmake_minimum_required(VERSION 2.8.3)

find_package(OpenCV REQUIRED)

include_directories(
  ${OpenCV_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/thirdparty/opencv-3.4
)

link_directories(
  ${OpenCV_LIBRARY_DIRS}
)

add_executable(test_image_enhance test_image_enhance.cpp)
target_link_libraries(test_image_enhance image_enhance  ${OpenCV_LIBS})