/* on OS X: compile with
> sudo port install opencv
> g++ -L/opt/local/lib -lcxcore -lhighgui -I/opt/local/include/opencv opencv.cpp -o test

* Run with
> ./test bird.jpg
*/ 
#include "highgui.h"

int main( int argc, char * argv[])
{
  IplImage * img = cvLoadImage( argv[1] );
  cvNamedWindow("OpenCV prototype", CV_WINDOW_AUTOSIZE);
  cvShowImage("OpenCV prototype", img);
  cvWaitKey(0);
  cvReleaseImage(&img);
  cvDestroyWindow("OpenCV prototype");
  
  return 0;
}