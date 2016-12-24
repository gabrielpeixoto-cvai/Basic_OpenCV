#include "ocv_utils.h"

using namespace std;
using namespace cv;

namespace ocv_utils {

std::vector<cv::Mat> calcHistogram(const cv::Mat& img)
{

	const int nc = img.channels();
	vector<Mat> hist(nc);

	// Initalize histogram arrays
	for (int i = 0; i < hist.size(); i++)
		hist[i] = Mat::zeros(1, 256, CV_32SC1);

	// Calculate the histogram of the image
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			for (int k = 0; k < nc; k++)
			{
				uchar val = nc == 1 ? img.at<uchar>(i,j) : img.at<Vec3b>(i,j)[k];
				hist[k].at<int>(val) += 1;
			}
		}
	}
	return hist;
}

cv::Mat plotHistogram(const std::vector<cv::Mat> &hist)
{
	Mat canvas;
	int nc = hist.size();

	if (nc == 1)
	{
		canvas = Mat::ones(256, 256, CV_8UC1);
		int hmax = 0;
		for (int j = 0; j < 256; j++)
			hmax = hist[0].at<int>(j) > hmax ? hist[0].at<int>(j) : hmax;

		for (int j = 0, rows = canvas.rows; j < 256; j++)
		{
			line(
				canvas, 
				Point(j, rows), 
				Point(j, rows - (hist[0].at<int>(j) * rows/hmax)), 
				Scalar(200,200,200),
				1, 8, 0
			);
		}

		return canvas;
	}
	else
	{
		canvas = Mat::zeros(512, 512, CV_8UC3);
		Scalar colors[3] = { Scalar(255,0,0), Scalar(0,255,0), Scalar(0,0,255) };
		int hmax[4] = {0,0,0,0};
		for (int i = 0; i < nc; i++)
		{
			for (int j = 0; j < 256; j++)
			{
				hmax[i] = hist[i].at<int>(j) > hmax[i] ? hist[i].at<int>(j) : hmax[i];
				hmax[3] = hist[i].at<int>(j) > hmax[3] ? hist[i].at<int>(j) : hmax[3];
			}
		}
		Mat roi;
		for (int i = 0; i < nc; i++)
		{
			roi = canvas(i == 0 ? Rect(0,256,256,256) : (i == 1 ? Rect(256,0,256,256) : Rect(0,0,256,256)));
			for (int j = 0; j < 256; j++)
			{
				line(
					roi, 
					Point(j, 256), 
					Point(j, 256 - (hist[i].at<int>(j) * 256/hmax[i])), 
					colors[i],
					1, 8, 0
				);
			}
		}

		roi = canvas (Rect(256,256,256,256));
		for (int j = 0; j < 256; j++)
		{
			int s[3];
			if (hist[0].at<int>(j) > hist[1].at<int>(j) && hist[0].at<int>(j) > hist[2].at<int>(j)) {
				if (hist[1].at<int>(j) > hist[2].at<int>(j)) {s[0] = 0; s[1] = 1; s[2] = 2;}
				else {s[0] = 0; s[1] = 2; s[2] = 1;}
			} else if (hist[1].at<int>(j) > hist[0].at<int>(j) && hist[1].at<int>(j) > hist[2].at<int>(j)) {
				if (hist[0].at<int>(j) > hist[2].at<int>(j)) {s[0] = 1; s[1] = 0; s[2] = 2;}
				else {s[0] = 1; s[1] = 2; s[2] = 0;}
			} else if (hist[2].at<int>(j) > hist[0].at<int>(j) && hist[2].at<int>(j) > hist[1].at<int>(j)) {
				if (hist[0].at<int>(j) > hist[1].at<int>(j)) {s[0] = 2; s[1] = 0; s[2] = 1;}
				else {s[0] = 2; s[1] = 1; s[2] = 0;}
			}
			for (int i = 0; i < nc; i++)
			{
				line(
					roi, 
					Point(j, 256),
					Point(j, 256 - (hist[s[i]].at<int>(j) * 256/hmax[3])), 
					nc == 1 ? Scalar(200,200,200) : colors[s[i]], 
					1, 8, 0
				);
			}
		}
		return canvas;
	}
}

} // namespace ocv_utils
