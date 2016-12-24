#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

/// Global variables

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int highThreshold;
int nTimes;
int const max_lowThreshold = 100;
int const max_highThreshold = 100;
int const max_nTimes = 10;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(int, void*)
{
  int i;
  /// Reduce noise with a kernel 3x3
  blur( src_gray, detected_edges, Size(3,3) );
  for(i=0; i<=nTimes;i++){
    blur( detected_edges, detected_edges, Size(3,3) );
  }
  /// Canny detector
  Canny( detected_edges, detected_edges, lowThreshold, highThreshold, kernel_size );

  /// Using Canny's output as a mask, we display our result
  //dst = Scalar::all(0);

  //src.copyTo( dst, detected_edges);
  imshow( window_name, detected_edges );
 }


/** @function main */
int main( int argc, char** argv )
{
  /// Load an image
  src = imread( argv[1] );

  if( !src.data )
  { return -1; }

  /// Create a matrix of the same type and size as src (for dst)
  dst.create( src.size(), src.type() );

  /// Convert the image to grayscale
  cvtColor( src, src_gray, COLOR_BGR2GRAY );

  /// Create a window
  namedWindow( window_name, WINDOW_NORMAL );

  /// Create a Trackbar for user to enter threshold
  createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
  createTrackbar( "High Threshold:", window_name, &highThreshold, max_highThreshold, CannyThreshold );
  createTrackbar( "Blur times:", window_name, &nTimes, max_nTimes, CannyThreshold );

  /// Show the image
  CannyThreshold(0, 0);

  /// Wait until user exit program by pressing a key
  waitKey(0);

  imwrite( "result.jpg", detected_edges);

  return 0;
  }
