#include "opencv2/opencv.hpp"
#include <cstdio>
#include <ctime>

using namespace cv;
using namespace std;

int main(int, char**)
{   
	clock_t start;
	clock_t end;
//    "v4l2src device=\"/dev/video0\" ! video/x-raw,width=1920,height=1080,format=BGR ! appsink"
  VideoCapture cap(" filesrc location=test.mp4 ! qtdemux ! h264parse ! omxh264dec ! videoconvert ! video/x-raw, format=(string)BGR ! appsink ");
  //VideoCapture cap( "nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720, format=(string)I420, framerate=(fraction)24/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)I420 ! videoconvert ! video/x-raw, format=(string)BGR ! appsink");
    //VideoCapture cap("nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)2592, height=(int)1944, format=(string)I420, framerate=(fraction)24/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)I420 ! videoconvert ! video/x-raw, format=(string)BGR ! appsink");
   //VideoCapture cap("nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)640, height=(int)480, format=(string)I420, framerate=(fraction)30/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)I420 ! videoconvert ! video/x-raw, format=(string)BGR ! appsink");
   //VideoCapture cap("v4l2src device=/dev/video0 ! video/x-raw, framerate=30/1, width=1920, height=1080, format=RGB ! videorate ! videoconvert ! appsink");
   // VideoCapture cap("v4l2src device=/dev/video1 ! video/x-raw, framerate=30/1, width=1920, height=1080, format=RGB ! videorate ! videoconvert ! appsink");
    //VideoCapture cap2("v4l2src device=/dev/video2 ! video/x-raw, framerate=30/1, width=1920, height=1080, format=RGB ! videoconvert ! appsink");
   // gst-launch-1.0 v4l2src device="/dev/video0" ! video/x-raw,width=640,height=480 ! autovideosink

   // VideoCapture cap("v4l2src device=/dev/video0 ! video/x-raw,width=640,height=480 ! autovideosink");

    //VideoCapture cap(1); // open the default camera
    //cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
	//cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
    //if(!cap.isOpened() && !cap1.isOpened())  // check if we succeeded
    if( !cap.isOpened()){
	       cout<<"hello"<<endl;
   //if(!cap.isOpened() && !cap1.isOpened() && !cap2.isOpened())
        return -1;}

    //Mat rgb;
    namedWindow("frame",1);
    //namedWindow("frame1",2);
    for(;;)
    {
    	start = clock();
     // Mat frame;
      Mat frame(1920,1080, CV_8UC3, Scalar(0,0,255));
      cap >> frame; // get a new frame from camera
        //cvtColor(frame, rgb, CV_YUV2BGR);
        
      // Mat frame1;
       //cap1 >> frame1;

       //Mat frame2;
       //cap2 >> frame2;

       imshow("frame", frame);
       end = clock();
       cout << "FPS:" << 1/((end-start)/(double)CLOCKS_PER_SEC) << endl;
      // imshow("frame1",frame1);
       //imshow("frame2",frame2);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
