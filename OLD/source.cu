/*
 * source.cu
 *
 *  Created on: 9 Feb 2017
 *      Author: andy
 */

#include "Capture.hpp"
#include <cstdio>
#include <ctime>

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/gpu/gpu.hpp>

#include <getopt.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

using namespace std;
using namespace cv;

int main(){
	int fd;

	//read and write access required and requested
	if((fd = open("/dev/video1",O_RDWR))<0){
		cout << "Error opening device." << endl;
		perror("open");
		exit(1);
	}

	struct v4l2_capability cap;
	if(ioctl(fd,VIDIOC_QUERYCAP, &cap) < 0){
		perror("VIDIOC_QUERYCAP");
		exit(1);
	}else{
		cout << "Capabilties of device retrieved." << endl;
	}

	//Checking video capabilities
	if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)){
		cout << "The device does not handle single-planar video capture." << endl;
	}else{
		cout << "This device can handle video capture. Proceed." << endl;
	}

	if(!(cap.capabilities & V4L2_CAP_STREAMING)){
		cout << "This device does not allow video streaming." << endl;
	}else{
		cout << "This device allows video streaming. Proceed." << endl;
	}

	//Setting video format with IOCTL calls
	struct v4l2_format format;
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	format.fmt.pix.width = 1920;
	format.fmt.pix.height = 1080;

	if(ioctl(fd,VIDIOC_S_FMT, &format) < 0){
		perror("VIDIOC_S_FMT");
		exit(1);
	}else{
		cout << "Video format of device set. Proceed." << endl;
	}

	//Initializing buffer
	struct v4l2_requestbuffers buffrequest;
	buffrequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buffrequest.memory = V4L2_MEMORY_MMAP;
	buffrequest.count = 1;

	if(ioctl(fd,VIDIOC_REQBUFS, &buffrequest) < 0){
		perror("VIDIOC_REQBUFS");
		exit(1);
	}else{
		cout << "Buffer initialization complete. Proceed." << endl;
	}

	//Allocating buffers
	struct v4l2_buffer bufferinfo;
	memset(&bufferinfo, 0, sizeof(bufferinfo));

	bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	bufferinfo.memory = V4L2_MEMORY_MMAP;
	bufferinfo.index = 0;

	if(ioctl(fd, VIDIOC_QUERYBUF, &bufferinfo) < 0){
		perror("VIDIOC_QUERYBUF");
		exit(1);
	}else{
		cout << "Buffer memory allocated. Proceed." << endl;
	}

	void *buffer_start = mmap(
			NULL,
			bufferinfo.length,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			fd,
			bufferinfo.m.offset
			);

	if(buffer_start == MAP_FAILED){
		perror("mmap");
		exit(1);
	}

	memset(buffer_start, 0 ,bufferinfo.length);

	struct v4l2_buffer bufferinfo2;
	memset(&bufferinfo2, 0, sizeof(bufferinfo2));

	bufferinfo2.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	bufferinfo2.memory = V4L2_MEMORY_MMAP;
	bufferinfo2.index = 0;

	int type = bufferinfo2.type;
	if(ioctl(fd, VIDIOC_STREAMON, &type) < 0){
		perror("VIDIOC_STREAMON");
		exit(1);
	}

	int key;
	namedWindow("image1",CV_WINDOW_AUTOSIZE);

	while(1){
		if(ioctl(fd, VIDIOC_QBUF, &bufferinfo2) < 0){
			perror("VIDIOC_QBUF");
			exit(1);
		}

		if(ioctl(fd ,VIDIOC_DQBUF, &bufferinfo2) < 0){
			perror("VIDIOC_DQBUF");
			exit(1);
		}

		Mat img(Size(1920, 1080), CV_8UC1, (uchar *)buffer_start);
		Mat image = imdecode(img,1);

		if(!image.empty()){
			imshow("image1",image);
			//imwrite("image.jpg",image);
		}else{
			cout << "Image is empty" << endl;
		}
		key=waitKey(10);
		if(key == 27){
			break;
		}
	}

	if(ioctl(fd, VIDIOC_STREAMOFF, &type) < 0){
		perror("VIDIOC_STREAMOFF");
		exit(1);
	}

	//Convert image in buffer memory to opencv Mat
	Mat img(Size(1280, 720), CV_8UC1, (uchar *)buffer_start);
	Mat image = imdecode(img,1);
	namedWindow("image1",CV_WINDOW_AUTOSIZE);

	if(!image.empty()){
		imshow("image1",image);
		waitKey(0);
		imwrite("image.jpg",image);
	}else{
		cout << "Image is empty" << endl;
	}

	cout << "Successful" << endl;

	close(fd);
	return EXIT_SUCCESS;
}*/


int main(){

	clock_t start;
	clock_t end;
	Capture cap1("/dev/video1");
	Capture cap2("/dev/video2");
	cap1.setWidth(1920);
	cap1.setHeight(1080);
	cap2.setWidth(1920);
	cap2.setHeight(1080);

	cap1.startCapture();
	cap2.startCapture();

	Mat image1;
	Mat image2;
	int key;
	namedWindow("image1",1);
	//namedWindow("image2",1);

	while(1){
		start = clock();
		image1 = cap1.grabFrame();
	//	image2 = cap2.grabFrame();
		end = clock();

		cout << "FPS:" << 1/((end-start)/(double)CLOCKS_PER_SEC) << endl;
		imshow("image1",image1);
		//imshow("image2",image2);

		key = waitKey(1);
		if(key == 27){
			break;
		}
	}
	return EXIT_SUCCESS;
}
