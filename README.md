# Image Enhancement for SLAM

This module provides a high-performance, multi-stage image enhancement pipeline specifically designed to improve the robustness of visual SLAM systems (e.g., semi-direct methods) in challenging lighting conditions.

## Motivation

Visual SLAM algorithms are highly sensitive to image quality. When processing datasets like EuRoC V103, which feature significant portions with low light, low contrast, and drastic illumination changes (both inter-frame and across stereo pairs), standard feature tracking can degrade or fail entirely.

While simple methods like **Histogram Equalization** can increase global contrast, they often introduce undesirable artifacts:
*   **Loss of Detail**: Nuanced textures can be washed out.
*   **Unnatural Over-enhancement**: Creates artificial-looking textures where SLAM might extract unstable, non-repeatable keypoints.
*   **Noise Amplification**: Dark regions become noisy, further degrading feature quality.

To address these issues, a more sophisticated approach is required that can enhance local contrast and restore texture details without introducing detrimental artifacts.

## Methodology

This implementation is based on the **Retinex theory**, which models an image `I(x,y)` as the product of its illumination `L(x,y)` and reflectance `R(x,y)` components:

`I(x,y) = L(x,y) * R(x,y)`

The core idea is to estimate the illumination map `L` and then recover the reflectance `R`, which represents the intrinsic, lighting-invariant properties of the scene. The enhancement pipeline consists of the following key steps, as implemented in the `ImageEnhance` class:

### Step 1: Illumination Map Estimation via Fast Global Smoother (FGS)

The illumination `L` is assumed to be a smooth, low-frequency representation of the image. Instead of using a simple Gaussian blur, which can cause halo artifacts around strong edges, this algorithm employs the **Fast Global Smoother (FGS)** filter (fgs_filter.h).

*   The FGS is an edge-preserving smoothing filter that effectively separates the low-frequency illumination layer from the high-frequency reflectance (texture) layer.
*   To improve performance, the filtering is performed on a down-sampled version of the image (`resize_` parameter).

### Step 2: Illumination Adjustment

The estimated illumination map is then adjusted using a **Gamma correction** (`gamma_` parameter). This step non-linearly enhances the illumination, brightening the dark areas more significantly than the already bright ones.

`L_adjusted(x,y) = L(x,y) ^ gamma`

A `gamma` value less than 1.0 increases the overall brightness.

### Step 3: Reflectance Recovery

The enhanced image (i.e., the reflectance map) is recovered by dividing the original image by the adjusted illumination map, following the Retinex model:

`R(x,y) = I(x,y) / L_adjusted(x,y)`

This step effectively removes the non-uniform lighting and reveals the underlying texture details.

### Step 4: Detail Smoothing with Bilateral Filter

The recovered reflectance map may contain some residual noise. A **Bilateral Filter** is applied to smooth the image while preserving important structural edges, resulting in a cleaner final output.

### Step 5: Final Contrast Enhancement (Histogram Clipping)

To produce a visually pleasing result with optimal dynamic range, a final contrast enhancement step (`ContrastEnhance` function) is performed. This is a robust alternative to simple histogram stretching.

*   For color images, the enhancement is applied only to the **Value (V) channel** in the HSV color space to avoid color distortion.
*   The image's histogram is computed, and the lower and upper bounds (`contrast_to_low_`, `contrast_to_high_`) are clipped based on the cumulative distribution function (CDF). This effectively ignores a small percentage of the darkest and brightest pixels, making the stretching robust to outliers.
*   The pixel values within the clipped range `[ilow, ihigh]` are then re-mapped to the full `[0, 255]` range, significantly improving the final image contrast.

This multi-stage process ensures that images are enhanced in a way that is both effective for computer vision algorithms and visually natural, providing a solid foundation for robust SLAM tracking.

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




