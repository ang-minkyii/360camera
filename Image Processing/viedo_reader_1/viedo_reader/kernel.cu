
#include <iostream>  
#include <string>  
#include <vector>  
#include <algorithm>  
#include <numeric>  
#include <opencv2/core/core.hpp>  
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/core/opengl_interop.hpp> 
#include <opencv2/core/cuda_devptrs.hpp>
#include <opencv2/gpu/gpu.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/contrib/contrib.hpp> 
#include <opencv2/gpu/stream_accessor.hpp>
#include <opencv2/gpu/gpumat.hpp>
#include <time.h>
#include "parameters.h"
#define WIDTH2 1920
#define HEIGHT2 1920
#define FINALWIDTH 3874
#define FINALHEIGHT 1920
#define LEFT1 1261
#define LEFT2 1909
#define LEFT3 2564
#define RIGHT 660
#define MIDDLEWIDTH 50

using namespace cv;
using namespace cv::gpu;
#include<iostream>  
using namespace std;
__global__ void stitch_kernel(const PtrStepSz<uchar3> src1, const PtrStepSz<uchar3> src2, PtrStep<uchar3> dst, int limit, int left)
{
	int abs_x = threadIdx.x + blockDim.x * blockIdx.x;
	int abs_y = threadIdx.y + blockDim.y * blockIdx.y;
	if (abs_y < 0 || abs_y >= limit || abs_x < 0 || abs_x >= MIDDLEWIDTH) {
		return;

	}
	int pointOnLeft = abs_x + left;
	int pointOnRight = RIGHT - MIDDLEWIDTH + abs_x;
	uchar3 value1 = src1(abs_y, pointOnLeft);
	uchar3 value2 = src2(abs_y, pointOnRight);
	unsigned char newRed = (((left + MIDDLEWIDTH) - pointOnLeft) / (MIDDLEWIDTH*1.0)) * value1.x + ((pointOnLeft - left) / (MIDDLEWIDTH*1.0)) * value2.x;
	unsigned char newGreen = (((left + MIDDLEWIDTH) - pointOnLeft) / (MIDDLEWIDTH*1.0)) * value1.y + ((pointOnLeft - left) / (MIDDLEWIDTH*1.0)) * value2.y;
	unsigned char newBlue = (((left + MIDDLEWIDTH) - pointOnLeft) / (MIDDLEWIDTH*1.0)) * value1.z + ((pointOnLeft - left) / (MIDDLEWIDTH*1.0)) * value2.z;
	dst(abs_y, pointOnLeft) = make_uchar3(newRed, newGreen, newBlue);

}

void stitch_caller(const PtrStepSz<uchar3>& src1, const PtrStepSz<uchar3>& src2, PtrStep<uchar3> dst, int limit, int left, cudaStream_t stream)
{
	int blockWidth = 32;
	int blockHeight = 8;
	dim3 blockSize(blockWidth, blockHeight);
	int blocksY = src1.rows / blockHeight;
	int blocksX = src1.cols / blockWidth;
	const dim3 gridSize(blocksX, blocksY);
	stitch_kernel << <gridSize, blockSize, 0, stream >> >(src1, src2, dst, limit, left);
	if (stream == 0)
	{
		cudaDeviceSynchronize();
	}

}
void stitch(const GpuMat& src1, const GpuMat& src2, GpuMat& dst, int limit, int left, Stream& stream = Stream::Null())
{
	CV_Assert(src1.type() == CV_8UC3);
	CV_Assert(src2.type() == CV_8UC3);
	cudaStream_t s = StreamAccessor::getStream(stream);
	stitch_caller(src1, src2, dst, limit, left, s);
}
int main(int argc, char* argv[])
{

	const string fname = "D:\\Documents\\Movies\\out1.avi";
	const string fname2 = "D:\\Documents\\Movies\\out2.avi";
	const string fname3 = "D:\\Documents\\Movies\\out3.avi";
	const string fname4 = "D:\\Documents\\Movies\\out4.avi";
	std::string fname5 = "D:\\Documents\\Movies\\Project\\attempt3\\1.avi";
	const string fname6 = "D:\\Documents\\Movies\\Project\\attempt3\\2.avi";
	const string fname7 = "D:\\Documents\\Movies\\Project\\attempt3\\3.avi";
	const string fname8 = "D:\\Documents\\Movies\\Project\\attempt3\\4.avi";
	time_t finish, start;
	//string mm = cv::getBuildInformation();
	//cout << mm << endl;
	//cv::namedWindow("CPU", cv::WINDOW_NORMAL);
	//cv::namedWindow("GPU", cv::WINDOW_OPENGL);
	//gpu::setGlDevice();
	Mat frame,frame2,frame3,frame4;
	Mat framecorrected(HEIGHT2, WIDTH2, CV_8UC3, Scalar(0, 0, 0));
	Mat framecorrected2(HEIGHT2, WIDTH2, CV_8UC3, Scalar(0, 0, 0));
	Mat framecorrected3(HEIGHT2, WIDTH2, CV_8UC3, Scalar(0, 0, 0));
	Mat framecorrected4(HEIGHT2, WIDTH2, CV_8UC3, Scalar(0, 0, 0));
	GpuMat frame_d, frame2_d, frame3_d, frame4_d;
	GpuMat framecorrected_d, framecorrected2_d, framecorrected3_d, framecorrected4_d;
	Mat result(1566, 640, CV_8UC3, Scalar(0, 0, 0));
	cv::VideoCapture reader(fname5);
	cv::VideoCapture reader2(fname6);
	cv::VideoCapture reader3(fname7);
	cv::VideoCapture reader4(fname8);
	CvSize frameSize = cvSize(int(reader.get(CV_CAP_PROP_FRAME_WIDTH)), int(reader.get(CV_CAP_PROP_FRAME_HEIGHT)));
	printf("height:%d\n", frameSize.height);
	printf("width:%d\n", frameSize.width);
	double count = reader.get(CV_CAP_PROP_FRAME_COUNT);
	printf("number of frames = %d\n", count);
	int n = 0;
	//reader.set(CV_CAP_PROP_POS_FRAMES, count - 1);
//	cv::gpu::GpuMat d_frame;
//	cv::gpu::VideoReader_GPU d_reader(fname);
	//d_reader.dumpFormat(std::cout);

	cv::TickMeter tm;
	std::vector<double> cpu_times;
//	std::vector<double> gpu_times;
	
	
	cv::Mat x_map2(HEIGHT2, WIDTH2, CV_32FC1, &_x_map_1920);
	cv::Mat y_map2(HEIGHT2, WIDTH2, CV_32FC1, &_y_map_1920);
	GpuMat x_map2_d, y_map2_d;
	x_map2_d.upload(x_map2);
	y_map2_d.upload(y_map2);
	for (;;)
	{
		tm.reset(); tm.start();
		if (!reader.read(frame) || !reader2.read(frame2) || !reader3.read(frame3) || !reader4.read(frame4))
			break;
		
		/*if (n == 0)
		{
			cv::imwrite("D:\\Documents\\Images\\attempt3_5.jpg",frame);
		}
		n++;*/
		tm.stop();
		cpu_times.push_back(tm.getTimeMilli());

	//	tm.reset(); tm.start();
		//if (!d_reader.read(d_frame))
			//break;
		//tm.stop();
		//gpu_times.push_back(tm.getTimeMilli());
		time(&start);
		frame_d.upload(frame);
		frame2_d.upload(frame2);
		frame3_d.upload(frame3);
		frame4_d.upload(frame4);
		remap(frame_d, framecorrected_d, x_map2_d, y_map2_d, INTER_LINEAR);
		remap(frame2_d, framecorrected2_d, x_map2_d, y_map2_d, INTER_LINEAR);
		remap(frame3_d, framecorrected3_d, x_map2_d, y_map2_d, INTER_LINEAR);
		remap(frame4_d, framecorrected4_d, x_map2_d, y_map2_d, INTER_LINEAR);
	//	int width = LEFT1 + MIDDLEWIDTH + framecorrected.cols - RIGHT;
		Mat result(FINALHEIGHT, FINALWIDTH, CV_8UC3, Scalar(0, 0, 0));
		//GpuMat combine(1920, width, CV_8UC3, Scalar(0, 0, 0));
		GpuMat result_d(FINALHEIGHT, FINALWIDTH, CV_8UC3, Scalar(0, 0, 0));
		GpuMat left_roi_d(result_d, Rect(0, 0, LEFT1, framecorrected_d.size().height));
		Size middleSize(MIDDLEWIDTH, framecorrected_d.size().height);
		GpuMat middle_zone_d(middleSize, CV_8UC3, Scalar(0, 0, 0));
		GpuMat croppedImage1;
		Rect Roi1(0, 0, LEFT1, framecorrected_d.size().height);
		croppedImage1 = framecorrected_d(Roi1);
		croppedImage1.copyTo(left_roi_d);
		GpuMat second_roi_d(result_d,Rect(LEFT1+MIDDLEWIDTH,0,framecorrected2_d.cols-RIGHT,framecorrected2_d.rows));
		GpuMat croppedImage2;
		Rect Roi2(RIGHT, 0, framecorrected2_d.cols - RIGHT, framecorrected2_d.rows);
		croppedImage2 = framecorrected2_d(Roi2);
		croppedImage2.copyTo(second_roi_d);
		stitch(framecorrected_d, framecorrected2_d, result_d, framecorrected_d.rows, LEFT1);

		GpuMat third_roi_d(result_d, Rect(LEFT2 + MIDDLEWIDTH, 0, framecorrected3_d.cols - RIGHT, framecorrected3_d.rows));
		GpuMat croppedImage3;
		croppedImage3 = framecorrected3_d(Roi2);
		croppedImage3.copyTo(third_roi_d);
		stitch(result_d, framecorrected3_d, result_d, result_d.rows, LEFT2);

		GpuMat fourth_roi_d(result_d, Rect(LEFT3 + MIDDLEWIDTH, 0, framecorrected4_d.cols - RIGHT, framecorrected4_d.rows));
		GpuMat croppedImage4 = framecorrected4_d(Roi2);
		croppedImage4.copyTo(fourth_roi_d);
		stitch(result_d, framecorrected4_d, result_d, result_d.rows, LEFT3);

		result_d.download(result);
		//framecorrected_d.download(framecorrected);
		//framecorrected2_d.download(framecorrected2);
		//framecorrected3_d.download(framecorrected3);
		//framecorrected4_d.download(framecorrected4);
		time(&finish);
		/*remap(frame, framecorrected, x_map2, y_map2, INTER_LINEAR);
		remap(frame2, framecorrected2, x_map2, y_map2, INTER_LINEAR);
		remap(frame3, framecorrected3, x_map2, y_map2, INTER_LINEAR);
		remap(frame4, framecorrected4, x_map2, y_map2, INTER_LINEAR);*/
		//namedWindow("CPU", WINDOW_NORMAL);
		//namedWindow("CPU2", WINDOW_NORMAL);
		//namedWindow("CPU3", WINDOW_NORMAL);
		//namedWindow("CPU4", WINDOW_NORMAL);
		//cv::imshow("CPU", framecorrected);
		//imshow("CPU2", framecorrected2);
		//imshow("CPU3", framecorrected3);
		//imshow("CPU4", framecorrected4);
		namedWindow("result", WINDOW_NORMAL);
		imshow("result", result);
		/**Mat left_roi(res, Rect(0, 0, LEFT1, src.size().height));
		Size middleSize(MIDDLEWIDTH, src.size().height);//added
		Mat middleZone(middleSize, CV_8UC3, Scalar(0, 0, 0));//added
		Mat croppedImage1;

		// Rect Roi1(0,0,LEFT,image2.rows);
		Rect Roi1(0, 0, LEFT1, src.rows);
		// croppedImage1 = image2(Roi1);
		croppedImage1 = src(Roi1);
		croppedImage1.copyTo(left_roi);
		//printf("largest1 = %d, %d,%d\n",(int)largest1,(int)(image1.cols-largest2),image1.rows);
		// Mat right_roi(combine,Rect(LEFT+MIDDLEWIDTH,0,image1.cols - RIGHT,image1.rows));
		Mat second_roi(res, Rect(LEFT1 + MIDDLEWIDTH, 0, src2.cols - RIGHT, src2.rows));
		//Mat right_roi(combine,Rect(largest1,0,image1.cols-largest2,image1.rows));
		Mat croppedImage2;
		//Rect Roi2(RIGHT,0,image1.cols-RIGHT,image1.rows);
		Rect Roi2(RIGHT, 0, src2.cols - RIGHT, src2.rows);
		//croppedImage2 = image1(Roi2);
		croppedImage2 = src2(Roi2);
		// croppedImage2.copyTo(right_roi);
		croppedImage2.copyTo(second_roi);
		//imshow("combine",combine);
		//imwrite("combined.jpg",combine);

		//Mat_<Vec3b> _orgImg1 = image2;
		// Mat_<Vec3b> _orgImg2 = image1;
		Mat_<Vec3b> _orgImg1 = src;
		Mat_<Vec3b> _orgImg2 = src2;
		//Mat_<Vec3b> _retImg = combine;
		Mat_<Vec3b> _retImg = res;
		//vector<Mat> channels;
		//split(combine,channels);
		for (int j = 0; j < res.rows; j++) {
			for (int i = 0; i < MIDDLEWIDTH; i++) {
				//int pointOnLeft = i + LEFT;
				int pointOnLeft = i + LEFT1;
				int pointOnRight = RIGHT - MIDDLEWIDTH + i;
				int RedOne = _orgImg1.at<Vec3b>(j, pointOnLeft)[0];
				int GreenOne = _orgImg1.at<Vec3b>(j, pointOnLeft)[1];
				int YellowOne = _orgImg1.at<Vec3b>(j, pointOnLeft)[2];
				int RedTwo = _orgImg2.at<Vec3b>(j, pointOnRight)[0];
				int GreenTwo = _orgImg2.at<Vec3b>(j, pointOnRight)[1];
				int YellowTwo = _orgImg2.at<Vec3b>(j, pointOnRight)[2];
				double newRed = (((LEFT1 + MIDDLEWIDTH) - pointOnLeft) / (MIDDLEWIDTH*1.0)) * RedOne + ((pointOnLeft - LEFT1) / (MIDDLEWIDTH*1.0)) * RedTwo;
				double newGreen = ((LEFT1 + MIDDLEWIDTH - pointOnLeft) / (MIDDLEWIDTH*1.0)) * GreenOne + ((pointOnLeft - LEFT1) / (MIDDLEWIDTH*1.0)) * GreenTwo;
				double newYellow = ((LEFT1 + MIDDLEWIDTH - pointOnLeft) / (MIDDLEWIDTH*1.0)) * YellowOne + ((pointOnLeft - LEFT1) / (MIDDLEWIDTH*1.0)) * YellowTwo;
			//	double newRed = (((LEFT+MIDDLEWIDTH) - pointOnLeft)/(MIDDLEWIDTH*1.0)) * RedOne + ((pointOnLeft- LEFT)/(MIDDLEWIDTH*1.0)) * RedTwo;
			//	double newGreen = ((LEFT+MIDDLEWIDTH-pointOnLeft)/(MIDDLEWIDTH*1.0)) * GreenOne + ((pointOnLeft- LEFT1)/(MIDDLEWIDTH*1.0)) * GreenTwo;
			//	double newYellow = ((LEFT+MIDDLEWIDTH-pointOnLeft)/(MIDDLEWIDTH*1.0)) * YellowOne + ((pointOnLeft- LEFT)/(MIDDLEWIDTH*1.0)) * YellowTwo;
				_retImg.at<Vec3b>(j, pointOnLeft)[0] = (int)newRed;
				_retImg.at<Vec3b>(j, pointOnLeft)[1] = (int)newGreen;
				_retImg.at<Vec3b>(j, pointOnLeft)[2] = (int)newYellow;


			}
		}
		Mat third_roi(res, Rect(LEFT2 + MIDDLEWIDTH, 0, src3.cols - RIGHT, src3.rows));
		Mat croppedImage3 = src3(Roi2);
		croppedImage3.copyTo(third_roi);
		_orgImg1 = res;
		_orgImg2 = src3;
		_retImg = res;
		for (int j = 0; j < res.rows; j++) {
			for (int i = 0; i < MIDDLEWIDTH; i++) {
				//int pointOnLeft = i + LEFT;
				int pointOnLeft = i + LEFT2;
				int pointOnRight = RIGHT - MIDDLEWIDTH + i;
				int RedOne = _orgImg1.at<Vec3b>(j, pointOnLeft)[0];
				int GreenOne = _orgImg1.at<Vec3b>(j, pointOnLeft)[1];
				int YellowOne = _orgImg1.at<Vec3b>(j, pointOnLeft)[2];
				int RedTwo = _orgImg2.at<Vec3b>(j, pointOnRight)[0];
				int GreenTwo = _orgImg2.at<Vec3b>(j, pointOnRight)[1];
				int YellowTwo = _orgImg2.at<Vec3b>(j, pointOnRight)[2];
				double newRed = (((LEFT2 + MIDDLEWIDTH) - pointOnLeft) / (MIDDLEWIDTH*1.0)) * RedOne + ((pointOnLeft - LEFT2) / (MIDDLEWIDTH*1.0)) * RedTwo;
				double newGreen = ((LEFT2 + MIDDLEWIDTH - pointOnLeft) / (MIDDLEWIDTH*1.0)) * GreenOne + ((pointOnLeft - LEFT2) / (MIDDLEWIDTH*1.0)) * GreenTwo;
				double newYellow = ((LEFT2 + MIDDLEWIDTH - pointOnLeft) / (MIDDLEWIDTH*1.0)) * YellowOne + ((pointOnLeft - LEFT2) / (MIDDLEWIDTH*1.0)) * YellowTwo;
				_retImg.at<Vec3b>(j, pointOnLeft)[0] = (int)newRed;
				_retImg.at<Vec3b>(j, pointOnLeft)[1] = (int)newGreen;
				_retImg.at<Vec3b>(j, pointOnLeft)[2] = (int)newYellow;


			}
		}
		Mat fourth_roi(res, Rect(LEFT3 + MIDDLEWIDTH, 0, src4.cols - RIGHT, src4.rows));
		Mat croppedImage4 = src4(Roi2);
		croppedImage4.copyTo(fourth_roi);
		_orgImg1 = res;
		_orgImg2 = src4;
		_retImg = res;
		for (int j = 0; j < res.rows; j++) {
			for (int i = 0; i < MIDDLEWIDTH; i++) {
				//int pointOnLeft = i + LEFT;
				int pointOnLeft = i + LEFT3;
				int pointOnRight = RIGHT - MIDDLEWIDTH + i;
				int RedOne = _orgImg1.at<Vec3b>(j, pointOnLeft)[0];
				int GreenOne = _orgImg1.at<Vec3b>(j, pointOnLeft)[1];
				int YellowOne = _orgImg1.at<Vec3b>(j, pointOnLeft)[2];
				int RedTwo = _orgImg2.at<Vec3b>(j, pointOnRight)[0];
				int GreenTwo = _orgImg2.at<Vec3b>(j, pointOnRight)[1];
				int YellowTwo = _orgImg2.at<Vec3b>(j, pointOnRight)[2];
				double newRed = (((LEFT3 + MIDDLEWIDTH) - pointOnLeft) / (MIDDLEWIDTH*1.0)) * RedOne + ((pointOnLeft - LEFT3) / (MIDDLEWIDTH*1.0)) * RedTwo;
				double newGreen = ((LEFT3 + MIDDLEWIDTH - pointOnLeft) / (MIDDLEWIDTH*1.0)) * GreenOne + ((pointOnLeft - LEFT3) / (MIDDLEWIDTH*1.0)) * GreenTwo;
				double newYellow = ((LEFT3 + MIDDLEWIDTH - pointOnLeft) / (MIDDLEWIDTH*1.0)) * YellowOne + ((pointOnLeft - LEFT3) / (MIDDLEWIDTH*1.0)) * YellowTwo;
				_retImg.at<Vec3b>(j, pointOnLeft)[0] = (int)newRed;
				_retImg.at<Vec3b>(j, pointOnLeft)[1] = (int)newGreen;
				_retImg.at<Vec3b>(j, pointOnLeft)[2] = (int)newYellow;


			}
		}*/
		//cv::imshow("GPU", d_frame);

		if (cv::waitKey(3) > 0)
			break;
	}

	if (!cpu_times.empty() )//&& !gpu_times.empty())
	{
		std::cout << std::endl << "Results:" << std::endl;

		std::sort(cpu_times.begin(), cpu_times.end());
//		std::sort(gpu_times.begin(), gpu_times.end());

		double cpu_avg = std::accumulate(cpu_times.begin(), cpu_times.end(), 0.0) / cpu_times.size();
	//	double gpu_avg = std::accumulate(gpu_times.begin(), gpu_times.end(), 0.0) / gpu_times.size();

		std::cout << "CPU : Avg : " << cpu_avg << " ms FPS : " << 1000.0 / cpu_avg << std::endl;
	//std::cout << "GPU : Avg : " << gpu_avg << " ms FPS : " << 1000.0 / gpu_avg << std::endl;
	}
	printf("difference in time = %d\n", difftime(finish,start));
	cvDestroyAllWindows();
	system("pause");
	return 0;
}