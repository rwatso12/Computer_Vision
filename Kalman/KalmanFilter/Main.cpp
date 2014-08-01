#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

using namespace std;
using namespace cv;

#define drawCross( img, center, color, d )\
line(img, Point(center.x - d, center.y - d), Point(center.x + d, center.y + d), color, 2, CV_AA, 0);\
line(img, Point(center.x + d, center.y - d), Point(center.x - d, center.y + d), color, 2, CV_AA, 0 )\

int main()
{
  Mat frame, thresh_frame;
  vector<Mat> channels;
  VideoCapture capture;
  vector<Vec4i> hierarchy;
  vector<vector<Point> > contours;

  //Create two output windows
  cvNamedWindow( "raw_video", CV_WINDOW_AUTOSIZE );
  cvNamedWindow( "processed_video", CV_WINDOW_AUTOSIZE );

  //Used as storage element for Hough circles
  CvMemStorage* storage = cvCreateMemStorage(0);

  KalmanFilter KF(4, 2, 0);
  Mat_<float> state(4, 1);
  Mat_<float> processNoise(4, 1, CV_32F);
  Mat_<float> measurement(2,1); measurement.setTo(Scalar(0));

  KF.statePre.at<float>(0) = 0;
  KF.statePre.at<float>(1) = 0;
  KF.statePre.at<float>(2) = 0;
  KF.statePre.at<float>(3) = 0;

  KF.transitionMatrix = *(Mat_<float>(4, 4) << 1,0,1,0,   0,1,0,1,  0,0,1,0,  0,0,0,1); // Including velocity
  KF.processNoiseCov = *(cv::Mat_<float>(4,4) << 0.2,0,0.2,0,  0,0.2,0,0.2,  0,0,0.3,0,  0,0,0,0.3);

  setIdentity(KF.measurementMatrix);
  setIdentity(KF.processNoiseCov, Scalar::all(1e-4));
  setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
  setIdentity(KF.errorCovPost, Scalar::all(.1));

  while((char)waitKey(1) != 'q' && capture.grab())
    {
      capture.retrieve(frame);

      split(frame, channels);
      add(channels[0], channels[1], channels[1]);
      subtract(channels[2], channels[1], channels[2]);
      threshold(channels[2], thresh_frame, 50, 255, CV_THRESH_BINARY);
      medianBlur(thresh_frame, thresh_frame, 5);

      findContours(thresh_frame, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

      Mat drawing = Mat::zeros(thresh_frame.size(), CV_8UC1);
      for(size_t i = 0; i < contours.size(); i++)
        {
//          cout << contourArea(contours[i]) << endl;
          if(contourArea(contours[i]) > 500)
            drawContours(drawing, contours, i, Scalar::all(255), CV_FILLED, 8, vector<Vec4i>(), 0, Point());
        }
      thresh_frame = drawing;

      findContours(thresh_frame, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

      drawing = Mat::zeros(thresh_frame.size(), CV_8UC1);
      for(size_t i = 0; i < contours.size(); i++)
        {
//          cout << contourArea(contours[i]) << endl;
          if(contourArea(contours[i]) > 500)
            drawContours(drawing, contours, i, Scalar::all(255), CV_FILLED, 8, vector<Vec4i>(), 0, Point());
        }
      thresh_frame = drawing;

// Get the moments
      vector<Moments> mu(contours.size() );
      for( size_t i = 0; i < contours.size(); i++ )
        { mu[i] = moments( contours[i], false ); }

//  Get the mass centers:
      vector<Point2f> mc( contours.size() );
      for( size_t i = 0; i < contours.size(); i++ )
        { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }

      Mat prediction = KF.predict();
      Point predictPt(prediction.at<float>(0),prediction.at<float>(1));

      for(size_t i = 0; i < mc.size(); i++)
        {
          drawCross(frame, mc[i], Scalar(255, 0, 0), 5);
          measurement(0) = mc[i].x;
          measurement(1) = mc[i].y;
        }

      Point measPt(measurement(0),measurement(1));

      Mat estimated = KF.correct(measurement);
      Point statePt(estimated.at<float>(0),estimated.at<float>(1));

      drawCross(frame, statePt, Scalar(255, 255, 255), 5);

      vector<vector<Point> > contours_poly( contours.size() );
      vector<Rect> boundRect( contours.size() );
      for( size_t i = 0; i < contours.size(); i++ )
       { approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
         boundRect[i] = boundingRect( Mat(contours_poly[i]) );
       }

      for( size_t i = 0; i < contours.size(); i++ )
       {
         rectangle( frame, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2, 8, 0 );
       }


      imshow("Video", frame);
      imshow("Red", channels[2]);
      imshow("Binary", thresh_frame);
    }
  return 0;
}