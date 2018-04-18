#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cuda.h>
#include <cuda_runtime.h>
#include <string>
#include "utils.h"

using namespace cv;
size_t width();
size_t height();
void preProcess(uchar4 **inputImage, uchar4 **rectImage,
		uchar4 **d_imageOrg, uchar4 **d_imageRect,
		const std::string &filename);
void postProcess(const std::string& output_file, uchar4* data_ptr);
void cleanUp();
