#include <sstream>
#include <iostream>
#include <vector>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>

using namespace std;
using namespace cv;

int lowerH=0;
int lowerS=0;
int lowerV=0;

//int upperH=180;
int upperH=256;
int upperS=256;
int upperV=256;

//This function threshold the HSV image and create a binary image
IplImage* GetThresholdedImage(IplImage* imgHSV){
 
 IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
 cvInRangeS(imgHSV, cvScalar(lowerH,lowerS,lowerV), cvScalar(upperH,upperS,upperV), imgThresh); 
 
 return imgThresh;

}

//This function create two windows and 6 trackbars for the "Ball" window
void setwindowSettings(){
 cvNamedWindow("RGB");
 cvNamedWindow("HSV");
 
 cvCreateTrackbar("LowerH", "HSV", &lowerH, 256, NULL);
        cvCreateTrackbar("UpperH", "HSV", &upperH, 256, NULL);

  cvCreateTrackbar("LowerS", "HSV", &lowerS, 256, NULL);
        cvCreateTrackbar("UpperS", "HSV", &upperS, 256, NULL);

  cvCreateTrackbar("LowerV", "HSV", &lowerV, 256, NULL);
        cvCreateTrackbar("UpperV", "HSV", &upperV, 256, NULL); 
}


int main(){
 CvCapture* capture =0; 
 
 capture = cvCaptureFromCAM(0);
 if(!capture){
   printf("Capture failure\n");
   return -1;
 }

        IplImage* frame=0;
  
 setwindowSettings();

  //iterate through each frames of the video
 while(true){
 
  frame = cvQueryFrame(capture);
  if(!frame)  break;
  frame=cvCloneImage(frame); 

   IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 
  //cvCvtColor(frame, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV
  //cvCvtColor(frame, imgHSV, CV_BGR2YCrCb);
    cvCvtColor(frame, imgHSV, CV_BGR2Lab);
   IplImage* imgThresh = GetThresholdedImage(imgHSV);
  
  cvShowImage("HSV", imgThresh);
                cvShowImage("RGB", frame);

   //Clean up used images
  cvReleaseImage(&imgHSV);
  cvReleaseImage(&imgThresh);
  cvReleaseImage(&frame);

   //Wait 80mS
  int c = cvWaitKey(80);
  //If 'ESC' is pressed, break the loop
  if((char)c==27 ) break;

 }

  cvDestroyAllWindows();
 cvReleaseCapture(&capture);

       return 0;
}