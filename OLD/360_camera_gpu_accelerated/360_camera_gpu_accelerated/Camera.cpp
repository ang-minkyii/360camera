#include "Camera.h"


Camera::~Camera()
{
}
Camera::Camera()
{

}

Camera::Camera(int x, int y, double bt, double ex, int gn)
{
	cout << "A new camera object has been created" << endl;
	setHeight(y);
	setWidth(x);
	brightness = bt;
	exposure = ex;
	setStream(0);
	gain = gn;
}

void Camera::setHeight(int h) {
	picHeight = h;
}

void Camera::setWidth(int w) {
	picWidth = w;
}

void Camera::setStream(int id) {
	stream = VideoCapture(id);
	stream.set(CV_CAP_PROP_FRAME_WIDTH, picWidth);
	stream.set(CV_CAP_PROP_FRAME_HEIGHT, picHeight);
	stream.set(CV_CAP_PROP_FPS,30);
	stream.set(CV_CAP_PROP_EXPOSURE, exposure);
	stream.set(CV_CAP_PROP_BRIGHTNESS, brightness);
	stream.set(CV_CAP_PROP_GAIN, gain);
}


VideoCapture Camera::getStream(void) {
	return stream;
}

void Camera::getProperties(void) {
	cout << "Brightness: " << stream.get(CV_CAP_PROP_BRIGHTNESS) << endl;
	cout << "Exposure: " << stream.get(CV_CAP_PROP_EXPOSURE) << endl;
	cout << "Contrast: " << stream.get(CV_CAP_PROP_CONTRAST) << endl;
	cout << "Gain: " << stream.get(CV_CAP_PROP_GAIN) << endl;
	cout << "Format: " << stream.get(CV_CAP_PROP_FORMAT) << endl;
	cout << "Mode: " << stream.get(CV_CAP_PROP_MODE) << endl;
	cout << "Saturation: " << stream.get(CV_CAP_PROP_SATURATION) << endl;
	cout << "Hue: " << stream.get(CV_CAP_PROP_HUE) << endl << endl;
}