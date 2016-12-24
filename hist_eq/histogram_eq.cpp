#include "ocv_utils.h"

int main(int argc, char *argv[])
{
	using namespace std;
	using namespace cv;
	using namespace ocv_utils;

	if (argc < 2)
	{
		cerr << "Usage: " << argv[0] << " <file_name> [1=HSV,2=YCrCb,3=RGB]" << endl;
		return 1;
	}

	int method = 1;
	if (argc >= 3)
	{
		method = atoi(argv[2]);
		if (method < 1 || method > 3)
			method = 1;
	}

	Mat src = imread (argv[1]);
	if (src.data == NULL)
	{
		cerr << "Impossible to open '" << argv[1] << "' file!" << endl;
		return -1;
	}

	Mat src_hist, eq, eq_hist;
	src_hist = plotHistogram (calcHistogram (src));

	if (method == 1)
	{
		cvtColor(src,eq,CV_BGR2HSV);
		vector<Mat> channels;
		split(eq,channels);
		equalizeHist(channels[2], channels[2]);
		merge(channels,eq);
		cvtColor(eq,eq,CV_HSV2BGR);
		eq_hist = plotHistogram (calcHistogram (eq));
	}
	else if (method == 2)
	{
		cvtColor(src,eq,CV_BGR2YCrCb);
		vector<Mat> channels;
		split(eq,channels);
		equalizeHist(channels[0], channels[0]);
		merge(channels,eq);
		cvtColor(eq,eq,CV_YCrCb2BGR);
		eq_hist = plotHistogram (calcHistogram (eq));
	}	
	else if (method == 3)
	{
		vector<Mat> channels;
		split(src,channels);
		equalizeHist(channels[0], channels[0]);
		equalizeHist(channels[1], channels[1]);
		equalizeHist(channels[2], channels[2]);
		merge(channels,eq);
		eq_hist = plotHistogram (calcHistogram (eq));
	}

	resize (src, src, Size(512*src.cols/src.rows,512), 0, 0, INTER_CUBIC);
	resize (eq, eq, Size(512*eq.cols/eq.rows,512), 0, 0, INTER_CUBIC);
	hconcat (src, src_hist, src);
	hconcat (eq, eq_hist, eq);
	vconcat (src, eq, src);

	namedWindow(argv[0], CV_WINDOW_NORMAL);
    //setWindowProperty(argv[0], CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	imshow(argv[0], src);

	cv::waitKey (0);

	return 0;
}

