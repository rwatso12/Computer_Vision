#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{
    // Initialize and allocate memory to load the video stream from camera
    VideoCapture camera(0);

    if (!camera.isOpened())
        return 1;

    while (true)
    {
        // Grab and retrieve each frames of the video sequentially
        Mat frame;
        camera >> frame;

        // Set Region of Interest to the area defined by the box
        cv::Rect roi_1;
        roi_1.x = 0;
        roi_1.y = 0;
        roi_1.width = 320;
        roi_1.height = 480;

		cv::Rect roi_2;
		roi_2.x=320;
		roi_2.y=0;
		roi_2.width=320;
		roi_2.height=480;

        // Crop the original image to the defined ROI
        cv::Mat crop_1 = frame(roi_1);
		cv::Mat crop_2 = frame(roi_2);
        cv::imshow("crop 1", crop_1);
		cv::imshow("crop 2",crop_2);

        int c = cvWaitKey(30);

        if (27 == char(c))
            break;
    }

    return 0;
}
