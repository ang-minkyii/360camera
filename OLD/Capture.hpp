/*
 * Capture.hpp
 *
 *  Created on: 13 Feb 2017
 *      Author: andy
 */

#ifndef CAPTURE_HPP_
#define CAPTURE_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/gpu/gpu.hpp>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
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

class Capture{
public:
	int fd;
	struct v4l2_capability cap;
	struct v4l2_format format;
	struct v4l2_requestbuffers buffrequest;
	struct v4l2_buffer bufferinfo;
	struct v4l2_buffer bufferinfo2;
	void *buffer_start;
	int type;
	int fWidth = 640;
	int fHeight = 480;
	Mat image;
	Mat img;

	Capture(const char *deviceFile);
	~Capture();
	void startCapture();
	void setWidth(int width);
	void setHeight(int height);
	Mat grabFrame();
};


#endif /* CAPTURE_HPP_ */
