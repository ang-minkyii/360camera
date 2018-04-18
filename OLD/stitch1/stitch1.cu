/*
 ============================================================================
 Name        : stitching.cu
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : CUDA compute reciprocals
 ============================================================================
 */

#include <iostream>
//#include <numeric>
#include <stdlib.h>
#include <stdio.h>
//#include <opencv2/flann/flann.hpp>
//#include <opencv2/flann/all_indices.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/gpu/gpumat.hpp>
using namespace cv;
int main(){
	float homography[3][3] = {{0.706282,-0.540353,258.458391},{0.317655,0.619805,39.477261},{0.000126,-0.000737,1.00000}};
	Mat H(3,3,CV_32FC1,&homography);
	Mat image1 = imread("result_test1.jpg");
	Mat image2 = imread("result_test2.jpg");
	Size imgSize(image1.cols + image2.cols,image1.rows);
	Mat dst(imgSize,CV_8UC3,Scalar(0,0,0));
	gpu::GpuMat image1_gpu, image2_gpu, result;
	image1_gpu.upload(image1);
	image2_gpu.upload(image2);

	gpu::warpPerspective(image1_gpu,result,H,Size(image1.cols + image2.cols, image1.rows));
	gpu::GpuMat half(result,Rect(0,0,image2_gpu.cols,image2_gpu.rows));
	image2_gpu.copyTo(half);
	result.download(dst);
	imwrite("result.jpg",dst);
	//printf("%f\n",H.at<float>(1,0));
	return 0;
}
