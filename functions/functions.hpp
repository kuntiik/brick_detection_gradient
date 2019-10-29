#ifndef FUNCTIONS
#define FUNCTIONS
#include <opencv2/opencv.hpp>
int16_t ** calculate_gradient(cv::Mat image, const int height, const int width, const int *kernel);

#endif
