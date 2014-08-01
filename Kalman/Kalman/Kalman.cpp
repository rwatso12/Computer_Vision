#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/highgui/highgui_c.h>

using namespace cv;
using namespace std;


struct mouse_info_struct { int x,y; };
struct mouse_info_struct mouse_info = {-1,-1}, last_mouse;

vector<Point> mousev,kalmanv;

void on_mouse(int event, int x, int y, int flags, void* param) {
	{
		last_mouse = mouse_info;
		mouse_info.x = x;
		mouse_info.y = y;
		
	}
}

int main (int argc, char *argv[]) {
    Mat img(500, 500, CV_8UC3);
    KalmanFilter KF(4, 2, 0);
    Mat_<float> state(4, 1); /* (x, y, Vx, Vy) */
    Mat processNoise(4, 1, CV_32F);
    Mat_<float> measurement(2,1); measurement.setTo(Scalar(0));
    char code = (char)-1;
	
    for(;;)
    {
		if (mouse_info.x < 0 || mouse_info.y < 0) {
			imshow("mouse kalman", img);
			waitKey(30);
			continue;
		}
        KF.statePre.at<float>(0) = mouse_info.x;
		KF.statePre.at<float>(1) = mouse_info.y;
		KF.statePre.at<float>(2) = 0;
		KF.statePre.at<float>(3) = 0;
		KF.transitionMatrix = *(Mat_<float>(4, 4) << 1,0,1,0,   0,1,0,1,  0,0,1,0,  0,0,0,1);
		
        setIdentity(KF.measurementMatrix);
        setIdentity(KF.processNoiseCov, Scalar::all(1e-4));
        setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
        setIdentity(KF.errorCovPost, Scalar::all(.1));
		
		
        for(;;)
        {
			
            Mat prediction = KF.predict();
            Point predictPt(prediction.at<float>(0),prediction.at<float>(1));
			
            measurement(0) = mouse_info.x;
			measurement(1) = mouse_info.y;
			
			Point measPt(measurement(0),measurement(1));
			mousev.push_back(measPt);
			Mat estimated = KF.correct(measurement);
			Point statePt(estimated.at<float>(0),estimated.at<float>(1));
			kalmanv.push_back(statePt);
			
            img = Scalar::all(0);

			for (int i = 0; i < kalmanv.size()-1; i++) {
				line(img, kalmanv[i], kalmanv[i+1], Scalar(0,255,0), 1);
			}
			
            imshow( "mouse kalman", img );
            code = (char)waitKey(100);
        }
    }
}