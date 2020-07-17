#include <stdio.h>
// #include <sys/stat.h>
// #include <sys/time.h>
#include <iostream>
#include "image_enhance/image_enhance.h"


void TestImageEnhancement() {
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


int main(int argc, char** argv) {
    TestImageEnhancement();
    return 0;
}
