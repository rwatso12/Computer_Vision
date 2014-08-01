#include <sstream>
#include <iostream>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include "HomingBeacon.h"

//initial min and max HSV filter values.
//these will be changed using trackbars
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;
//default capture width and height
const int FRAME_WIDTH = 1020;
const int FRAME_HEIGHT = 724;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = 90*90;
//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";
void on_trackbar( int, void* )
{//This function gets called whenever a
	// trackbar position is changed





}
string intToString(int number){


	std::stringstream ss;
	ss << number;
	return ss.str();
}
void createTrackbars(){
	//create window for trackbars


	namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf( TrackbarName, "H_MIN", H_MIN);
	sprintf( TrackbarName, "H_MAX", H_MAX);
	sprintf( TrackbarName, "S_MIN", S_MIN);
	sprintf( TrackbarName, "S_MAX", S_MAX);
	sprintf( TrackbarName, "V_MIN", V_MIN);
	sprintf( TrackbarName, "V_MAX", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
	createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
	createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
	createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
	createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
	createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
	createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );


}
void drawObject(vector<HomingBeacon> TheHomingBeacons,Mat &frame){

	for(int i=0; i<TheHomingBeacons.size(); i++){

		TheHomingBeacons.at(i).getxPos();
		TheHomingBeacons.at(i).getyPos();
		TheHomingBeacons.at(i).getarea();

	cv::circle(frame,cv::Point(TheHomingBeacons.at(i).getxPos(),TheHomingBeacons.at(i).getyPos()),10,cv::Scalar(0,0,255));
	cv::putText(frame,intToString(TheHomingBeacons.at(i).getxPos())+ " , " + intToString(TheHomingBeacons.at(i).getyPos()),cv::Point(TheHomingBeacons.at(i).getxPos(),TheHomingBeacons.at(i).getyPos()+20),1,1,Scalar(0,255,0));
	cv::putText(frame,TheHomingBeacons.at(i).getType(),cv::Point(TheHomingBeacons.at(i).getxPos(),TheHomingBeacons.at(i).getyPos()-30),1,2,TheHomingBeacons.at(i).getColor());

	cv::putText(frame,intToString(TheHomingBeacons.at(i).getarea()),cv::Point(TheHomingBeacons.at(i).getxPos(),TheHomingBeacons.at(i).getyPos()-20),1,1,Scalar(0,255,0));
}
}
void morphOps(Mat &thresh){

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle

	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);


	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);



}
void trackFilteredObject(Mat threshold,Mat HSV, Mat &cameraFeed){

	vector <HomingBeacon> SideOnes;

	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				int area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				if((area>MIN_OBJECT_AREA) && (area<MAX_OBJECT_AREA)){

					HomingBeacon SideOne; 

					SideOne.setxPos( moment.m10/area );
					SideOne.setyPos( moment.m01/area ); 
					SideOne.setarea( moment.m00 );

					SideOnes.push_back(SideOne);

					objectFound = true;

				}else objectFound = false;


			}
			//let user know you found an object
			if(objectFound ==true){
				//draw object location on screen
				drawObject(SideOnes,cameraFeed);}

		}else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
	}
}
void trackFilteredObject(HomingBeacon TheHomingBeacon,Mat threshold,Mat HSV, Mat &cameraFeed){

	vector <HomingBeacon> SideOnes;

	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				int area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				if((area>MIN_OBJECT_AREA) && (area<MAX_OBJECT_AREA)){

					HomingBeacon SideOne; 

					SideOne.setxPos( moment.m10/area );
					SideOne.setyPos( moment.m01/area ); 
					SideOne.setarea( moment.m00 );
					SideOne.setType(TheHomingBeacon.getType());
					SideOne.setColor(TheHomingBeacon.getColor());

					SideOnes.push_back(SideOne);

					objectFound = true;

				}else objectFound = false;


			}
			//let user know you found an object
			if(objectFound ==true){
				//draw object location on screen
				drawObject(SideOnes,cameraFeed);}

		}else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
		}
}
int main(int argc, char* argv[])
{
	//if we would like to calibrate our filter values, set to true.
	bool calibrationMode = true;
	
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	Mat threshold;
	Mat HSV;

	if(calibrationMode){
		//create slider bars for HSV filtering
		createTrackbars();
	}
	//video capture object to acquire webcam feed
	VideoCapture capture;
	//open capture object at location zero (default location for webcam)
	capture.open(1);
	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	while(1){
		//store image to matrix
		capture.read(cameraFeed);
		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);

		if(calibrationMode==true){
		//if in calibration mode, we track objects based on the HSV slider values.
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
		morphOps(threshold);
		imshow(windowName2,threshold);
		trackFilteredObject(threshold,HSV,cameraFeed);
		}else{

			HomingBeacon SideOne("SideOne"), SideTwo("SideTwo");


			cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
			inRange(HSV,SideOne.getHSVmin(),SideOne.getHSVmax(),threshold);
			morphOps(threshold);
			trackFilteredObject(SideOne,threshold,HSV,cameraFeed);

			cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
			inRange(HSV,SideTwo.getHSVmin(),SideTwo.getHSVmax(),threshold);
			morphOps(threshold);
			trackFilteredObject(SideTwo,threshold,HSV,cameraFeed);
		}

		//show frames 
		//imshow(windowName2,threshold);

		imshow(windowName,cameraFeed);
		//imshow(windowName1,HSV);


		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		waitKey(30);
	}






	return 0;
}
