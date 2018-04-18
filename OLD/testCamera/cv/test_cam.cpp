/*
 ============================================================================
 Name        : testCamera.cu
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : CUDA compute reciprocals
 ============================================================================
 */

/*#include <iostream>
#include <numeric>
#include <stdlib.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
using namespace cv;
int main(){
	VideoCapture cap(1);
    if(!cap.isOpened()){
        return -1;
    }
    while(true){
            Mat webcam;
            cap.read(webcam);
            imshow("webcam",webcam);
            std::cout <<"initialized" << std::endl;
           // Mat frame;
           // cap >> frame;
            //imwrite("capture.jpg",frame);
            //if(waitKey(30) >= 0) break;
    }

}*/

#include <stdio.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv){
		VideoCapture cap0(1);
		VideoCapture cap(2);
		VideoCapture cap2(3);
		VideoCapture cap3(4);
		if (!cap0.isOpened())
		{
			cout << "Cannot open the video cam 0" << endl;
			return -1;
		}
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
	    //double fps = cap.get(5); //get the frames per seconds of the video
	    //cout << "Frame per seconds : " << fps << endl;
	    cap0.set(CV_CAP_PROP_FRAME_WIDTH,1920);
	    cap0.set(CV_CAP_PROP_FRAME_HEIGHT,1080);
	    cap.set(CV_CAP_PROP_FRAME_WIDTH,1920);
	    cap.set(CV_CAP_PROP_FRAME_HEIGHT,1080);
	    cap2.set(CV_CAP_PROP_FRAME_WIDTH,1920);
	    cap2.set(CV_CAP_PROP_FRAME_HEIGHT,1080);
	    cap3.set(CV_CAP_PROP_FRAME_WIDTH,1920);
	    cap3.set(CV_CAP_PROP_FRAME_HEIGHT,1080);

	while(1){
		Mat image0;
		Mat image1;
		Mat image2;
		Mat image3;
		cap0.read(image0);
		cap.read(image1);
		cap2.read(image2);
		cap3.read(image3);
		 if (!image0.empty() && !image1.empty() && !image2.empty() && !image3.empty()) {
			imshow("image0",image0);
			imshow("image1",image1);
			imshow("image2",image2);
			imshow("image3",image3);
		 }
		
	     if(waitKey(100) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
	       {
	                cout << "esc key is pressed by user" << endl;
	                break;
	       }
	}
	cap0.release();
	cap.release();
	cap2.release();
	cap3.release();
	return 0;
}