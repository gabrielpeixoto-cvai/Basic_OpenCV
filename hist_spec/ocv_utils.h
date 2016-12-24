#ifndef OCV_UTILS_H
#define OCV_UTILS_H

#include <opencv2/opencv.hpp>

#define DBG(fmt, ...) printf (fmt " (%s:%i)\n", ##__VA_ARGS__, __FILE__, __LINE__);

namespace ocv_utils {

typedef std::vector<cv::Mat> hist_t;

hist_t calcHistogram(const cv::Mat& img);
hist_t calcCumulativeHist(const hist_t &hist);
hist_t calcBackProjection(const hist_t &hist);
hist_t makeHistogramSpecification(const std::vector<uchar> spec1, const std::vector<uchar> spec2 = std::vector<uchar>(), const std::vector<uchar> spec3 = std::vector<uchar>());
cv::Mat applyTransformation(const cv::Mat &img, const hist_t &hist);
cv::Mat applyHistogramEqualization (const cv::Mat &img);
cv::Mat applyHistogramSpecification (const cv::Mat &img, const hist_t &spec);
cv::Mat plotHistogram(const hist_t &hist);

} // namespace ocv_utils

#endif // OCV_UTILS_H
