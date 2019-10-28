#ifndef FUNCTIONS
#define FUNCTIONS
#include <opencv2/opencv.hpp>


//extern const int HEIGHT;
//extern const int WIDTH;

int16_t ** calculate_gradient(cv::Mat image, const int *kernel);

#endif

