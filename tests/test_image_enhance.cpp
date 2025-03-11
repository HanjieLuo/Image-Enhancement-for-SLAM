#include <dirent.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h> // for usleep

#include <iostream>

#include "image_enhance.h"

using namespace image_enhance;

void TestImages() {
    cv::Mat test_img1, test_img2, test_img3;
    cv::Mat enhanced_img1, enhanced_img2, enhanced_img3;

    test_img1 = cv::imread("./../data/1.png", 1);
    test_img2 = cv::imread("./../data/2.png", 0);
    test_img3 = cv::imread("./../data/3.png", 0);

    ImageEnhance image_enhance = ImageEnhance();

    image_enhance.Apply(test_img1, enhanced_img1);
    image_enhance.Apply(test_img2, enhanced_img2);
    image_enhance.Apply(test_img3, enhanced_img3);

    int width = test_img1.cols;
    int height = test_img1.rows;
    cv::Mat img_show1(height, width * 2, CV_8UC3);
    test_img1.copyTo(img_show1(cv::Rect(0, 0, width, height)));
    enhanced_img1.copyTo(img_show1(cv::Rect(width, 0, width, height)));

    width = test_img2.cols;
    height = test_img2.rows;
    cv::Mat img_show2(height, width * 2, CV_8UC3);
    cv::cvtColor(test_img2, img_show2(cv::Rect(0, 0, width, height)), cv::COLOR_GRAY2BGR);
    cv::cvtColor(enhanced_img2, img_show2(cv::Rect(width, 0, width, height)), cv::COLOR_GRAY2BGR);

    width = test_img3.cols;
    height = test_img3.rows;
    cv::Mat img_show3(height, width * 2, CV_8UC3);
    cv::cvtColor(test_img3, img_show3(cv::Rect(0, 0, width, height)), cv::COLOR_GRAY2BGR);
    cv::cvtColor(enhanced_img3, img_show3(cv::Rect(width, 0, width, height)), cv::COLOR_GRAY2BGR);

    cv::imshow("test_img1", img_show1);
    cv::imshow("test_img2", img_show2);
    cv::imshow("test_img3", img_show3);

    cv::imwrite("./../data/1_enhanced.png", enhanced_img1);
    cv::imwrite("./../data/2_enhanced.png", enhanced_img2);
    cv::imwrite("./../data/3_enhanced.png", enhanced_img3);

    cv::waitKey(0);
}

void TestPerformance() {
    std::string img_file = "./../data/4.png";
    cv::Mat img_enhanced, img = cv::imread(img_file, 0);

    ImageEnhance image_enhance = ImageEnhance(0.7);

    int run_times = 1000;
    struct timeval t1, t2;
    float time, time_sum = 0;
    const int target_fps = 2;
    const int ms_per_second = 1000;
    const int target_frame_duration = ms_per_second / target_fps;
    for (int i = 0; i < run_times; i++) {
        gettimeofday(&t1, NULL);
        image_enhance.Apply(img, img_enhanced);
        gettimeofday(&t2, NULL);

        time = (t2.tv_sec - t1.tv_sec) * 1000.f + (t2.tv_usec - t1.tv_usec) / 1000.f;
        time_sum += time;

        std::cout << "time: " << time << "ms" << std::endl;

        // 计算需要休眠的时间
        int sleep_duration = target_frame_duration - time;
        if (sleep_duration > 0) {
            usleep(sleep_duration * 1000);
        }
    }

    std::cout << "Avg time: " << time_sum / run_times << "ms" << std::endl;
}

int main(int argc, char** argv) {
    TestImages();
    // TestPerformance();
    return 0;
}
