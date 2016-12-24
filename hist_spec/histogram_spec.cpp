#include "ocv_utils.h"

int main(int argc, char *argv[])
{
	using namespace std;
	using namespace cv;
	using namespace ocv_utils;

	if (argc <= 4)
	{
		cerr << "Usage: " << argv[0] << " <file_name> <g=gray,r=rgb,h=hsv> <first> [<interm_coefs>] <last>" << endl;
		return 1;
	}

	Mat src = imread (argv[1]);
	if (src.data == NULL)
	{
		cerr << "Impossible to open '" << argv[1] << "' file!" << endl;
		return -1;
	}


	Mat img;

	vector <uchar> list;
	for (int i = 3; i < argc; i++)
	{
		list.push_back (atoi (argv[i]));
	}


	if (!strcmp (argv[2], "g"))
	{
		cvtColor(src,src,CV_BGR2GRAY);
		hist_t spec = makeHistogramSpecification (list);
		img = applyHistogramSpecification (src, spec);
	}
	else if (!strcmp (argv[2], "h"))
	{
		hist_t spec = makeHistogramSpecification (list);
		cvtColor(src,img,CV_BGR2HSV);
		vector<Mat> channels;
		split(img,channels);
		channels[2] = applyHistogramSpecification (channels[2], spec);
		merge(channels, img);
		cvtColor(img, img,CV_HSV2BGR);
	}
	else if (!strcmp (argv[2], "r"))
	{
		hist_t spec = makeHistogramSpecification (list, list, list);
		img = applyHistogramSpecification (src, spec);
	}
	else
	{
		cerr << "Unknown paramenter " << argv[2] << endl;
		return -1;
	}

	Mat canvas;
	Mat hist_src = plotHistogram (calcHistogram (src));
	Mat hist_img = plotHistogram (calcHistogram (img));
	resize (src, src, Size(512*src.cols/src.rows,512), 0, 0, INTER_CUBIC);
	resize (img, img, Size(512*img.cols/img.rows,512), 0, 0, INTER_CUBIC);
	resize (hist_src, hist_src, Size(512,512), 0, 0, INTER_CUBIC);
	resize (hist_img, hist_img, Size(512,512), 0, 0, INTER_CUBIC);
	hconcat (src, hist_src, src);
	hconcat (img, hist_img, img);
	vconcat (src, img, img);

	namedWindow(argv[0], CV_WINDOW_NORMAL);
    //setWindowProperty(argv[0], CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	imshow(argv[0], img);

	cv::waitKey (0);

	return 0;
}
