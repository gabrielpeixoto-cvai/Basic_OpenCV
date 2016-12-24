//canny
/*
 *  g++ `pkg-config --libs --cflags opencv` filtros.cpp -o filtros && ./filtros <nome_da_imagem>
 */

#include <iostream>
#include <iomanip>
#include <sstream>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

cv::Mat Canny(cv::Mat img, int blur_size)
{

	cv::Mat ret = img.clone();
	std::stringstream ss;

  // We convert our filtered image to grayscale:
  cv::Mat src_gray;
  cvtColor (img, src_gray, CV_RGB2GRAY);

  //gaussian blur

  cv::GaussianBlur(src_gray, src_gray, cv::Size(blur_size, blur_size), 0.5, 0, cv::BORDER_DEFAULT);

  //Sobel

	// We calculate the "derivatives" in x and y directions. For this, we use the function Sobel:
	cv::Mat grad_x, grad_y;
	cv::Sobel (src_gray, grad_x, CV_16S, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
	cv::Sobel (src_gray, grad_y, CV_16S, 0, 1, 3, 1, 0, cv::BORDER_DEFAULT);


	// We convert our partial results back to CV_8U:
	cv::Mat abs_grad_x, abs_grad_y;
	cv::convertScaleAbs (grad_x, abs_grad_x);
	cv::convertScaleAbs (grad_y, abs_grad_y);

	// We try to approximate the gradient by adding both directional gradients
	addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, ret);


	cv::putText(ret, ss.str(), cv::Point(30,30), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar (255,255,255),1,CV_AA);

	return ret;
}

int main(const int argc, const char *argv[])
{
	if (argc != 2)
	{
		cerr << "Usage: " << argv[0] << " <filename>" << endl;
		return 1;
	}


	std::string filename = argv[1];

	std::string winName = "Filter";

	cv::Mat src;

	src = cv::imread(filename);
	if (!src.data)
	{
		cerr << "Impossible to open " << filename << "!" << endl;
		return -1;
	}

	cv::namedWindow (winName, cv::WINDOW_NORMAL);
	const int w = 1200;
	if (src.rows > w)
		resize(src,src, cv::Size(w, w*src.rows/src.cols));
	char tipo = '1';

	imshow (winName, Canny(src,3));
	tipo = cv::waitKey();

	return 0;
}
