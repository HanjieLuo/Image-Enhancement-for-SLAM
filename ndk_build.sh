rm -rf ndk && mkdir -p ndk && cd ndk

export NDK=/Users/luohanjie/Library/Android/sdk/ndk/25.1.8937393

echo $NDK
export CC=/usr/bin/clang-10
export CXX=/usr/bin/clang++-10

cmake \
 -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
 -DANDROID_ABI=arm64-v8a \
 -DANDROID_NATIVE_API_LEVEL=android-29 \
 -DANDROID_STL=c++_shared \
 -DCMAKE_ANDROID_STL_TYPE=c++_shared \
 ..
make -j20
