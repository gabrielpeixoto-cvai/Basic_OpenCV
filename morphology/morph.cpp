#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <iostream>

using namespace cv;
using namespace std;


Mat src;
Mat dst, interm;

int caso;

int Nerosions = 0;
int Ndilations = 0;
int Dilkrnl = 0;
int Erokrnl = 0;
int const max_Nerosions = 10;
int const max_Ndilations = 10;
int const max_Dilkrnl = 11;
int const max_Erokrnl = 11;

char* window_name = "Morphology";

void Morphology(int, void*){
  int i;
  Mat Eelement = getStructuringElement( MORPH_RECT,
                                       Size( 2*Erokrnl + 1, 2*Erokrnl+1 ),
                                       Point( Erokrnl, Erokrnl ) );

  Mat Delement = getStructuringElement( MORPH_RECT,
                                      Size( 2*Dilkrnl + 1, 2*Dilkrnl+1 ),
                                      Point( Dilkrnl, Dilkrnl ) );
  interm = src.clone();
  dst = src.clone();
  switch (caso){
   case 1:
      for(i=0;i<Ndilations;i++){
        dilate(interm, dst, Delement);
        interm = dst.clone();
      }
      for(i=0;i<Nerosions;i++){
        erode( interm, dst, Eelement);
        interm = dst.clone();
      }
   break;

   case 2:
      Dilkrnl = 1;
      Erokrnl = 1;
      Delement = getStructuringElement( MORPH_RECT,
                                      Size( 2*Dilkrnl + 1, 2*Dilkrnl+1 ),
                                      Point( Dilkrnl, Dilkrnl ) );
      Eelement = getStructuringElement( MORPH_RECT,
                                      Size( 2*Erokrnl + 1, 2*Erokrnl+1 ),
                                      Point( Erokrnl, Erokrnl ) );
      /*
      close to remove the white points on black background
      open to fill the black points on white letters
      then the noise is pratically removed
      to get the image with only the noise, just subtract the denoised image
      from the original one
      */
      //close
      erode(interm,dst,Eelement);
      dilate(dst,dst,Delement);
      //opening
      dilate(dst,dst,Delement);
      erode(dst,dst,Eelement);
      //erode(interm,dst,Eelement);
      imshow(window_name, dst);
      waitKey(0);
      imshow(window_name, dst);
      subtract(src,dst,dst);
      waitKey(0);


   break;

   case 3:
      Dilkrnl = 1;
      Erokrnl = 1;
      Delement = getStructuringElement( MORPH_RECT,
                                      Size( 2*Dilkrnl + 1, 2*Dilkrnl+1 ),
                                      Point( Dilkrnl, Dilkrnl ) );
      Eelement = getStructuringElement( MORPH_RECT,
                                      Size( 2*Erokrnl + 1, 2*Erokrnl+1 ),
                                      Point( Erokrnl, Erokrnl ) );

      //based on empirical experiments
      /*
      close to remove the white points on black background
      open to fill the black points on white letters
      with this the noise is removed
      do this for both images and the erode one
      subtract the denoised image from the eroded one and you get the contour
      */
      //close
      erode(interm,dst,Eelement);
      dilate(dst,dst,Delement);
      //opening
      dilate(dst,dst,Delement);
      erode(dst,dst,Eelement);
      //erode(interm,dst,Eelement);
      imshow(window_name, dst);
      waitKey(0);
      //close
      erode(interm,interm,Eelement);
      dilate(interm,interm,Delement);
      //opening
      dilate(interm,interm,Delement);
      erode(interm,interm,Eelement);
      //dilata para tentar manter os contornos mais retos
      dilate(interm,interm,Delement);
      //erode(interm,dst,Eelement);
      imshow(window_name, interm);
      waitKey(0);
      erode(dst,dst,Eelement);
      subtract(interm,dst,dst);

   break;

   default:
     printf("Opcao errada");
}

  hconcat (src, dst, dst);

  imshow(window_name, dst);
}

int main(int argc, char** argv){

  src = imread( argv[1] );
  caso = atoi(argv[2]);

  if( !src.data ){
    cout<<"cannot open file!"<<endl;
    return -1;
  }

  /// Create a window
  namedWindow(window_name, WINDOW_NORMAL );

  switch (caso){
   case 1:
      /// Create a Trackbar for user to enter threshold
      createTrackbar( "Nbr of erosions:", window_name, &Nerosions, max_Nerosions, Morphology );
      createTrackbar( "Nbr of dilations:", window_name, &Ndilations, max_Ndilations, Morphology );
      createTrackbar( "Dilation Kernel Size:", window_name, &Dilkrnl, max_Dilkrnl, Morphology );
      createTrackbar( "Erosion Kernel Size:", window_name, &Erokrnl, max_Erokrnl, Morphology );
   break;

   case 2:

   break;

   case 3:

   break;

   default:
     printf("Opcao errada");
}

  /// Show the image
  Morphology(0, 0);

  waitKey(0);
  imwrite( "result.jpg", dst);

  return 0;
}
