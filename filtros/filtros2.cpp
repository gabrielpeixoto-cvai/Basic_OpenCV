/*
 *  g++ `pkg-config --libs --cflags opencv` filtros.cpp -o filtros && ./filtros <nome_da_imagem>
 */

#include <iostream>
#include <iomanip>
#include <sstream>

#include <opencv2/opencv.hpp>

using namespace std;

cv::Mat aplicarFiltro(cv::Mat img, int tipo)
{

	cv::Mat ret = img.clone();
	std::stringstream ss;

	switch (tipo)
	{
		case 1:
			ss << "Imagem Original";
			break;
		case 2:
			ss << "Unsharp Masking";
			{
				cv::Mat img_gray;
				cvtColor (img, img_gray, CV_RGB2GRAY);
				cv::Mat img_blur = img_gray.clone();
				for (int i = 0; i < 1; ++i)
					blur (img_blur, img_blur, cv::Size(3,3));
				ret = 8*(img_blur - img_gray);
			}
			break;
		case 3:
			ss << "Edge Enhacement";
			{
				for (int i = 0; i < 1; ++i)
					blur (ret, ret, cv::Size(3,3));
				ret = 2*(img - ret);
				ret += img;
			}
			break;
		case 4:
			ss << "High-boost filtering";
			{
				float m[3][3] = { {  -1,  -1,   -1},
								  				{  -1,   8,   -1},
								  				{  -1,  -1,   -1} };
				cv::Mat kernel = cv::Mat(3,3,CV_32F, m) ;
				cv::filter2D(img, ret, CV_8U, kernel, cv::Point(-1,-1), cv::BORDER_DEFAULT);
			}
			break;
		case 5:
			ss << "Sharpening";
			{
				float m[3][3] = { {  -1,  -1,   -1},
								  				{  -1,   9,   -1},
								  				{  -1,  -1,   -1} };
				cv::Mat kernel = cv::Mat(3,3,CV_32F, m) ;
				cv::filter2D(img, ret, CV_8U, kernel, cv::Point(-1,-1), cv::BORDER_DEFAULT);
			}
			break;
		case 6:
			ss << "Laplacian";
			{
				cv::GaussianBlur(ret, ret, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT);
				cv::Laplacian(ret, ret, CV_8U, 3, 1, 0, cv::BORDER_DEFAULT);
			}
			break;
		case 7:
			ss << "Sobel";
			{
				// We convert our filtered image to grayscale:
				cv::Mat src_gray;
				cvtColor (img, src_gray, CV_RGB2GRAY);

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
			}
			break;
		case 8:
			ss << "Sharpening 5x5";
			{
				float m[5][5] = { {  1,  4,   6,  4,  1 },
								  				{  4, 16,  24, 16,  4 },
								  				{  6, 24,-478, 24,  6 },
								  				{  4, 16,  24, 16,  4 },
								  				{  1,  4,   6,  4,  1 } };
				cv::Mat kernel = cv::Mat(5,5,CV_32F, m) / -1.0f/256.0f;
				cv::filter2D(img, ret, CV_8U, kernel, cv::Point(-1,-1), cv::BORDER_DEFAULT);
			}
			break;
		default:
			exit(1);
	}


	cv::putText(ret, ss.str(), cv::Point(30,30), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar (255,255,255),1,CV_AA);;
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
	do
	{
		imshow (winName, aplicarFiltro(src, tipo - '0'));
		tipo = cv::waitKey();
	} while (tipo >= '0' && tipo < '9');

	return 0;
}
