 
#include <opencv2/core/cuda_devptrs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/gpu/stream_accessor.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#define RADIUS 340
#define centreX 320
#define centreY 240
#define PI 3.1415926
#define cameraFieldAngle PI
#define LIMIT 1e-4
using namespace cv;
using namespace cv::gpu;
using namespace std;
//自定义内核函数
//void swap_rb_caller(const PtrStepSz<uchar3>& src,PtrStep<uchar3> dst,cudaStream_t stream);
__device__ double func(double l, double phi)
{
	double result = (l / 2 - 1)*cos(phi) - sin(phi) + 1;
	return result;
}
__device__ double getPhi(double l)
{
	double head = 0;
	double tail = 0;
	double mid = 0;
	double result = 0;
	if (l >= 0 && l < 2)
	{
		head = 0;
		tail = PI / 2;
		mid = head;
		result = func(l, mid);
		while (abs(result)>LIMIT)
		{
			mid = (tail + head) / 2;
			result = func(l, mid);

			if (result > 0)
			{
				head = mid;
			}
			else
			{
				tail = mid;
			}
		}
	}
	else
	{
		head = PI / 2;
		tail = PI;
		mid = tail;
		result = func(l, mid);
		while (abs(result) > LIMIT)
		{
			mid = (tail + head) / 2;
			result = func(l, mid);
			if (result < 0)
			{
				head = mid;
			}
			else
			{
				tail = mid;
			}
		}
	}
	return mid;
}
__global__ void correction_kernel2(const PtrStepSz<uchar3> src, PtrStep<uchar3> dst)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int j = threadIdx.y + blockIdx.y * blockDim.y;
	/*double dx = cameraFieldAngle / src.cols;
	double dy = cameraFieldAngle / src.rows;

	double latitude = getPhi((double)j * 4.0 / src.rows);
	double longitude = getPhi((double)i * 4.0 / src.cols);*/
	double dx = cameraFieldAngle / src.cols;
	double dy = dx;
	double latitude = (PI - cameraFieldAngle) / 2 + j * dy;
	double longitude = (PI - cameraFieldAngle) / 2 + i * dx;
	double x = -sin(latitude)*cos(longitude);
	double y = cos(latitude);
	double z = sin(latitude) * sin(longitude);
	double theta_shpere = acos(z);
	double phi_shpere = atan2(y, x);
	double foval = RADIUS / (cameraFieldAngle / 2);
	double p = foval * theta_shpere;
	double theta = phi_shpere;
	double x_cart = p * cos(theta);
	double y_cart = p * sin(theta);
	int u = x_cart + centreX;
	int v = -y_cart + centreY;
	if (i < src.cols && j < src.rows && u < src.cols && v < src.cols)
	{
		uchar3 value = src(v, u);
		dst(j, i) = make_uchar3(value.x, value.y, value.z);
	}
}
__global__ void correction_kernel(const PtrStepSz<uchar3> src, PtrStep<uchar3> dst)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int j = threadIdx.y + blockIdx.y * blockDim.y;
	double dx = cameraFieldAngle / src.cols;
	double dy = dx;
	double latitude = (PI - cameraFieldAngle) / 2 + j * dy;
	double longitude = (PI - cameraFieldAngle) / 2 + i * dx;
	double x = -sin(latitude) * cos(longitude);
	double y = cos(latitude);
	double z = sin(latitude) * sin(longitude);

	double theta_shpere = acos(z);
	double phi_shpere = atan2(y, x);
	double p = sin(theta_shpere);
	double theta = phi_shpere;
	double x_cart = p * cos(theta);
	double y_cart = p * sin(theta);
	int u = x_cart * RADIUS + centreX;
	int v = -y_cart * RADIUS + centreY;
	if (i < src.cols && j < src.rows && u < src.cols && v < src.rows)
	{
		uchar3 value = src(v, u);
		dst(j, i) = make_uchar3(value.x, value.y, value.z);
	}
}

void correction_caller(const PtrStepSz<uchar3>& src, PtrStep<uchar3> dst, cudaStream_t stream)
{
	/*dim3 block(32,8);

	dim3 grid((src.cols + block.x - 1) / block.x, (src.rows + block.y - 1) / block.y);

	correction_kernel <<<grid, block, 0, stream >>>(src, dst);
	if (stream == 0)
		cudaDeviceSynchronize();
		*/
	int blockWidth = 16;
	int blockHeight = 8;
	dim3 blockSize(blockWidth, blockHeight);
	int   blocksY = src.rows / blockHeight + 1;
	int   blocksX = src.cols / blockWidth + 1; //TODO
	const dim3 gridSize(blocksX, blocksY);  //TODO
	correction_kernel2 <<<gridSize, blockSize, 0, stream >>>(src, dst);
	if (stream == 0)
	{
		cudaDeviceSynchronize();
	}
}
void correction(const GpuMat& src, GpuMat& dst, Stream& stream = Stream::Null())
{
	CV_Assert(src.type() == CV_8UC3);
	Size imgSize(src.size().width, src.size().width);
	dst.create(imgSize, src.type());
	cudaStream_t s = StreamAccessor::getStream(stream);
	correction_caller(src, dst, s);

}
int main() {
	Mat image0;
	image0 = imread("D:\\Documents\\Images\\capture.jpg");
	if (image0.empty()) {
		return -1;
	}
	Size imgSize(image0.size().width, image0.size().width);
	Mat dst(imgSize, CV_8UC3, Scalar(0, 0, 0));
	GpuMat gpuMat, output;
	gpuMat.upload(image0);//upload image to gpuMat
	correction(gpuMat, output);
	output.download(dst);
	imwrite("D:\\Documents\\Images\\corrected_image0.jpg", dst);
	printf("done\n");
	waitKey(0);
/*	VideoCapture cap0(1);
	if (!cap0.isOpened())
	{
		cout << "Cannot open the video cam 0" << endl;
		return -1;
	}

	while (1) {
		Mat image0;
		cap0.read(image0);
		if (!image0.empty())
		{
			imshow("image0", image0);
		}
		Size imgSize(image0.size().width, image0.size().width);
		Mat dst(imgSize, CV_8UC3, Scalar(0, 0, 0));
		//imshow("src",image);
		//if(!gpuMat.empty()){
		//free(gpuMat);
	//	}
		//if(!output.empty()){
		//free(output);
	//	}
		GpuMat gpuMat, output;
		gpuMat.upload(image0);//upload image to gpuMat
		correction(gpuMat, output);
		output.download(dst);
		imshow("corrected_image0", dst);
		if (waitKey(100) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}*/
	/*Mat image = imread("capture.jpg");
	Size imgSize(image.size().width,image.size().width);
	Mat dst(imgSize,CV_8UC3,Scalar(0,0,0));
	imshow("src",image);
	GpuMat gpuMat,output;
	gpuMat.upload(image);//upload image to gpuMat
	swap_rb(gpuMat,output);
	output.download(dst);*/
	//imwrite("result.jpg",dst);
	//waitKey(0);
	return 0;
}