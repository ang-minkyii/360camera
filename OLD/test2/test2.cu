//#include <iostream>
#include <numeric>
#include <stdlib.h>
//#include "utils.h"
//#include <string>
#include <stdio.h>

//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/opencv.hpp>

//using namespace cv;
#include "functions.hpp"
#define PI 3.1415926
#define RADIUS 770
#define centreX 776
#define centreY 540
double cameraFieldAngle = PI;
double latitudeOffset = (PI - cameraFieldAngle)/2;
double longitudeOffset = (PI - cameraFieldAngle)/2;

__global__
void correction(const uchar4* const inputImage,
				uchar4* const rectImage,
				int numRows, int numCols,double dx, double dy,double latitudeOffset,double longitudeOffset)
{
	int j = threadIdx.y + blockIdx.y * blockDim.y;
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	double x,y,z,r;
	double latitude,longitude;
	double u,v;
	double u_latitude,v_latitude;
	double theta_sphere;
	double phi_shpere;
	double theta;
	double x_cart;
	double y_cart;
	double p;
	latitude = latitudeOffset + j * dy;
	longitude = latitudeOffset + i * dx;

	x = -sin(latitude) * cos(longitude);
	y = cos(latitude);
	z = sin(latitude) * sin(longitude);

	theta_sphere = acos(z);
	phi_shpere = atan2(y,x);
	//phi_shpere = phi_shpere * PI/180;
	p = sin(theta_sphere);
	theta = phi_shpere;

	x_cart = p*cos(theta);
	y_cart = p*sin(theta);
	double R = RADIUS;
	u = x_cart *R + centreX;
	v = -y_cart *R + centreY;





	if(j < numCols && i < numRows  && v < numRows && u < numCols){
		int newIndex = numRows * v + u;
		int index = numRows * j + i;
		uchar4 value = inputImage[index];
		rectImage[newIndex] = value;
	}


}
/*
 *
 * Original CPU Code of correction
 *
 * Mat correct(Mat imageInput){
    double camerFieldAngle = PI;

    double rateOfWindow = 1;

    //int width = imageInput.size().width*rateOfWindow;
    int width = imageInput.size().width * rateOfWindow;
    int height = width;
    Size imgSize(width,height);
    Mat retImg(imgSize,CV_8UC3,Scalar(0,0,0));
    double dx = camerFieldAngle/imgSize.width;
    double dy = dx;

    double latitude;
    double longitude;

    double x,y,z,r;
    double Theta_sphere;
    double Phi_sphere;
    double p;
    double theta;
    double x_cart;
    double y_cart;
    int u,v;
    int u_latitude, v_latitude;

    double longitude_offset, latitude_offset;

    longitude_offset = (PI-camerFieldAngle)/2;
    latitude_offset = (PI - camerFieldAngle)/2;

    Mat_<Vec3b> _retImg = retImg;
    Mat_<Vec3b> _imageInput = imageInput;

    for(int j = 0; j < imgSize.height;j++){
        latitude = latitude_offset + j * dy;
        for(int i = 0; i < imgSize.width;i++){
            longitude = latitude_offset + i * dx;
            x = -sin(latitude) * cos(longitude);
            y = cos(latitude);
            z = sin(latitude) * sin(longitude);

            Theta_sphere = acos(z);
            Phi_sphere = cvFastArctan(y, x);
            Phi_sphere = Phi_sphere * PI /180;
            p = sin(Theta_sphere);
            theta = Phi_sphere;

            x_cart = p * cos(theta);
            y_cart = p * sin(theta);
            double R = RADIUS;
            u = x_cart * R + centreX;
            v = -y_cart * R + centreY;
            _retImg.at<Vec3b>(j,i) = _imageInput.at<Vec3b>(v,u);
        }
    }
    return retImg;
}
 * */
void OverallCorrection(const uchar4* const h_inputImage,
		uchar4* const d_inputImage,uchar4* const d_rectImage,
		size_t numRows, size_t numCols){
	int blockWidth = 32;
	const dim3 blockSize(blockWidth, blockWidth, 1);
	int blocksX = numRows/blockWidth + 1;
	int blocksY = numCols/blockWidth + 1;
	const dim3 gridSize(blocksX, blocksY, 1);
	double dx = cameraFieldAngle/numCols;
	double dy = cameraFieldAngle/numRows;
	correction<<<gridSize,blockSize>>>(d_inputImage,d_rectImage,numRows, numCols,dx,dy,latitudeOffset, longitudeOffset);
	cudaDeviceSynchronize();
	checkCudaErrors(cudaGetLastError());
}
int main(void)
{
	//Mat img(512,512,CV_8UC3,Scalar(0));
	//putText(img,"Hello",Point(10,img.rows/2),FONT_HERSHEY_DUPLEX,1.0,CV_RGB(118,185,0),2);
	//imshow("hello",img);
	//Mat img2;
	//img2 = imread("my_photo-1.jpg");
	//imshow("image2",img2);
	//imwrite("result.jpg",img);
	//waitKey();
	Mat img1 = imread("capture.jpg");
	imwrite("cpimg1.jpg",img1);
	uchar4 *h_inputImage, *d_inputImage;
	uchar4 *h_rectImage, *d_rectImage;

	std::string input_file;
	std::string output_file;
	input_file = "capture.jpg";
	output_file = "Output.jpg";

	preProcess(&h_inputImage, &h_rectImage, &d_inputImage, &d_rectImage,input_file);
	OverallCorrection(h_inputImage,d_inputImage,d_rectImage,height(),width());
	cudaDeviceSynchronize();
	checkCudaErrors(cudaGetLastError());

	size_t numPixels = height() * width();
	checkCudaErrors(cudaMemcpy(h_rectImage,d_rectImage,sizeof(uchar4)* numPixels,cudaMemcpyDeviceToHost));

	postProcess(output_file,h_rectImage);
	cleanUp();
	return 0;
}

