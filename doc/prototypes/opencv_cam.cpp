/* on OS X: compile with
> sudo port install opencv
> g++ -L/opt/local/lib -lcxcore -lhighgui -I/opt/local/include/opencv opencv_cam.cpp -o test

* Run with
> ./test
*/ 
#include "highgui.h"

int main( int argc, char * argv[])
{
  cvNamedWindow("OpenCV prototype", CV_WINDOW_AUTOSIZE);
  CvCapture * capture = cvCreateCameraCapture(0);
  IplImage * frame;
  while (1)
  {
    frame = cvQueryFrame( capture );
    if (!frame) break;
    cvShowImage("OpenCV prototype", frame);
    char c = cvWaitKey(1);
    if (c == 27) break;
  }
  cvReleaseCapture(&capture);
  cvDestroyWindow("OpenCV prototype");
  return 0;
}