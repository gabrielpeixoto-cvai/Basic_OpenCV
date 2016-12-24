#ifndef OCV_UTILS_H
#define OCV_UTILS_H

#include <opencv2/opencv.hpp>

namespace ocv_utils {

std::vector<cv::Mat> calcHistogram(const cv::Mat& img);
cv::Mat plotHistogram(const std::vector<cv::Mat> &hist);

} // namespace ocv_utils

#endif // OCV_UTILS_H
