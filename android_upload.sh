#!/bin/bash

PROJECT_NAME=image_enhance
DST_LIB_PATH=/data/local/tmp/${PROJECT_NAME}/lib/
DST_BIN_PATH=/data/local/tmp/${PROJECT_NAME}/bin/
DST_DATA_PATH=/data/local/tmp/${PROJECT_NAME}/data/

adb root
adb shell "mkdir -p ${DST_LIB_PATH}"
adb shell "mkdir -p ${DST_BIN_PATH}"
adb shell "mkdir -p ${DST_DATA_PATH}"

adb push ./build/bin/* ${DST_BIN_PATH}

adb push ./data/* ${DST_DATA_PATH}

adb push ./build/lib/*.so ${DST_LIB_PATH}
adb push /Users/luohanjie/Softwares/opencv-4.8.1/build_android/install/sdk/native/libs/arm64-v8a/libopencv_imgcodecs.so ${DST_LIB_PATH}
adb push /Users/luohanjie/Softwares/opencv-4.8.1/build_android/install/sdk/native/libs/arm64-v8a/libopencv_highgui.so ${DST_LIB_PATH}
adb push /Users/luohanjie/Softwares/opencv-4.8.1/build_android/install/sdk/native/libs/arm64-v8a/libopencv_imgproc.so ${DST_LIB_PATH}
adb push /Users/luohanjie/Softwares/opencv-4.8.1/build_android/install/sdk/native/libs/arm64-v8a/libopencv_core.so ${DST_LIB_PATH}
adb push /Users/luohanjie/Softwares/opencv-4.8.1/build_android/3rdparty/lib/arm64-v8a/*.a ${DST_LIB_PATH}

adb push /Users/luohanjie/Library/Android/sdk/ndk/25.1.8937393/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/lib/aarch64-linux-android/libc++_shared.so ${DST_LIB_PATH}
adb push /Users/luohanjie/Library/Android/sdk/ndk/25.1.8937393/toolchains/llvm/prebuilt/darwin-x86_64/lib64/clang/14.0.6/lib/linux/aarch64/libomp.so ${DST_LIB_PATH}

# adb shell
# cd /data/local/tmp/image_enhance/
# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data/local/tmp/image_enhance/lib/ 
