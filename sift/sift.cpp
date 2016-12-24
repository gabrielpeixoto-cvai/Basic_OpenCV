#include <iostream>
#include <stdio.h>
#include "opencv2/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/core/ocl.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/xfeatures2d.hpp"


#include <iostream>

#define GOOD_MATCHES
//#define useSURF

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;


int main(int argc, const char* argv[])
{
	assert (argc==3);

	const Mat img1 = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	const Mat img2 = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
	assert (img1.data != NULL && img2.data != NULL);

#ifdef useSURF

	// computing descriptors
	Ptr<Feature2D> surf;
	surf  = SURF::create(800.0);
	Mat descriptors1, descriptors2;
	std::vector<KeyPoint> keypoints1, keypoints2;
	surf->detectAndCompute(img1, Mat(), keypoints1, descriptors1, false);
	surf->detectAndCompute(img2, Mat(), keypoints2, descriptors2, false);

	BFMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors1, descriptors2, matches);

#else

	cv::Ptr<Feature2D> f2d = xfeatures2d::SIFT::create();

	std::vector<KeyPoint> keypoints1, keypoints2;
  f2d->detect( img1, keypoints1 );
  f2d->detect( img2, keypoints2 );

	/
  Mat descriptors1, descriptors2;
  f2d->compute( img1, keypoints1, descriptors1 );
  f2d->compute( img2, keypoints2, descriptors2 );

  BFMatcher matcher;
  std::vector< DMatch > matches;
  matcher.match( descriptors1, descriptors2, matches );

#endif

	namedWindow("matches", CV_WINDOW_NORMAL);
	Mat img_matches;

#ifdef GOOD_MATCHES

	double max_dist = 0; double min_dist = 100;
	for( int i = 0; i < descriptors1.rows; i++ )
	{
		double dist = matches[i].distance;
		if( dist < min_dist ) min_dist = dist;
		if( dist > max_dist ) max_dist = dist;
	}

	vector< DMatch > good_matches;
	for( int i = 0; i < descriptors1.rows; i++ )
	{
		if( matches[i].distance <= max(2*min_dist, 0.02) )
			{ good_matches.push_back( matches[i]); }
	}
	drawMatches(img1, keypoints1, img2, keypoints2, good_matches, img_matches);
#else
	drawMatches(img1, keypoints1, img2, keypoints2, matches, img_matches);
#endif

	imshow("matches", img_matches);
	waitKey(0);

	imwrite( "result.jpg", img_matches);

	return 0;
}
