/*
 * @Author: Hanjie Luo luohanjie@gmail.com
 * @Date: 2025-03-11 09:40:14
 * @LastEditors: Hanjie Luo luohanjie@gmail.com
 * @LastEditTime: 2025-03-11 17:05:18
 * @FilePath: /Image-Enhancement-for-SLAM/src/image_enhance_interface.cpp
 * @Description:
 *
 * Copyright (c) 2025 by CVTE, All Rights Reserved.
 */

#include "image_enhance_interface.h"

#include "image_enhance.h"

namespace image_enhance {

ImageEnhanceInterface::ImageEnhanceInterface() noexcept {
}

ImageEnhanceInterface::~ImageEnhanceInterface() noexcept {
}

bool ImageEnhanceInterface::CreateImageEnhance() {
    if (image_enhance_) {
        image_enhance_.reset();
    }

    image_enhance_ = std::make_shared<ImageEnhance>();
    return true;
}

bool ImageEnhanceInterface::Apply(const int img_width,
                                  const int img_height,
                                  const int img_channels,
                                  const void *img,
                                  void **img_enhanced,
                                  const float gamma) {
    if (image_enhance_) {
        cv::Mat cv_img(img_height, img_width, CV_MAKETYPE(CV_8U, img_channels), (void *)img);
        static cv::Mat cv_img_enhanced;
        // cv::Mat cv_img_enhanced(img_height, img_width, CV_MAKETYPE(CV_8U, img_channels), (void *)img_enhanced);

        image_enhance_->Apply(cv_img, cv_img_enhanced, gamma);

        *img_enhanced = cv_img_enhanced.data;

        return true;
    }
    return false;
}

bool ImageEnhanceInterface::Apply2Gray(const int img_width,
                                       const int img_height,
                                       const int img_channels,
                                       const void *img,
                                       void **img_enhanced,
                                       const float gamma) {
    if (image_enhance_) {
        cv::Mat cv_img(img_height, img_width, CV_MAKETYPE(CV_8U, img_channels), (void *)img);
        static cv::Mat cv_img_gray, cv_img_enhanced;

        if (img_channels == 3) {
            cv::cvtColor(cv_img, cv_img_gray, cv::COLOR_BGR2GRAY);
        } else {
            cv_img_gray = cv_img;
        }

        image_enhance_->Apply(cv_img_gray, cv_img_enhanced, gamma);

        *img_enhanced = cv_img_enhanced.data;

        return true;
    }
    return false;
}

}  // namespace image_enhance