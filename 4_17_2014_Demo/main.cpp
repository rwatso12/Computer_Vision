#include <vector>
#include <string>
#include <iostream>
#include <opencv\highgui.h>
#include <opencv\cv.h>

using namespace std; 
using namespace cv;

int H_MIN = 161;
int H_MAX = 180;
int S_MIN = 117;
int S_MAX = 177;
int V_MIN = 89;
int V_MAX = 226;

int c1_;
double area_;

const int FRAME_WIDTH = 1024;
const int FRAME_HEIGHT = 720;
const int MAX_NUM_OBJECTS=50;
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;

string intToString(int number){


	std::stringstream ss;
	ss << number;
	return ss.str();
}

void morphOps(Mat &thresh){

	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));
	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);
	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);

}

void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed){

	Mat temp;
	threshold.copyTo(temp);
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
	 if(hierarchy.size()==0){
		 c1_=0;
		 cout<<c1_<<endl;
	 }
		int numObjects = hierarchy.size();
        if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				area_ = moment.m00;
                if(area_>MIN_OBJECT_AREA && area_<MAX_OBJECT_AREA && area_>refArea){
					x = moment.m10/area_;
					y = moment.m01/area_;
					objectFound = true;
					refArea = area_;
				}else objectFound = false;


			}
			if(objectFound ==true){
				c1_=1;
				cout<<c1_<<"  x=  "<<x<<"  y=  "<<y<<"  area=  "<<area_<<endl;
			}else{
				c1_=0;
				cout<<c1_<<endl;
		}
	  }
   }else {
	   c1_=0;
	   cout<<c1_<<endl;
	}
}

int main(int argc, char* argv[])
{
    bool trackObjects = true;
    bool useMorphOps = true;
	Mat cameraFeed;
	Mat HSV;
	Mat threshold;
	int x=0, y=0;
	VideoCapture capture;
	capture.open(1);
	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
	while(1){
		capture.read(cameraFeed);
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
		if(useMorphOps)
		morphOps(threshold);
		if(trackObjects)
			trackFilteredObject(x,y,threshold,cameraFeed);
	}

	return 0;
}
