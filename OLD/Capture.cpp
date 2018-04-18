/*
 * Capture.cpp
 *
 *  Created on: 13 Feb 2017
 *      Author: andy
 */

#include "Capture.hpp"

Capture::Capture(const char *deviceFile)
{
	//read and write access required and requested
	if((fd = open(deviceFile,O_RDWR))<0){
		cout << "Error opening device." << endl;
		perror("open");
		exit(1);
	}

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
}

Capture::~Capture()
{
	if(ioctl(fd, VIDIOC_STREAMOFF, &type) < 0){
		perror("VIDIOC_STREAMOFF");
		exit(1);
	}
	close(fd);
}

void Capture::startCapture()
{
	//Setting video format with IOCTL calls

	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	format.fmt.pix.width = fWidth;
	format.fmt.pix.height = fHeight;

	if(ioctl(fd,VIDIOC_S_FMT, &format) < 0){
		perror("VIDIOC_S_FMT");
		exit(1);
	}else{
		cout << "Video format of device set. Proceed." << endl;
	}

	//Initializing buffer
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

	buffer_start = mmap(
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
	memset(&bufferinfo2, 0, sizeof(bufferinfo2));

	bufferinfo2.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	bufferinfo2.memory = V4L2_MEMORY_MMAP;
	bufferinfo2.index = 0;

	type = bufferinfo2.type;

	if(ioctl(fd, VIDIOC_STREAMON, &type) < 0){
		perror("VIDIOC_STREAMON");
		exit(1);
	}
	img = Mat(Size(fWidth, fHeight), CV_8UC1, (uchar *)buffer_start);
}


Mat Capture::grabFrame()
{
	if(ioctl(fd, VIDIOC_QBUF, &bufferinfo2) < 0){
		perror("VIDIOC_QBUF");
		exit(1);
	}

	if(ioctl(fd ,VIDIOC_DQBUF, &bufferinfo2) < 0){
		perror("VIDIOC_DQBUF");
		exit(1);
	}

	img.data = (uchar *)buffer_start;
	imdecode(img,1,&image);
	return image;
}

void Capture::setWidth(int width)
{
	fWidth = width;
}

void Capture::setHeight(int height)
{
	fHeight = height;
}
