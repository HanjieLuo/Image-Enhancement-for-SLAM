cmake_minimum_required(VERSION 2.8.3)

find_package(OpenCV REQUIRED)

include_directories(
  ${OpenCV_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/thirdparty/opencv-3.4
)

link_directories(
  ${OpenCV_LIBRARY_DIRS}
)

set(IMAGE_ENHANCE_LIBRARIES
  ${OpenCV_LIBS}
)

add_library(fgs_filter SHARED fgs_filter.cpp)
target_link_libraries(fgs_filter ${IMAGE_ENHANCE_LIBRARIES})

add_library(image_enhance SHARED image_enhance.cpp)
target_link_libraries(image_enhance fgs_filter ${IMAGE_ENHANCE_LIBRARIES})