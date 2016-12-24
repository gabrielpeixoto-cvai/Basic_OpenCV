#include "ocv_utils.h"

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <sstream>

using namespace std;
using namespace cv;

void on_trackbar( int power, void* img)
{
	Mat src = ((Mat*)img)->clone();
	Mat dst = Mat::zeros (src.rows, src.cols, src.type());

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			for (int k = 0; k < src.channels(); k++)
			{
				//normal
				//dst.at<Vec3b>(i,j)[k] = 255*pow(src.at<Vec3b>(i,j)[k]/255.0, power/100.0f);
				//inverted s=L-1-r (s - resultado; L - range maximo (256) ; s - valor de entrada do pixel)
				//dst.at<Vec3b>(i,j)[k] = (255-src.at<Vec3b>(i,j)[k]);
				//inverted+Gamma
				dst.at<Vec3b>(i,j)[k] = (255)-255*pow(src.at<Vec3b>(i,j)[k]/255.0, power/100.0f);

			}
		}
	}

	Mat hist_src = ocv_utils::plotHistogram (ocv_utils::calcHistogram (src));
	resize (hist_src, hist_src, Size(src.rows,src.rows), 0, 0, INTER_CUBIC);
	Mat hist_dst = ocv_utils::plotHistogram (ocv_utils::calcHistogram (dst));
	resize (hist_dst, hist_dst, Size(dst.rows,dst.rows), 0, 0, INTER_CUBIC);
	hconcat (src, hist_src, src);
	hconcat (dst, hist_dst, dst);
	vconcat (src, dst, dst);

	std::stringstream ss; ss << "Gamma = " << std::fixed << std::setprecision(2) << power/100.0f;
	cv::putText(dst, ss.str(), cv::Point(30,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar (255,255,255),1,CV_AA);;

	imshow("Name", dst);
}

int main(int argc, char *argv[])
{


	if (argc != 2)
	{
		cerr << "Usage: " << argv[0] << " <file_name>" << endl;
		return 1;
	}

	const Mat img = imread (argv[1]);
	if (img.data == NULL)
	{
		cerr << "Impossible to open '" << argv[1] << "' file!" << endl;
		return -1;
	}

	namedWindow("Name", CV_WINDOW_NORMAL);
	//setWindowProperty("Name", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	Mat src;
	resize (img, src, Size(512*img.cols/img.rows,512), 0, 0, INTER_CUBIC);

	int gamma_slider = 100;
	int gamma_slider_max = 3 * 100;
	createTrackbar( "Gamma", "Name", &gamma_slider, gamma_slider_max, on_trackbar, (void*) &src );

	on_trackbar (gamma_slider, (void*) &src);

	cv::waitKey (0);

	return 0;
}
