#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <string.h>
#include <opencv\cv.h>
#include <opencv\cvaux.h>
#include <opencv\cxcore.h>
#include <opencv\highgui.h>
#include "opencv2/objdetect/objdetect.hpp"

using namespace cv;
using namespace std;


int main(int, char**)
{ 
CvCapture* capture = cvCaptureFromCAM(1);

IplImage* frame;
    while(1) {
        frame = cvQueryFrame( capture );    

        IplImage* out = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 3 );
        IplImage* gray_out = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
        IplImage* canny_out = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
        cvSmooth( frame, out, CV_GAUSSIAN, 19, 0 );
        cvCvtColor(out , gray_out, CV_RGB2GRAY);
        cvCanny( gray_out, canny_out, 10, 30, 3 );
		
		

        if( !frame ) break;
        cvShowImage( "Edge", canny_out );
        char c = cvWaitKey(33);
        if( c == 27 ) break;
    }
    cvReleaseCapture( &capture );
    cvDestroyWindow( "Edge" );
    return 0;
}
