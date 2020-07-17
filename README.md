# Image-Enhancement-for-SLAM
Image Enhancement for SLAM

在使用Semi-direct Method跑Euroc Dataset的v103数据时，发现效果很不好。导致错误的主要的原因有：图片太暗，对比度太低；图片亮度变化很大（不限于帧间，左右目有时候也会出现亮度不一致的情况）。于是，需要对输入图像进行预处理，提高图片的对比度，并且使得进行跟踪的两张图片亮度一致。对于提高图片的对比度，最简单的方法是使用直方图均衡化。不过直方图均衡化有一些很明显的缺点，如变换后细节消失；不自然的过分增强。对于SLAM系统，往往会在过份增强的纹理上提取出一些关键点，而这些关键点我们认为是不稳定的（如下图的窗帘）。所以，我们需要一种更加先进的图像增强算法用于SLAM的图像预处理。

## References ## 
Papers Describing the Approach:

罗汉杰. 图像增强方法、装置、计算机设备和存储介质 [P]. 中国专利: CN109801244A,2019-05-24.

http://www.luohanjie.com/2018-11-01/image-enhancement-for-slam.html  
http://www.luohanjie.com/2020-07-17/image-enhancement-algorithm-for-slam.html

## Requirements ##
The code is tested on Ubuntu 14.04. ImageEnhance requires the following tools and libraries: CMake, OpenCV 3.4. 

## Building ##

```
#!bash

cd Image-Enhancement-for-SLAM
mkdir build
cd build
cmake  ..
make
```

Test:

```
#!bash
./bin/test_image_enhance
```

## Example of result ##
Input image:
![input image](https://github.com/HanjieLuo/Image-Enhancement-for-SLAM/blob/master/data/3.png)

Output image:
![input image](https://github.com/HanjieLuo/Image-Enhancement-for-SLAM/blob/master/data/3_enhanced.png)

Test for Semi-direct Method in Euroc Dataset
[![](http://img.youtube.com/vi/bfZfZQ-2KLs/0.jpg)](http://www.youtube.com/watch?v=bfZfZQ-2KLs "Image Enhancement for SLAM")

## Contact information ##
Hanjie Luo [luohanjie@gmail.com](mailto:luohanjie@gmail.com)




