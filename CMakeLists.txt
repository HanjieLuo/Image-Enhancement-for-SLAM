cmake_minimum_required(VERSION 3.5)
project(image_enhancement)

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++17 -Wall -fdeclspec -Wno-deprecated-declarations")
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm.*|ARM.*|aarch64.*|AARCH64.*)")
    if (APPLE)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__ARM_NEON__ -DENABLE_NEON -Wno-unused-result -mcpu=apple-m1 -mtune=native")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__ARM_NEON__ -DENABLE_NEON -Wno-unused-result -march=armv8-a+fp+simd+crypto")
    endif()
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -fopenmp")
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# ====================================== LIBRARY ========================================
#是否使用静态版opencv
set(USE_STATIC_OPENCV OFF)

if (NOT ANDROID)
    # OpenCV
    if (USE_STATIC_OPENCV)
        # set(OPENCV_HOME /Users/luohanjie/Softwares/opencv-4.8.1/build_mac_static/install)
        # set(OpenCV_INCLUDE_DIRS ${OPENCV_HOME}/include/opencv4)
        # set(OpenCV_LIBRARY_DIRS
        #     ${OPENCV_HOME}/lib
        #     ${OPENCV_HOME}/lib/opencv4/3rdparty
        # )

        set(OPENCV_HOME /Users/luohanjie/Softwares/opencv-3.4.20/build_mac_static/install)
        set(OpenCV_INCLUDE_DIRS ${OPENCV_HOME}/include)
        set(OpenCV_LIBRARY_DIRS
            ${OPENCV_HOME}/lib
            ${OPENCV_HOME}/share/OpenCV/3rdparty/lib
        )

        set(OpenCV_LIBS opencv_core opencv_imgproc opencv_imgcodecs opencv_highgui opencv_features2d opencv_calib3d opencv_flann opencv_video)

        # MacOS Libraries
        find_library(ACCELERATE_LIBRARY Accelerate)
        find_library(COCOA_LIBRARY Cocoa)
        find_package(OpenCL)

        list(APPEND EXTERNAL_LIBRARIES ${OpenCV_LIBS} tegra_hal libpng libjpeg-turbo ${ACCELERATE_LIBRARY} ${COCOA_LIBRARY} OpenCL::OpenCL z)
    else ()
        find_package(OpenCV REQUIRED)

        set(OpenCV_LIBS opencv_core opencv_imgproc opencv_imgcodecs opencv_highgui opencv_features2d opencv_calib3d opencv_video)

        list(APPEND EXTERNAL_LIBRARIES ${OpenCV_LIBS})
    endif ()

    # Eigen
    find_package(Eigen3 CONFIG REQUIRED)
else ()
    # OpenCV
    if (USE_STATIC_OPENCV)
        set(OPENCV_HOME /Users/luohanjie/Softwares/opencv-3.4.20/build_android_static/install/sdk/native)
        set(OpenCV_INCLUDE_DIRS ${OPENCV_HOME}/jni/include)
        set(OpenCV_LIBRARY_DIRS
            ${OPENCV_HOME}/staticlibs/arm64-v8a
            ${OPENCV_HOME}/3rdparty/libs/arm64-v8a
        )

        # set(OPENCV_HOME ${PROJECT_SOURCE_DIR}/androidprecompilelibs/opencv4_static)
        # set(OpenCV_INCLUDE_DIRS ${OPENCV_HOME}/include)
        # set(OpenCV_LIBRARY_DIRS
        #         ${OPENCV_HOME}/lib/${ARCH_NAME}/
        #         ${OPENCV_HOME}/lib/${ARCH_NAME}/thirdparty
        #         )

        set(OpenCV_LIBS opencv_core opencv_imgproc opencv_imgcodecs opencv_highgui opencv_features2d opencv_calib3d opencv_video opencv_flann)

        list(APPEND EXTERNAL_LIBRARIES ${OpenCV_LIBS} tegra_hal libpng libjpeg-turbo z dl)
    else ()
        set(ANDROID_LIBS ${PROJECT_SOURCE_DIR}/androidprecompilelibs/)
        # add opencv
        include_directories(${ANDROID_LIBS}/opencv/include)
        link_directories(${ANDROID_LIBS}/opencv/lib/${ARCH_NAME}/)
        link_directories(${ANDROID_LIBS}/opencv/lib/${ARCH_NAME}/thirdparty)
        set(OpenCV_LIBS opencv_core opencv_imgproc opencv_imgcodecs opencv_highgui opencv_features2d opencv_calib3d opencv_video opencv_flann)

        list(APPEND EXTERNAL_LIBRARIES ${OpenCV_LIBS})
    endif ()

    # add eigen
    set(EIGEN3_INCLUDE_DIR /Users/luohanjie/Softwares/android/eigen/include /Users/luohanjie/Softwares/android/eigen/include/eigen3)
    # set(EIGEN3_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/androidprecompilelibs/eigen/include {PROJECT_SOURCE_DIR}/androidprecompilelibs/eigen/include/eigen3)

    list(APPEND EXTERNAL_LIBRARIES android log)
endif ()
# ====================================== LIBRARY ========================================

include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${OpenCV_INCLUDE_DIRS}
)

link_directories(
  ${PROJECT_BINARY_DIR}/lib
  ${OpenCV_LIBRARY_DIRS}
)

add_library(image_enhance SHARED "src/fgs_filter.cpp" "src/image_enhance.cpp" "src/image_enhance_interface.cpp")
target_link_libraries(image_enhance PRIVATE ${EXTERNAL_LIBRARIES})

add_executable(test_image_enhance "tests/test_image_enhance.cpp")
target_link_libraries(test_image_enhance image_enhance ${EXTERNAL_LIBRARIES})