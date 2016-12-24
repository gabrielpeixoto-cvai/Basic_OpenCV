#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int blurKernel = 1;
int bk_max = 10;
int clusterCount = 2;
int const cc_max = 20;
int attempts = 2;
int const att_max = 10;
int eps = 1;
int const eps_max = 10;
int imFormat = 0;
int const if_max = 2;

char* window_name = "Kmeans clustering";

Mat labels, centers, temp, src, new_image;

void kmeans(int, void*){

src = temp.clone();

int kernel_size = 2*blurKernel + 1;
switch (imFormat){
  case 0:
    //does nothign already BGR format
  break;
  case 1:
    cvtColor(src, src, COLOR_BGR2HSV);
  break;
  case 2:
    cvtColor(src, src, COLOR_BGR2XYZ);
  break;
  default:
    cout<<"wrong option";
}

blur(src, src, cv::Size(kernel_size,kernel_size), cv::Point(-1,-1), cv::BORDER_DEFAULT);

Mat samples(src.rows * src.cols, 3, CV_32F);
for( int y = 0; y < src.rows; y++ )
  for( int x = 0; x < src.cols; x++ )
    for( int z = 0; z < 3; z++)
      samples.at<float>(y + x*src.rows, z) = src.at<Vec3b>(y,x)[z];

kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, eps/1000),
        attempts, KMEANS_PP_CENTERS, centers );


//Mat new_image( src.size(), src.type() );
new_image = src.clone();

for( int y = 0; y < src.rows; y++ )
  for( int x = 0; x < src.cols; x++ )
  {
    int cluster_idx = labels.at<int>(y + x*src.rows,0);
    new_image.at<Vec3b>(y,x)[0] = centers.at<float>(cluster_idx, 0);
    new_image.at<Vec3b>(y,x)[1] = centers.at<float>(cluster_idx, 1);
    new_image.at<Vec3b>(y,x)[2] = centers.at<float>(cluster_idx, 2);
  }

  hconcat (temp, new_image, new_image);

  imshow(window_name, new_image);

  imwrite( "result.jpg", new_image);

}

int main( int argc, char** argv )
{
  temp = imread( argv[1], 1 );
  namedWindow(window_name, WINDOW_NORMAL );

  /// Create a Trackbar for user to enter threshold
  createTrackbar( "Image Format (0 - RGB | 1 - HSV | 2 - XYZ):", window_name, &imFormat, if_max, kmeans);
  createTrackbar( "Blur Kernel (2*n + 1):", window_name, &blurKernel, bk_max , kmeans);
  createTrackbar( "Cluster Count:", window_name, &clusterCount, cc_max, kmeans);
  createTrackbar( "Nmbr of Attempts:", window_name, &attempts, att_max, kmeans);
  createTrackbar( "Epsilon (eps/1000):", window_name, &eps, eps_max, kmeans);

  kmeans(0, 0);

  waitKey(0);
}
