#pragma once
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

class Camera
{
public:
	Camera();
	Camera(int x, int y, double bt, double ex, int gn); //This is the constructor
	~Camera();
	void setHeight(int h);
	void setWidth(int w);
	void setStream(int id);
	VideoCapture getStream(void);
	void getProperties(void);
	double brightness;
	double exposure;
	int gain;
private:
	int picHeight;
	int picWidth;
	VideoCapture stream;
};

