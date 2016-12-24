#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

#include <iostream>

using namespace cv;
using namespace std;


Mat src, src_gray, src_canny;
Mat dst, cdst;

int edgeThresh = 1;
int lowThreshold;
int highThreshold;
int nTimes;
int nInt=5;
int const max_lowThreshold = 100;
int const max_highThreshold = 100;
int const max_nTimes = 10;
int const max_nInt = 200;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Hough Transform";

void HoughTransform(int, void*){

  int i;
  /// Reduce noise with a kernel 3x3
  blur( src_gray, dst, Size(3,3) );
  for(i=0; i<=nTimes;i++){
    blur( dst, dst, Size(3,3) );
  }
  /// Canny detector
  Canny( dst, dst, lowThreshold, highThreshold, 3);

  cout<<dst.cols<<dst.rows<<endl;

  cvtColor(dst, cdst, CV_GRAY2BGR);

  #if 1
  vector<Vec2f> lines;
  HoughLines(dst, lines, 1, CV_PI/180, 100, nInt, 0 );

  for( size_t i = 0; i < lines.size(); i++ )
  {
     float rho = lines[i][0], theta = lines[i][1];
     Point pt1, pt2;
     double a = cos(theta), b = sin(theta);
     double x0 = a*rho, y0 = b*rho;
     pt1.x = cvRound(x0 + 1000*(-b));
     pt1.y = cvRound(y0 + 1000*(a));
     pt2.x = cvRound(x0 - 1000*(-b));
     pt2.y = cvRound(y0 - 1000*(a));
     line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
  }
 #else
  vector<Vec4i> lines;
  HoughLinesP(dst, lines, 1, CV_PI/180, nInt, 50, 10 );
  cout<<dst.cols<<dst.rows<<endl;
  //cdst=dst.clone();
  for(i = 0; i < lines.size(); i++ )
  {
    Vec4i l = lines[i];
    line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
  }
 #endif
  cout<<cdst.cols<<cdst.rows<<endl;

  hconcat (src, cdst, cdst);

  imshow(window_name, cdst);
}

int main(int argc, char** argv){

  src = imread( argv[1] );

  if( !src.data ){
    cout<<"cannot open file!"<<endl;
    return -1;
  }

  /// Create a matrix of the same type and size as src (for dst)
  dst.create( src.size(), src.type() );

  cvtColor(src, src_gray, COLOR_BGR2GRAY);

  /// Create a window
  namedWindow(window_name, WINDOW_NORMAL );

  /// Create a Trackbar for user to enter threshold
  createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, HoughTransform );
  createTrackbar( "High Threshold:", window_name, &highThreshold, max_highThreshold, HoughTransform );
  createTrackbar( "Blur times:", window_name, &nTimes, max_nTimes, HoughTransform );
  createTrackbar( "Min. Intersections:", window_name, &nInt, max_nInt, HoughTransform );

  /// Show the image
  HoughTransform(0, 0);

  waitKey(0);
  imwrite( "result.jpg", cdst);

  return 0;
}
