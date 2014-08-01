/*
* white_detection_v1
* Author: Jared Strader 
* Created: 3/14/14
* Last Updated: 3/19/2014
*
* Errors
* -1:Cannot open video device.
* -2:Cannot read frame from video stream.
*/

//#define _CRT_SECURE_NO_WARNINGS //avoid fopen error in visual studio

//SURF Detection Headers
#include <iostream>
#include <stdio.h>
#include <iomanip> //only needed for debugging (setprecision)
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp> //for resize image
#include <vector> //hue detection
#include <sstream> //hue detection
#include <opencv2/video/tracking.hpp> //hue detection

//using namespace std;
//using namespace cv;


double get_CPU_time_usage(clock_t clock1, clock_t clock2)
{
	double diffticks = clock1 - clock2;
	double diffms = (diffticks * 1000) / CLOCKS_PER_SEC;
	return diffms;
}


//Settings

//Default capture width and height
const int FRAME_WIDTH = 1020;
const int FRAME_HEIGHT = 724;


//Help function
static void help()
{
	std::cout << "This program detects white objects."
		<< std::endl << "" << std::endl << std::endl;
}


int main(int argc, char** argv)
{
	help();

	//Start Video Feed
	cv::VideoCapture cap(1);

	if (!cap.isOpened())
	{
		std::cout << "Can not open video capture device." << std::endl;
		exit(-1);
	}

	//Video feed settings
	cap.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH); //set frame width
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT); //set frame height

	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);   //get frame width
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get fram height
	std::cout << "Frame size: " << dWidth << "x" << dHeight << std::endl; //print frame size

	//Tracking Window
	cv::namedWindow("Tracking Window", cv::WINDOW_AUTOSIZE);

	//Video Capture Variables
	cv::Mat frame;
	
	
	const double threshold_val = 225;
	const double max_BINARY_value = 255;
	const int threshold_type = 3;

	while (1)
	{
		//start timer to measure performance
		clock_t begin_time = clock();

		//Capture Video Frame
		if (!cap.read(frame))
		{
			std::cout << "Cannot read a frame from video stream." << std::endl;
			exit(-2);
		}


		//cv::vector<cv::Mat> CV_8U channel;
		cv::Mat R, G, B;
		cv::Mat channel[3];
		split(frame, channel);
		channel[0].convertTo(B, CV_8U);
		channel[1].convertTo(G, CV_8U);
		channel[2].convertTo(R, CV_8U);
		cv::Mat ch1 = cvCreateImage(B.size(), 8, 1);
		cv::Mat ch2 = cvCreateImage(G.size(), 8, 1);
		cv::Mat ch3 = cvCreateImage(R.size(), 8, 1);
		cv::Mat temp = cvCreateImage(R.size(), 8, 1);
		cv::Mat res = cvCreateImage(R.size(), 8, 1);

		//cv::Mat* hh1 = &B;
		//cv::Mat* hh2 = &G;
		//cv::Mat* hh3 = &R;

		threshold(B, ch1, threshold_val, max_BINARY_value, threshold_type);
		threshold(G, ch2, threshold_val, max_BINARY_value, threshold_type);
		threshold(R, ch3, threshold_val, max_BINARY_value, threshold_type);
		
		cv::bitwise_and(ch1, ch2, temp);
		cv::bitwise_and(ch3, temp, res);
		imshow("Resulting Image", res);


		//cvThreshold(&channel[0], &R, threshold_val, max_BINARY_value, threshold_type);
		//cvThreshold(&channel[1], &G, threshold_val, max_BINARY_value, threshold_type);
		//cvThreshold(&channel[2], &B, threshold_val, max_BINARY_value, threshold_type);
		//cv::imshow("red channel", R);

		//IplImage* img = new IplImage(frame);
		//cvSplit(img, &R, &G, &B, NULL);
		//cv::Mat R(cv::Size(dWidth, dHeight), CV_MAKETYPE(CV_8U, 1));
		//cv::Mat G(cv::Size(dWidth, dHeight), CV_MAKETYPE(CV_8U, 1));
		//cv::Mat B(cv::Size(dWidth, dHeight), CV_MAKETYPE(CV_8U, 1));
		//cv::Mat CH1(cv::Size(dWidth, dHeight), CV_MAKETYPE(CV_8U, 1));
		//cv::Mat CH2(cv::Size(dWidth, dHeight), CV_MAKETYPE(CV_8U, 1));
		//cv::Mat CH3(cv::Size(dWidth, dHeight), CV_MAKETYPE(CV_8U, 1));

		if (cv::waitKey(30) == 27) //wait for 'esc' key press for 30ms.
		{
			std::cout << std::endl << "Press enter to exit program." << std::endl;
			break;
		}

		imshow("Tracking Window", frame);

		//print execution time
		clock_t end_time = clock();
		std::cout << "Frequency = " << double(get_CPU_time_usage(end_time, begin_time))
			<< " ms (" << 1 / (double(get_CPU_time_usage(end_time, begin_time)) / 1000) << " Hz)\n\n";
	}

	cvWaitKey(0);
	return 0;
}