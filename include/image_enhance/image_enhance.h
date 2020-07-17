#ifndef IMAGE_ENHANCE_H
#define IMAGE_ENHANCE_H

#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "fgs_filter.h"

class ImageEnhance {
    public:
    ImageEnhance(float gamma = 0.5,
                 double fgs_lambda = 15.,
                 double fgs_sigma_color = 10,
                 double fgs_lambda_attenuation = 0.25,
                 int fgs_num_iter = 3,
                 float contrast_gamma = 1.0,
                 float contrast_to_low = 0.02,   //0.02
                 float contrast_to_high = 0.99, //0.99
                 float resize = 0.5);

    ~ImageEnhance();

    void Apply(const cv::Mat &src, cv::Mat &dst);
    void Apply(const cv::Mat &src, cv::Mat &dst, float gamma);

    // J = low_out +(high_out - low_out).* ((I - low_in)/(high_in - low_in)).^ gamma
    void ImAdjust(const cv::Mat &src, cv::Mat &dst, float gamma = 1.0, int low_in = 3, int high_in = 252, int low_out = 0, int high_out = 255);
    
    void ContrastEnhance(const cv::Mat &src, cv::Mat &dst, float gamma = 1.0, float to_low = 0.01, float to_high = 0.99);

    void ShowMat(const std::string name, const cv::Mat &src);
    // float CalcGammaValue(const cv::Mat &img);

    private:
    double fgs_lambda_;
    double fgs_sigma_color_;
    double fgs_lambda_attenuation_;
    int fgs_num_iter_;
    float resize_;
    float eps_;

    int num_stripes_;
    float gamma_;

    cv::Mat img_src_f_;
    cv::Mat img_src_resize_;
    cv::Mat img_src_f_resize_;
    cv::Mat img_norm_;
    cv::Mat img_fgs_;
    cv::Mat img_illumination_;
    cv::Mat img_recovery_;
    cv::Mat img_recovery_uint8_;
    cv::Mat img_recovery_uint8_filter_;

    float contrast_gamma_;
    float contrast_to_low_;
    float contrast_to_high_;
    int hist_[256];
    cv::Mat lookup_table_;
    // float cdf_[256];

    // cv::Mat img_src_;
    // cv::Mat img_norm_;
    // cv::Mat img_fgs_;
    // cv::Mat img_illumination_;

    

   };
#endif