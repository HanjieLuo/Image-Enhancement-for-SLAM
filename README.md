# Image Enhancement for SLAM

It was found that the performance of semi-direct method was unsatisfactory when running the v103 data of Euroc dataset. One reason is that the images are too dark with low contrast. Another reason is that the brightness of the images varies greatly (not limited to inter-frame, sometimes there is also inconsistent brightness between left and right images). Therefore, it is necessary to preprocess the input images, improve the contrast of the images, and make make the two images have consistent brightness.

The simplest method to improve the contrast of the images is to use histogram equalization. However, histogram equalization has some obvious drawbacks, such as the loss of details after transformation and unnatural excessive enhancement. For SLAM systems, key points are often extracted on excessively enhanced textures, which we consider to be unstable. Therefore, we need a more advanced image enhancement algorithm for image preprocessing in SLAM.

We proposes an image enhancement algorithm based on the theory of Retinex, which enhances underexposed images, restores the texture in the images, and achieves real-time processing.

在使用Semi-direct Method跑Euroc Dataset的v103数据时，发现效果很不好。导致错误的主要的原因有：图片太暗，对比度太低；图片亮度变化很大（不限于帧间，左右目有时候也会出现亮度不一致的情况）。于是，需要对输入图像进行预处理，提高图片的对比度，并且使得进行跟踪的两张图片亮度一致。

对于提高图片的对比度，最简单的方法是使用直方图均衡化。不过直方图均衡化有一些很明显的缺点，如变换后细节消失；不自然的过分增强。对于SLAM系统，往往会在过份增强的纹理上提取出一些关键点，而这些关键点我们认为是不稳定的。所以，我们需要一种更加先进的图像增强算法用于SLAM的图像预处理。

本文提出了一种基于Retinex理论的图像增强算法，对欠曝光的图像进行增强, 能够恢复图像中的纹理，并且做到实时处理。

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

Test in underground parking lot dataset:  
[![Test in underground parking lot dataset](https://res.cloudinary.com/marcomontalbano/image/upload/v1612585629/video_to_markdown/images/youtube--rnttLhYdGiE-c05b58ac6eb4c4700831b2b3070cd403.jpg)](https://youtu.be/rnttLhYdGiE "Test in underground parking lot dataset")

Test in Euroc dataset V1_03_difficult:  
[![Test in Euroc dataset V1_03_difficult](https://res.cloudinary.com/marcomontalbano/image/upload/v1612585672/video_to_markdown/images/youtube--T9_IqWa4mJI-c05b58ac6eb4c4700831b2b3070cd403.jpg)](https://youtu.be/T9_IqWa4mJI "Test in Euroc dataset V1_03_difficult")

Test for semi-direct method in Euroc dataset:  
[![Test for semi-direct method in Euroc dataset](https://res.cloudinary.com/marcomontalbano/image/upload/v1612585723/video_to_markdown/images/youtube--bfZfZQ-2KLs-c05b58ac6eb4c4700831b2b3070cd403.jpg)](https://youtu.be/bfZfZQ-2KLs "Test for semi-direct method in Euroc dataset")

## Contact information ##
Hanjie Luo [luohanjie@gmail.com](mailto:luohanjie@gmail.com)




