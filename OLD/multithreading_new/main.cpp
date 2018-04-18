//
//  main.cpp
//  OOCV
//
//  Created by Vita on 2/4/17.
//  Copyright © 2017 Vita. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

//using namespace cv;
using namespace std;

void * VideoCap(void *);

void * VideoCap(void *arg)
{
    VideoCapture cap((char *)arg);
    
    if ( !cap.isOpened() )  // if not success, exit program
    {
        cout << "Cannot open the video file" << endl;
        exit(-1);
    }
    
    while(true){
        Mat Webcam;
        cap.read(Webcam);
        resize(Webcam, Webcam, Size(Webcam.cols/2, Webcam.rows/2)); // to half size
        namedWindow( "Display frame",CV_WINDOW_AUTOSIZE);
        imshow("Webcam1", Webcam);
        
        if(waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
}


int main(){
    
    VideoCapture cap(0);
    VideoCapture cap2(1);
    
    if ( !cap.isOpened() )  // if not success, exit program
    {
        cout << "Cannot open the video file" << endl;
        return -1;
    }

//    if ( !cap2.isOpened() )  // if not success, exit program
//    {
//        cout << "Cannot open the video file" << endl;
//        return -1;
//    }

    Mat Webcam;
    Mat Webcam2;

    while(true){
        //Takes x seconds
//        Mat Webcam;
        cap.read(Webcam);
        resize(Webcam, Webcam, Size(Webcam.cols/2, Webcam.rows/2)); // to half size
        namedWindow( "Display frame",CV_WINDOW_AUTOSIZE);
        imshow("Webcam1", Webcam);
        //imshow("Webcam", Webcam);
       

        
        if(waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }
}
