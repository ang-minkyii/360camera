#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int, char**)
{   

//    "v4l2src device=\"/dev/video0\" ! video/x-raw,width=1920,height=1080,format=BGR ! appsink"
//    VideoCapture cap("v4l2src device=/dev/video1 ! video/x-raw，image/jpeg, width=1920,height=1080,framerate=30/1，format=RGB ! jpegparse ! nvjpegdec ! appsink");
    //VideoCapture cap("nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)2592, height=(int)1944, format=(string)I420, framerate=(fraction)24/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)I420 ! videoconvert ! video/x-raw, format=(string)BGR ! appsink");
   //VideoCapture cap("nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)640, height=(int)480, format=(string)I420, framerate=(fraction)30/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)I420 ! videoconvert ! video/x-raw, format=(string)BGR ! appsink");
   VideoCapture cap("v4l2src device=/dev/video0 ! video/x-raw, framerate=30/1, width=1920, height=1080, format=RGB ! videorate ! videoconvert ! appsink");
    //VideoCapture cap1("v4l2src device=/dev/video1 ! video/x-raw, framerate=30/1, width=1920, height=1080, format=RGB ! videorate ! videoconvert ! appsink");
    //VideoCapture cap2("v4l2src device=/dev/video2 ! video/x-raw, framerate=30/1, width=1920, height=1080, format=RGB ! videoconvert ! appsink");
   // gst-launch-1.0 v4l2src device="/dev/video0" ! video/x-raw,width=640,height=480 ! autovideosink

   // VideoCapture cap("v4l2src device=/dev/video0 ! video/x-raw,width=640,height=480 ! autovideosink");
    //VideoCapture cap(1); // open the default camera
    //if(!cap.isOpened() && !cap1.isOpened())  // check if we succeeded
    if( !cap.isOpened()){
	cout<<"hello"<<endl;
   //if(!cap.isOpened() && !cap1.isOpened() && !cap2.isOpened())
        return -1;}

    //Mat edges;
    namedWindow("frame",1);
    //namedWindow("frame1",2);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        
      // Mat frame1;
       //cap1 >> frame1;

       //Mat frame2;
       //cap2 >> frame2;

       imshow("frame", frame);
      // imshow("frame1",frame1);
       //imshow("frame2",frame2);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
