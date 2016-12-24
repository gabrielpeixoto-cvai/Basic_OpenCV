#include "ocv_utils.h"

using namespace std;
using namespace cv;

namespace ocv_utils {

hist_t calcHistogram(const cv::Mat& img)
{

	const int nc = img.channels();
	hist_t hist(nc);

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

hist_t calcCumulativeHist(const hist_t &hist)
{
	hist_t ret(hist.size());
	for (int c = 0; c < hist.size(); ++c)
	{
		int sum = 0;
		ret[c] = Mat (1, 256, CV_32SC1);
		for (int i = 0; i < hist[c].cols; ++i)
		{
			sum = ret[c].at<int>(i) = sum + hist[c].at<int>(i);
		}
		for (int i = 0; i < hist[c].cols; ++i)
		{
			ret[c].at<int>(i) = 255 * ret[c].at<int>(i) / sum;
		}
	}
	return ret;
}

hist_t calcBackProjection(const hist_t &hist)
{
	hist_t ret(hist.size());
	for (int c = 0; c < hist.size(); ++c)
	{
		ret[c] = Mat (1, 256, CV_32SC1);
		int i = 0;
		for (int n = 0; n < 256; ++n)
		{
			while (n >= hist[c].at<int>(i) && i < 255)
				i++;
			ret[c].at<int>(n) = i;
		}
	}
	return ret;
}

hist_t makeHistogramSpecification(const vector<uchar> spec1, const vector<uchar> spec2, const vector<uchar> spec3)
{
	vector< vector<uchar> > specs;
	specs.push_back (spec1);
	if (spec2.size() != 0 && spec3.size() != 0)
	{
		specs.push_back (spec2);
		specs.push_back (spec3);
	}

	hist_t s (specs.size());
	for (int i = 0; i < s.size(); i++)
	{
		s[i] = Mat (1, 256, CV_32SC1);
		for (int j = 0; j < 256; j++)
		{
			float idx_f = (float)(specs[i].size()-1)*((float)j/256.0f);
			int idx_i = (int) idx_f;
			float w = idx_f - idx_i;
			s[i].at<int>(j) = 0.5f + ((1.0f - w) * specs[i][idx_i] + w * specs[i][idx_i+1]);
		}
	}
	return s;
}

Mat applyTransformation(const Mat &img, const hist_t &hist)
{
	assert (img.channels() == hist.size());
	Mat ret = Mat (img.size(), img.type());

	const int nc = img.channels();
	if (nc == 1)
	{
		for (int i = 0; i < img.rows; i++)
			for (int j = 0; j < img.cols; j++)
				ret.at<uchar>(i,j) = hist[0].at<int>(img.at<uchar>(i,j));
	}
	else
	{
		for (int i = 0; i < img.rows; i++)
			for (int j = 0; j < img.cols; j++)
				for (int k = 0; k < nc; k++)
					ret.at<Vec3b>(i,j)[k] = hist[k].at<int>(img.at<Vec3b>(i,j)[k]);
	}
	return ret;
}

Mat applyHistogramEqualization (const Mat &img)
{
	hist_t hist = calcHistogram (img);
	hist_t cumu = calcCumulativeHist (hist);
	Mat ret = applyTransformation (img, cumu);
	return ret;
}

Mat applyHistogramSpecification (const Mat &img, const hist_t &spec)
{
	hist_t img_hist = calcHistogram (img);
	hist_t img_hist_cumu = calcCumulativeHist (img_hist);

	hist_t spec_cumu = calcCumulativeHist (spec);
	hist_t spec_cumu_inv = calcBackProjection (spec_cumu);

	hist_t final(spec.size());
	for (int c = 0; c < spec.size(); c++)
	{
		final[c] = Mat (spec[c].size(), CV_32SC1);
		for (int i = 0; i < 256; i++)
		{
			final[c].at<int>(i) = spec_cumu_inv[c].at<int>(img_hist_cumu[c].at<int>(i));
		}
	}
	
	Mat ret = applyTransformation (img, final);
	namedWindow("ok", CV_WINDOW_NORMAL);

	return ret;
}

cv::Mat plotHistogram(const hist_t &hist)
{
	Mat canvas;
	int nc = hist.size();
	hist_t acc = calcCumulativeHist (hist);

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
		for (int j = 0; j < 255; j++)
			line(
				canvas,
				Point(j, 256 - acc[0].at<int>(j)),
				Point(j+1, 256 - acc[0].at<int>(j+1)),
				Scalar(255,255,255),
				1, 8, 0
			);

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
			for (int j = 0; j < 255; j++)
				line(
					roi,
					Point(j, 256 - acc[i].at<int>(j)),
					Point(j+1, 256 - acc[i].at<int>(j+1)),
					Scalar(255,255,255),
					1, 8, 0
				);
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
