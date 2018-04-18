#include "functions.hpp"

Mat imageOrg;
Mat imageRect;

uchar4 *d_imageOrg_;
uchar4 *d_imageRect_;
double rateOfWindow = 1;
size_t width() {return imageOrg.size().width * rateOfWindow;}
size_t height() {return width();}

void preProcess(uchar4 **inputImage, uchar4 **rectImage,
		uchar4 **d_imageOrg, uchar4 **d_imageRect,
		const std::string &filename){
	//make sure the initialization is ok
	checkCudaErrors(cudaFree(0));

	Mat image;
	image = imread(filename.c_str(), CV_LOAD_IMAGE_COLOR);
	if(image.empty()){
		std::cout << "COULD NOT OPEN THE IMAGE: "<< filename << std::endl;
		exit(1);
	}

	//imageRect.create(image,rows,image.cols,CV_8UC1);
	cvtColor(image,imageOrg,CV_BGR2RGBA);
	imageRect.create(image.rows,image.cols,CV_8UC1);
	if(!imageOrg.isContinuous() || !imageRect.isContinuous()){
		std::cerr << "Images are not continuous!" << std::endl;
		exit(1);
	}

	*inputImage = (uchar4 *)imageOrg.ptr<unsigned char>(0);
	*rectImage = (uchar4 *)imageOrg.ptr<unsigned char>(0);
	const size_t numPixels = width() * height();

	checkCudaErrors(cudaMalloc(d_imageOrg,sizeof(uchar4) * numPixels));
	checkCudaErrors(cudaMalloc(d_imageRect,sizeof(uchar4)* numPixels));
	checkCudaErrors(cudaMemset(*d_imageRect,0,numPixels * sizeof(uchar4)));

	//copy input array to the GPU
	checkCudaErrors(cudaMemcpy(*d_imageOrg,*inputImage,sizeof(uchar4)*numPixels,cudaMemcpyHostToDevice));
	d_imageOrg_ = *d_imageOrg;
	d_imageRect_ = *d_imageRect;

}

void postProcess(const std::string& output_file, uchar4* data_ptr){
	Mat output(height(),width(),CV_8UC1,(void*)data_ptr);
	imwrite(output_file.c_str(),output);
}

void cleanUp(){
	cudaFree(d_imageOrg_);
	cudaFree(d_imageRect_);
}
