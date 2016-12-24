#include "ocv_utils.h"

#define ROI_SIZE 3

#define window11

using namespace cv;
using namespace std;
using namespace ocv_utils;



/**  @function main */
int main( int argc, char** argv ){

  Mat src, dst, src_hist, dst_hist;


  /// Load image
  src = imread( argv[1], 1 );


  if( !src.data ){
    cout<<"Usage: ./localHist <path_to_image>"<<endl;
    return -1;
  }

  /// Convert to grayscale
  cvtColor( src, src, CV_BGR2GRAY );

  cout<<"initial state"<<endl;
  cout<<"src="<<src.rows<<"  "<<src.cols<<endl;
  cout<<"dst="<<dst.rows<<"  "<<dst.cols<<endl;

  dst=src.clone();


  cout<<"After cloning"<<endl;
  cout<<"src="<<src.rows<<"  "<<src.cols<<endl;
  cout<<"dst="<<dst.rows<<"  "<<dst.cols<<endl;

  if((src.rows%ROI_SIZE)!=0 || (src.cols%ROI_SIZE)!=0){
    resize (src, src, Size(ROI_SIZE*70,ROI_SIZE*70), 0, 0, INTER_CUBIC);
    resize (dst, dst, Size(ROI_SIZE*70,ROI_SIZE*70), 0, 0, INTER_CUBIC);
  }

  //cout<<"After Resize"<<endl;
  //cout<<"src="<<src.rows<<"  "<<src.cols<<endl;
  //cout<<"dst="<<dst.rows<<"  "<<dst.cols<<endl;

  //problema esta nos limites do for
  int i,j;
	for(i=((ROI_SIZE-1)/2);i<(src.rows-((ROI_SIZE-1)/2));i++){
	   for(j=((ROI_SIZE-1)/2);j<(src.cols-((ROI_SIZE-1)/2));j++){
				Mat roi=src(Rect(i-((ROI_SIZE-1)/2), j-((ROI_SIZE-1)/2), ROI_SIZE, ROI_SIZE));
				equalizeHist(roi, roi);
	  }
	}

  //debug
  cout<<src.rows<<"  "<<src.cols<<endl;
  cout<<dst.rows<<"  "<<dst.cols<<endl;

  // create histogram plots
  src_hist = plotHistogram(calcHistogram(src));
  dst_hist = plotHistogram(calcHistogram(dst));


  //debug
  cout<<"histogram done!"<<endl;

  //resize and concatenate the images to show in one window
  resize (src, src, Size(512*src.cols/src.rows,512), 0, 0, INTER_CUBIC);
	resize (dst, dst, Size(512*dst.cols/dst.rows,512), 0, 0, INTER_CUBIC);

  //prints for debug
  cout<<src.rows<<"  "<<src.cols<<endl;
  cout<<dst.rows<<"  "<<dst.cols<<endl;
  resize (src_hist, src_hist, Size(512*src_hist.cols/src_hist.rows,512), 0, 0, INTER_CUBIC);
  resize (dst_hist, dst_hist, Size(512*dst_hist.cols/dst_hist.rows,512), 0, 0, INTER_CUBIC);

  //prints for debug
  cout<<src.rows<<"  "<<src.cols<<endl;
  cout<<dst.rows<<"  "<<dst.cols<<endl;
	hconcat (src, src_hist, src);
	hconcat (dst, dst_hist, dst);
	vconcat (dst, src, dst);

  imwrite( "result.jpg", dst);

	namedWindow(argv[0], CV_WINDOW_NORMAL);
	imshow(argv[0], dst);

	waitKey(0);

  return 0;
}
