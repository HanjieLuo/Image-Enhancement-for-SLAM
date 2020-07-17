#include "image_enhance/image_enhance.h"

ImageEnhance::ImageEnhance(float gamma,
                           double fgs_lambda,
                           double fgs_sigma_color,
                           double fgs_lambda_attenuation,
                           int fgs_num_iter,
                           float contrast_gamma,
                           float contrast_to_low,
                           float contrast_to_high,
                           float resize) {
    gamma_ = gamma;
    fgs_lambda_ = fgs_lambda;
    fgs_sigma_color_ = fgs_sigma_color;
    fgs_lambda_attenuation_ = fgs_lambda_attenuation;
    fgs_num_iter_ = fgs_num_iter;
    resize_ = resize;

    eps_ = 0.001;
    num_stripes_ = cv::getNumThreads();

    contrast_gamma_ = contrast_gamma;
    contrast_to_low_ = contrast_to_low;
    contrast_to_high_ = contrast_to_high;

    lookup_table_.create(1, 256, CV_8U);
}

ImageEnhance::~ImageEnhance() {
}

void ImageEnhance::Apply(const cv::Mat &src, cv::Mat &dst) {
    // CalcGammaValue(src);

    src.convertTo(img_src_f_, CV_32F);

    cv::resize(src, img_src_resize_, cv::Size(), resize_, resize_);
    img_src_resize_.convertTo(img_src_f_resize_, CV_32F);

    cv::resize(img_src_f_, img_src_f_resize_, cv::Size(), resize_, resize_);

    img_norm_ = img_src_f_resize_ / 255.0 + eps_;

    int ch = img_norm_.channels();
    int w_resize = img_norm_.cols;
    int h_resize = img_norm_.rows;

    fastGlobalSmootherFilter(img_src_resize_, img_norm_, img_fgs_, fgs_lambda_, fgs_sigma_color_, fgs_lambda_attenuation_, fgs_num_iter_);

    // ShowMat("img_fgs", img_fgs_);
    int cols = w_resize * ch;
    int rows = h_resize;

    if (img_fgs_.isContinuous()) {
        cols *= rows;
        rows = 1;
    }

    int stripe_sz = (int)ceil((float)cols / num_stripes_);

    cv::parallel_for_(cv::Range(0, num_stripes_), [&](const cv::Range &range) {
        int start = std::min(range.start * stripe_sz, cols);
        int end = std::min(range.end * stripe_sz, cols);

        int x, y;
        for (y = 0; y < rows; y++) {
            float *p = img_fgs_.ptr<float>(y);
            for (x = start; x < end; x++) {
                p[x] = pow(p[x], gamma_);
            }
        }
    });

    cv::resize(img_fgs_, img_illumination_, src.size(), 0, 0);

    // ShowMat("img_illumination", img_illumination_);66

    // cv::waitKey(0);

    img_recovery_ = img_src_f_.mul(1 / img_illumination_);

    img_recovery_.convertTo(img_recovery_uint8_, CV_8U);

    // cv::imwrite("img_recovery_uint8_1.jpg", img_recovery_uint8_);

    /*
    @param sigmaColor Filter sigma in the color space. A larger value of the parameter means that
    farther colors within the pixel neighborhood (see sigmaSpace) will be mixed together, resulting
    in larger areas of semi-equal color.
    @param sigmaSpace Filter sigma in the coordinate space. A larger value of the parameter means that
    farther pixels will influence each other as long as their colors are close enough (see sigmaColor
    ). When d\>0, it specifies the neighborhood size regardless of sigmaSpace. Otherwise, d is
    proportional to sigmaSpace.
    */
    // cv::imwrite("before_bilateralFilter.jpg", img_recovery_uint8_);
    cv::bilateralFilter(img_recovery_uint8_, img_recovery_uint8_filter_, 5, 10, 10);
    // cv::imwrite("after_bilateralFilter.jpg", img_recovery_uint8_filter_);
    // cv::bilateralFilter(img_recovery_uint8_, dst, 5, 10, 10);

    // cv::imshow("before_gamma", img_recovery_uint8_filter_);
    //  cv::bilateralFilter(img_recovery_uint8_, dst, 5, 10, 10);
    ContrastEnhance(img_recovery_uint8_filter_, dst, contrast_gamma_, contrast_to_low_, contrast_to_high_);

    // cv::imshow("img_recovery_uint8_filter", img_recovery_uint8_filter_);
    // struct timeval t1, t2;

    // gettimeofday(&t1, NULL);
    // ImAdjust(img_recovery_uint8_filter_, dst, 1.0);
    // gettimeofday(&t2, NULL);

    // std::cout << "Time: " << (t2.tv_sec - t1.tv_sec) * 1000 + (t2.tv_usec - t1.tv_usec) / 1000 << "ms" << std::endl;

    // cv::imwrite("after_filter.jpg", dst);

    // double min;
    // double max;

    // cv::minMaxIdx(img_illumination_, &min, &max);
    // std::cout<<min<<", "<<max<<std::endl;
}

void ImageEnhance::Apply(const cv::Mat &src, cv::Mat &dst, float gamma) {
    gamma_ = gamma;
    Apply(src, dst);
}

void ImageEnhance::ImAdjust(const cv::Mat &src, cv::Mat &dst, float gamma, int low_in, int high_in, int low_out, int high_out) {
    cv::Mat lookup_table(1, 256, CV_8U);

    float k = (float(high_out) - low_out) / (high_in - low_in);

    uchar *p = lookup_table.ptr();
    for (int i = 0; i < 256; ++i) {
        // J = low_out +(high_out - low_out).* ((I - low_in)/(high_in - low_in)).^ gamma
        if (i <= low_in) {
            p[i] = low_out;
        } else if (i >= high_in) {
            p[i] = high_out;
        } else {
            p[i] = cv::saturate_cast<uchar>(pow(k * (i - low_in) + low_out, gamma));
        }
        // std::cout<<int(p[i])<<std::endl;
    }

    cv::LUT(src, lookup_table, dst);
}

void ImageEnhance::ContrastEnhance(const cv::Mat &img_input, cv::Mat &img_output, float gamma, float to_low, float to_high) {
    int i, j;
    float size = img_input.rows * img_input.cols;

    cv::Mat src, dst, hsv;
    std::vector<cv::Mat> hsv_planes;
    if (img_input.channels() == 3) {
        cv::cvtColor(img_input, hsv, cv::COLOR_BGR2HSV);
        split(hsv, hsv_planes);
        src = hsv_planes[2];

        // cv::imshow("v", src);
    } else {
        src = img_input;
    }

    memset(hist_, 0, sizeof(hist_));

    const uchar *p_img;
    for (i = 0; i < src.rows; i++) {
        p_img = src.ptr<uchar>(i);
        for (j = 0; j < src.cols; j++) {
            hist_[p_img[j]]++;
        }
    }

    //cumu
    float cdf_value, sum = 0.;
    int ilow = 0;
    int ihigh = 255;
    bool flag_low = true, flag_high = true;

    for (i = 0; i < 256; i++) {
        sum = sum + hist_[i];
        cdf_value = sum / size;

        if (flag_low && (cdf_value > to_low)) {
            ilow = i;
            flag_low = false;
        }

        if (flag_high && (cdf_value > to_high)) {
            ihigh = i;
            flag_high = false;
        }
        // std::cout<<i<<":"<<cdf_value<<","<<std::endl;
    }
    // std::cout<<"ilow:"<<ilow<<", ihigh:"<<ihigh<<std::endl;

    float k = 255.0 / (ihigh - ilow);

    uchar *p = lookup_table_.ptr();
    for (i = 0; i < 256; ++i) {
        // J = low_out +(high_out - low_out).* ((I - low_in)/(high_in - low_in)).^ gamma
        if (i <= ilow) {
            p[i] = 0;
        } else if (i >= ihigh) {
            p[i] = 255;
        } else {
            p[i] = cv::saturate_cast<uchar>(pow(k * (i - ilow), gamma));
        }
        // std::cout<<int(p[i])<<std::endl;
    }

    cv::LUT(src, lookup_table_, dst);

    // cv::imshow("dst", dst);

    if (img_input.channels() == 3) {
        hsv_planes[2] = dst;
        merge(hsv_planes, hsv);
        cv::cvtColor(hsv, img_output, cv::COLOR_HSV2BGR);
    } else {
        img_output = dst;
    }
}

void ImageEnhance::ShowMat(const std::string name, const cv::Mat &src) {
    double min;
    double max;
    cv::minMaxIdx(src, &min, &max);
    cv::Mat adjMap;

    float scale = 255 / (max - min);
    src.convertTo(adjMap, CV_8UC1, scale, -min * scale);

    cv::Mat falseColorsMap;
    cv::applyColorMap(adjMap, falseColorsMap, cv::COLORMAP_HOT);
    // cv::imwrite("fgs.jpg", falseColorsMap);
    cv::imshow(name, falseColorsMap);
}

/*
float ImageEnhance::CalcGammaValue(const cv::Mat &img) {
    cv::Scalar mean, stddev;
    float gamma;

    cv::meanStdDev(img, mean, stddev);

    gamma = stddev[0] / (stddev[0] + 35);

    std::cout<<gamma<<std::endl;

    if (mean[0] < 127.5) {
        gamma = 1 / gamma;
    }

    std::cout<<"mean:"<<mean<<"; std:"<<stddev<<", gamma:"<<gamma<<std::endl;
    return gamma;
}
*/


