//
//  main.cpp
//  testStitching
//
//  Created by Didi Chi on 1/1/17.
//  Copyright © 2017 dako. All rights reserved.
//

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#define FINALWIDTH 1566
#define FINALHEIGHT 640
// MIDDLEWIDTH 100 LEFT 1256 RIGHT 723 WORKS FOR 1920 * 1080
#define MIDDLEWIDTH 100//50 //100//110//270//50
#define LEFT 991//682//365//465 //1256//1398//1219//1256//1398 for recifiedii.jpg//465//678
#define LEFT2 682
#define LEFT3 991
#define LEFT1 365
#define RIGHT 165//217 //723//831//721//712//831 for recitifed....//217
using namespace cv;
using namespace std;
int main(int argc, const char * argv[]) {
    
    //VideoCapture cap;
   /* VideoCapture cap("v4l2src ! video/x-raw, framerate=30/1, width=640, height=480, format=RGB ! videoconvert ! appsink");
    
    if(!cap.isOpened()){
        return 1;
    }
    
    while(1){
        Mat image1;
        cap.read(image1);
        imshow("image1",image1);
        if(waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
        
    }*/
    // insert code here...
    Mat image1 = imread("sample4_result.jpg");//RIGHT
    Mat image2 = imread("sample3_result.jpg");//LEFT
    Mat image3 = imread("sample1_result.jpg");
    Mat image4 = imread("sample2_result.jpg");
    //imshow("result_test1.jpg",image1);
    //imshow("result_test2.jpg",image2);
    
    //double largest1 = 1340;//1742.353027//198.304810
   // double largest2 = 600;//905.512085//13.629153
    printf("the width of image 1 is %d\n", image1.cols);
    printf("the width of image 2 is %d\n", image2.cols);
    int width = LEFT + MIDDLEWIDTH + image1.cols - RIGHT;
    
    printf("the width of the final image is %d\n",width);
    Mat combine(max(image1.size().height,image2.size().height),width,CV_8UC3,Scalar(0,0,0));
    Mat result(FINALHEIGHT,FINALWIDTH,CV_8UC3,Scalar(0,0,0));
   // printf("width of combine = %d\n",combine.cols);
   // printf("height of combine = %d\n",combine.rows);
    
    //Mat left_roi(combine,Rect(0,0,LEFT,image2.size().height));
    Mat left_roi(result,Rect(0,0,LEFT1,image4.size().height));
    Size middleSize(MIDDLEWIDTH,image2.size().height);//added
    Mat middleZone(middleSize,CV_8UC3,Scalar(0,0,0));//added
    Mat croppedImage1;
    
   // Rect Roi1(0,0,LEFT,image2.rows);
    Rect Roi1(0,0,LEFT1,image2.rows);
   // croppedImage1 = image2(Roi1);
     croppedImage1 = image4(Roi1);
    croppedImage1.copyTo(left_roi);
    //printf("largest1 = %d, %d,%d\n",(int)largest1,(int)(image1.cols-largest2),image1.rows);
   // Mat right_roi(combine,Rect(LEFT+MIDDLEWIDTH,0,image1.cols - RIGHT,image1.rows));
    Mat second_roi(result, Rect(LEFT1 + MIDDLEWIDTH,0,image3.cols - RIGHT, image3.rows));
    //Mat right_roi(combine,Rect(largest1,0,image1.cols-largest2,image1.rows));
    Mat croppedImage2;
    //Rect Roi2(RIGHT,0,image1.cols-RIGHT,image1.rows);
    Rect Roi2(RIGHT,0,image3.cols-RIGHT,image3.rows);
    //croppedImage2 = image1(Roi2);
    croppedImage2 = image3(Roi2);
   // croppedImage2.copyTo(right_roi);
    croppedImage2.copyTo(second_roi);
    //imshow("combine",combine);
    //imwrite("combined.jpg",combine);
    
    //Mat_<Vec3b> _orgImg1 = image2;
   // Mat_<Vec3b> _orgImg2 = image1;
    Mat_<Vec3b> _orgImg1 = image4;
    Mat_<Vec3b> _orgImg2 = image3;
    //Mat_<Vec3b> _retImg = combine;
    Mat_<Vec3b> _retImg = result;
    //vector<Mat> channels;
    //split(combine,channels);
    for(int j = 0;j < result.rows;j++){
        for(int i = 0; i < MIDDLEWIDTH;i++){
            //int pointOnLeft = i + LEFT;
            int pointOnLeft = i + LEFT1;
            int pointOnRight = RIGHT - MIDDLEWIDTH + i;
           int RedOne = _orgImg1.at<Vec3b>(j,pointOnLeft)[0];
            int GreenOne = _orgImg1.at<Vec3b>(j,pointOnLeft)[1];
            int YellowOne = _orgImg1.at<Vec3b>(j,pointOnLeft)[2];
            int RedTwo = _orgImg2.at<Vec3b>(j,pointOnRight)[0];
            int GreenTwo = _orgImg2.at<Vec3b>(j,pointOnRight)[1];
            int YellowTwo = _orgImg2.at<Vec3b>(j,pointOnRight)[2];
            double newRed = (((LEFT1+MIDDLEWIDTH) - pointOnLeft)/(MIDDLEWIDTH*1.0)) * RedOne + ((pointOnLeft- LEFT1)/(MIDDLEWIDTH*1.0)) * RedTwo;
            double newGreen = ((LEFT1+MIDDLEWIDTH-pointOnLeft)/(MIDDLEWIDTH*1.0)) * GreenOne + ((pointOnLeft- LEFT1)/(MIDDLEWIDTH*1.0)) * GreenTwo;
            double newYellow = ((LEFT1+MIDDLEWIDTH-pointOnLeft)/(MIDDLEWIDTH*1.0)) * YellowOne + ((pointOnLeft- LEFT1)/(MIDDLEWIDTH*1.0)) * YellowTwo;
            /*double newRed = (((LEFT+MIDDLEWIDTH) - pointOnLeft)/(MIDDLEWIDTH*1.0)) * RedOne + ((pointOnLeft- LEFT)/(MIDDLEWIDTH*1.0)) * RedTwo;
             double newGreen = ((LEFT+MIDDLEWIDTH-pointOnLeft)/(MIDDLEWIDTH*1.0)) * GreenOne + ((pointOnLeft- LEFT1)/(MIDDLEWIDTH*1.0)) * GreenTwo;
             double newYellow = ((LEFT+MIDDLEWIDTH-pointOnLeft)/(MIDDLEWIDTH*1.0)) * YellowOne + ((pointOnLeft- LEFT)/(MIDDLEWIDTH*1.0)) * YellowTwo;*/
            _retImg.at<Vec3b>(j,pointOnLeft)[0] = (int)newRed;
            _retImg.at<Vec3b>(j,pointOnLeft)[1] = (int)newGreen;
            _retImg.at<Vec3b>(j,pointOnLeft)[2] = (int)newYellow;
           
            
        }
    }
    Mat third_roi(result,Rect(LEFT2+MIDDLEWIDTH,0,image2.cols - RIGHT, image2.rows));
    Mat croppedImage3 = image2(Roi2);
    croppedImage3.copyTo(third_roi);
    _orgImg1 = result;
    _orgImg2 = image2;
    _retImg = result;
    for(int j = 0;j < result.rows;j++){
        for(int i = 0; i < MIDDLEWIDTH;i++){
            //int pointOnLeft = i + LEFT;
            int pointOnLeft = i + LEFT2;
            int pointOnRight = RIGHT - MIDDLEWIDTH + i;
            int RedOne = _orgImg1.at<Vec3b>(j,pointOnLeft)[0];
            int GreenOne = _orgImg1.at<Vec3b>(j,pointOnLeft)[1];
            int YellowOne = _orgImg1.at<Vec3b>(j,pointOnLeft)[2];
            int RedTwo = _orgImg2.at<Vec3b>(j,pointOnRight)[0];
            int GreenTwo = _orgImg2.at<Vec3b>(j,pointOnRight)[1];
            int YellowTwo = _orgImg2.at<Vec3b>(j,pointOnRight)[2];
            double newRed = (((LEFT2+MIDDLEWIDTH) - pointOnLeft)/(MIDDLEWIDTH*1.0)) * RedOne + ((pointOnLeft- LEFT2)/(MIDDLEWIDTH*1.0)) * RedTwo;
            double newGreen = ((LEFT2+MIDDLEWIDTH-pointOnLeft)/(MIDDLEWIDTH*1.0)) * GreenOne + ((pointOnLeft- LEFT2)/(MIDDLEWIDTH*1.0)) * GreenTwo;
            double newYellow = ((LEFT2+MIDDLEWIDTH-pointOnLeft)/(MIDDLEWIDTH*1.0)) * YellowOne + ((pointOnLeft- LEFT2)/(MIDDLEWIDTH*1.0)) * YellowTwo;
            _retImg.at<Vec3b>(j,pointOnLeft)[0] = (int)newRed;
            _retImg.at<Vec3b>(j,pointOnLeft)[1] = (int)newGreen;
            _retImg.at<Vec3b>(j,pointOnLeft)[2] = (int)newYellow;
            
            
        }
    }
    Mat fourth_roi(result,Rect(LEFT3+MIDDLEWIDTH,0,image1.cols - RIGHT, image1.rows));
    Mat croppedImage4 = image1(Roi2);
    croppedImage4.copyTo(fourth_roi);
    _orgImg1 = result;
    _orgImg2 = image1;
    _retImg = result;
    for(int j = 0;j < result.rows;j++){
        for(int i = 0; i < MIDDLEWIDTH;i++){
            //int pointOnLeft = i + LEFT;
            int pointOnLeft = i + LEFT3;
            int pointOnRight = RIGHT - MIDDLEWIDTH + i;
            int RedOne = _orgImg1.at<Vec3b>(j,pointOnLeft)[0];
            int GreenOne = _orgImg1.at<Vec3b>(j,pointOnLeft)[1];
            int YellowOne = _orgImg1.at<Vec3b>(j,pointOnLeft)[2];
            int RedTwo = _orgImg2.at<Vec3b>(j,pointOnRight)[0];
            int GreenTwo = _orgImg2.at<Vec3b>(j,pointOnRight)[1];
            int YellowTwo = _orgImg2.at<Vec3b>(j,pointOnRight)[2];
            double newRed = (((LEFT3+MIDDLEWIDTH) - pointOnLeft)/(MIDDLEWIDTH*1.0)) * RedOne + ((pointOnLeft- LEFT3)/(MIDDLEWIDTH*1.0)) * RedTwo;
            double newGreen = ((LEFT3+MIDDLEWIDTH-pointOnLeft)/(MIDDLEWIDTH*1.0)) * GreenOne + ((pointOnLeft- LEFT3)/(MIDDLEWIDTH*1.0)) * GreenTwo;
            double newYellow = ((LEFT3+MIDDLEWIDTH-pointOnLeft)/(MIDDLEWIDTH*1.0)) * YellowOne + ((pointOnLeft- LEFT3)/(MIDDLEWIDTH*1.0)) * YellowTwo;
            _retImg.at<Vec3b>(j,pointOnLeft)[0] = (int)newRed;
            _retImg.at<Vec3b>(j,pointOnLeft)[1] = (int)newGreen;
            _retImg.at<Vec3b>(j,pointOnLeft)[2] = (int)newYellow;
            
            
        }
    }

    imwrite("samples.jpg",result);
    
   /* Mat result;
    float homography[3][3] = {{0.706282,-0.540353,258.458391},{0.317655,0.619805,39.477261},{0.000126,-0.000737,1.00000}};
    Mat H(3,3,CV_32FC1,&homography);
    warpPerspective(image1,result,H,Size(image1.cols+image2.cols,image1.rows));
    Mat half(result,Rect(0,0,image2.cols,image2.rows));
    image2.copyTo(half);
    string result_name = "result.jpg";
    imshow( "Result", result );
    imwrite(result_name, result);*/
    //waitKey(0);

    //std::cout << "Hello, World!\n";*/
    
    
    return 0;
}
