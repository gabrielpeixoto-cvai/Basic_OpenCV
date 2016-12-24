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
			ss << "Blur 3x3";
			cv::blur(img, ret, cv::Size(3,3), cv::Point(-1,-1), cv::BORDER_DEFAULT);
			break;
		case 3:
			ss << "Blur 5x5";
			cv::blur(img, ret, cv::Size(5,5), cv::Point(-1,-1), cv::BORDER_DEFAULT);
			break;
		case 4:
			ss << "Gaussian 3x3";
			cv::GaussianBlur(img, ret, cv::Size(3,3), 0.5, 0, cv::BORDER_DEFAULT);
			break;
		case 5:
			ss << "Gaussian 5x5";
			cv::GaussianBlur(img, ret, cv::Size(5,5), 0.5, 0, cv::BORDER_DEFAULT);
			break;
		case 6:
			ss << "Median 3x3";
			cv::medianBlur(img, ret, 3);
			break;
		case 7:
			ss << "Median 5x5";
			cv::medianBlur(img, ret, 5);
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
	const int w = 2000;
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
