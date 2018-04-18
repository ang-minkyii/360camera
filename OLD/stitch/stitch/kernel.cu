#include <opencv2/core/cuda_devptrs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/gpu/stream_accessor.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/gpu/devmem2d.hpp>
#include <opencv2/gpu/gpumat.hpp>


#define MIDDLEWIDTH 100
#define LEFT 1256
#define RIGHT 723
#define LARGEST1 1340
#define LARGEST2 600
using namespace cv;
using namespace cv::gpu;
using namespace std;
__global__ void stitch_kernel(const PtrStepSz<uchar3> src1, const PtrStepSz<uchar3> src2, PtrStep<uchar3> dst, int limit)
{
	int abs_x = threadIdx.x + blockDim.x * blockIdx.x;
	int abs_y = threadIdx.y + blockDim.y * blockIdx.y;
	if (abs_y < 0 || abs_y >= limit || abs_x < 0 || abs_x >= MIDDLEWIDTH) {
		return;

	}
	int pointOnLeft = abs_x + LEFT;
	int pointOnRight = RIGHT - MIDDLEWIDTH + abs_x;
	uchar3 value1 = src1(abs_y, pointOnLeft);
	uchar3 value2 = src2(abs_y, pointOnRight);
	unsigned char newRed = (((LEFT + MIDDLEWIDTH) - pointOnLeft) / (MIDDLEWIDTH*1.0)) * value1.x + ((pointOnLeft - LEFT) / (MIDDLEWIDTH*1.0)) * value2.x;
	unsigned char newGreen = (((LEFT + MIDDLEWIDTH) - pointOnLeft) / (MIDDLEWIDTH*1.0)) * value1.y + ((pointOnLeft - LEFT) / (MIDDLEWIDTH*1.0)) * value2.y;
	unsigned char newBlue = (((LEFT + MIDDLEWIDTH) - pointOnLeft) / (MIDDLEWIDTH*1.0)) * value1.z + ((pointOnLeft - LEFT) / (MIDDLEWIDTH*1.0)) * value2.z;
	dst(abs_y, pointOnLeft) = make_uchar3(newRed, newGreen, newBlue);

}

void stitch_caller(const PtrStepSz<uchar3>& src1, const PtrStepSz<uchar3>& src2, PtrStep<uchar3> dst, int limit, cudaStream_t stream)
{
	int blockWidth = 32;
	int blockHeight = 8;
	dim3 blockSize(blockWidth, blockHeight);
	int blocksY = src1.rows / blockHeight;
	int blocksX = src1.cols / blockWidth;
	const dim3 gridSize(blocksX, blocksY);
	stitch_kernel << <gridSize, blockSize, 0, stream >> >(src1, src2, dst, limit);
	if (stream == 0)
	{
		cudaDeviceSynchronize();
	}
	
}
void stitch(const GpuMat& src1, const GpuMat& src2, GpuMat& dst, int limit, Stream& stream = Stream::Null())
{
	CV_Assert(src1.type() == CV_8UC3);
	CV_Assert(src2.type() == CV_8UC3);
	cudaStream_t s = StreamAccessor::getStream(stream);
	stitch_caller(src1, src2, dst, limit, s);
}
int main()
{	
	Mat imageRight = imread("D:\\Documents\\Images\\resulti_phi.jpg");
	Mat imageLeft = imread("D:\\Documents\\Images\\resultii_phi.jpg");
	int width = LEFT + MIDDLEWIDTH + imageRight.cols - RIGHT;
	Mat combine(max(imageRight.rows, imageLeft.rows), width, CV_8UC3, Scalar(0, 0, 0));
	GpuMat gpuMatRight, gpuMatLeft, output;
	gpuMatRight.upload(imageRight);
	gpuMatLeft.upload(imageLeft);
	output.upload(combine);
	GpuMat left_roi(output, Rect(0, 0, LEFT, gpuMatLeft.size().height));
	Size middleSize(MIDDLEWIDTH, gpuMatLeft.size().height);
	GpuMat middleZone(middleSize, CV_8UC3, Scalar(0, 0, 0));
	GpuMat croppedImage1;
	Rect roi1(0, 0, LEFT, imageLeft.rows);
	croppedImage1 = gpuMatLeft(roi1);
	croppedImage1.copyTo(left_roi);
	GpuMat right_roi(output, Rect(LEFT + MIDDLEWIDTH, 0, imageRight.cols - RIGHT, imageRight.rows));
	GpuMat croppedImage2;
	Rect roi2(RIGHT, 0, imageRight.cols - RIGHT, imageRight.rows);
	croppedImage2 = gpuMatRight(roi2);
	croppedImage2.copyTo(right_roi);
	stitch(gpuMatLeft, gpuMatRight, output, gpuMatLeft.rows);
	output.download(combine);
	imwrite("D:\\Documents\\Images\\temp.jpg", combine);

	return 0;
}