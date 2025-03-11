/*
 * @Author: Hanjie Luo luohanjie@gmail.com
 * @Date: 2025-03-11 09:39:55
 * @LastEditors: Hanjie Luo luohanjie@gmail.com
 * @LastEditTime: 2025-03-11 17:05:33
 * @FilePath: /Image-Enhancement-for-SLAM/include/image_enhance_interface.h
 * @Description:
 *
 * Copyright (c) 2025 by CVTE, All Rights Reserved.
 */

#pragma once

#include <cstdint>
#include <string>

#ifdef _WIN32

#ifdef IMAGE_ENHANCE_EXPORT_INTERFACE
#define IMAGE_ENHANCE_CXX_INTERFACE __declspec(dllexport)
#else
#define IMAGE_ENHANCE_CXX_INTERFACE __declspec(dllimport)
#endif

#else

#ifdef IMAGE_ENHANCE_EXPORT_INTERFACE
#define IMAGE_ENHANCE_CXX_INTERFACE __attribute__((visibility("default")))
#else
#define IMAGE_ENHANCE_CXX_INTERFACE
#endif

#endif

#define IMAGE_ENHANCE_C_INTERFACE extern "C" IMAGE_ENHANCE_CXX_INTERFACE
namespace image_enhance {
class ImageEnhance;

class IMAGE_ENHANCE_CXX_INTERFACE ImageEnhanceInterface {
   public:
    ImageEnhanceInterface() noexcept;
    ~ImageEnhanceInterface() noexcept;

   public:
    /**
     * 构造函数。
     */
    bool CreateImageEnhance();

    /**
     * 对输入的图像进行增强处理。
     * @param {const int} img_width - 目标图宽，单位为像素。
     * @param {const int} img_height - 目标图高，单位为像素。
     * @param {const int} img_channels - 图的通道数，1或者3。
     * @param {const void *} img - 指向目标图的像素矩阵指针，像素以opencv规定排列。如果是彩色图则格式为BGR。
     * @param [out] {void **} img_enhanced - 输出结果图片的内存块指针，返回图像大小和通道与img一致。
     * @param {const float} gamma - gamma变换系数,值越大,增强后图像越亮。
     * @return {bool} 是否成功运行.
     */
    bool Apply(const int img_width,
               const int img_height,
               const int img_channels,
               const void *img,
               void **img_enhanced,
               const float gamma);

    /**
     * 对输入的图像进行增强处理，并且将增强后的图像转为灰度图。
     * @param {const int} img_width - 目标图宽，单位为像素。
     * @param {const int} img_height - 目标图高，单位为像素。
     * @param {const int} img_channels - 图的通道数，1或者3。
     * @param {const void *} img - 指向目标图的像素矩阵指针，像素以opencv规定排列。如果是彩色图则格式为BGR。
     * @param [out] {void **} img_enhanced - 输出结果图片的内存块指针，返回图像大小与img一致，并且转为灰度图。
     * @param {const float} gamma - gamma变换系数,值越大,增强后图像越亮。
     * @return {bool} 是否成功运行.
     */
    bool Apply2Gray(const int img_width,
                    const int img_height,
                    const int img_channels,
                    const void *img,
                    void **img_enhanced,
                    const float gamma);

   private:
    std::shared_ptr<ImageEnhance> image_enhance_ = nullptr;
};

}  // namespace image_enhance
