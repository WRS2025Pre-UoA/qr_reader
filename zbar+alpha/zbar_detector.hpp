// zbar_detector.hpp
#ifndef ZBAR_DETECTOR_HPP
#define ZBAR_DETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <string>

std::string detect_with_zbar(cv::Mat& image);

#endif
