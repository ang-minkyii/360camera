/*
 * main.cu
 *
 *  Created on: 24 Nov 2016
 *      Author: andy
 */

#include <stdio.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/gpu/gpu.hpp>

#define FWIDTH 1920
#define FHEIGHT 1080

using namespace cv;
using namespace std;
using namespace gpu;

int main(int argc, char **argv){



		VideoCapture cap(0);
		VideoCapture cap2(1);
		//VideoCapture cap3(2gst-launch-1.0 -v v4l2src device=/dev/video2 ! image/jpeg, width=1920,height=1080,framerate=30/1 ! jpegparse ! jpegdec ! videoconvert ! videoscale ! xvimagesink sync=false v4l2src device=/dev/video0");
		VideoCapture cap3(2);
		VideoCapture cap4(4);

	    if (!cap.isOpened())  // if not success, exit program
	    {
	        cout << "Cannot open the video cam 1" << endl;
	        return -1;
	    }

	    if (!cap2.isOpened())  // if not success, exit program
	   {
	       cout << "Cannot open the video cam 2" << endl;
	       return -1;
	   }

	   if (!cap3.isOpened())  // if not success, exit program
	   {
		   cout << "Cannot open the video cam 3" << endl;
	       return -1;
	    }
	   if(!cap4.isOpened()){
		   cout << "Cannot open the video cam 4" << endl;
	   }

	    cap.set(CV_CAP_PROP_FOURCC,CV_FOURCC('M','J','P','G'));
	    cap.set(CV_CAP_PROP_FRAME_WIDTH,FWIDTH);
	    cap.set(CV_CAP_PROP_FRAME_HEIGHT,FHEIGHT);
	    cap.set(CV_CAP_PROP_EXPOSURE,0.5);
	    cap.set(CV_CAP_PROP_BRIGHTNESS,0.5);
	    cap.set(CV_CAP_PROP_FPS,30);
	    cap2.set(CV_CAP_PROP_FOURCC,CV_FOURCC('M','J','P','G'));
	    cap2.set(CV_CAP_PROP_FRAME_WIDTH,FWIDTH);
	    cap2.set(CV_CAP_PROP_FRAME_HEIGHT,FHEIGHT);
	    cap2.set(CV_CAP_PROP_EXPOSURE,0.5);
	    cap2.set(CV_CAP_PROP_BRIGHTNESS,0.5);
	    cap2.set(CV_CAP_PROP_FPS,30);
	    cap3.set(CV_CAP_PROP_FOURCC,CV_FOURCC('M','J','P','G'));
	    cap3.set(CV_CAP_PROP_FRAME_WIDTH,FWIDTH);
	    cap3.set(CV_CAP_PROP_FRAME_HEIGHT,FHEIGHT);
	    cap3.set(CV_CAP_PROP_EXPOSURE,0.5);
	    cap3.set(CV_CAP_PROP_BRIGHTNESS,0.5);
	    cap3.set(CV_CAP_PROP_FPS,30);
	    cap4.set(CV_CAP_PROP_FOURCC,CV_FOURCC('M','J','P','G'));
	    cap4.set(CV_CAP_PROP_FRAME_WIDTH,FWIDTH);
	    cap4.set(CV_CAP_PROP_FRAME_HEIGHT,FHEIGHT);
	    cap4.set(CV_CAP_PROP_EXPOSURE,0.5);
	    cap4.set(CV_CAP_PROP_BRIGHTNESS,0.5);
	    cap4.set(CV_CAP_PROP_FPS,30);

	    int key;
	    Mat image1;
	    Mat image2;
	    Mat image3;
	    Mat image4;


		VideoWriter video1("out1.avi",CV_FOURCC('M','J','P','G'),10,Size(FWIDTH,FHEIGHT),true);
		VideoWriter video2("out2.avi",CV_FOURCC('M','J','P','G'),10,Size(FWIDTH,FHEIGHT),true);
		VideoWriter video3("out3.avi",CV_FOURCC('M','J','P','G'),10,Size(FWIDTH,FHEIGHT),true);
		VideoWriter video4("out4.avi",CV_FOURCC('M','J','P','G'),10,Size(FWIDTH,FHEIGHT),true);

		namedWindow("image1",1);
		namedWindow("image2",1);
		namedWindow("image3",1);
		namedWindow("image4",1);

	while(1){
		cap >> image1;
		cap2 >> image2;
		cap3 >> image3;
		cap4 >> image4;

		video1.write(image1);
		video2.write(image2);
		video3.write(image3);
		video4.write(image4);

		imshow("image1", image1);
		imshow("image2", image2);
		imshow("image3", image3);
		imshow("image4", image4);

		key=waitKey(30);

	    if(key == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
	    {
	    	imwrite("pic_1.jpg",image1);
	    	imwrite("pic_2.jpg",image2);
	    	imwrite("pic_3.jpg",image3);
	    	imwrite("pic_4.jpg",image4);
	         break;
	    }
	}

	return 0;
}





