#include "CameraStreamer.hpp"

CameraStreamer::CameraStreamer(vector<int> index)
{
	camera_index = index;
	camera_count = index.size();
	isUSBCamera = true;

	startMultiCapture();
}

CameraStreamer::~CameraStreamer()
{
	stopMultiCapture();
}

void CameraStreamer::captureFrame(int index)
{
	VideoCapture *capture = camera_capture[index];
	capture->set(CV_CAP_PROP_FOURCC, CV_FOURCC('M','J','P','G'));
	capture->set(CV_CAP_PROP_FRAME_WIDTH, 1920);
	capture->set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
	capture->set(CV_CAP_PROP_FPS, 30);
	while (1){
		Mat frame;
		(*capture) >> frame;
		frame_queue[index]->push(frame);
		frame.release();
	}
}

void CameraStreamer::startMultiCapture()
{
	VideoCapture *capture;
	thread *t;
	concurrent_queue<Mat> *q;

	for (int i = 0; i < camera_count; i++){
		int idx = camera_index[i];
		capture = new VideoCapture(idx);
		cout << "Camera " << to_string(idx) << " setup" << endl;

		camera_capture.push_back(capture);
		t = new thread(&CameraStreamer::captureFrame, this, i);
		camera_thread.push_back(t);

		q = new concurrent_queue<Mat>;
		frame_queue.push_back(q);
	}
}

void CameraStreamer::stopMultiCapture()
{
	VideoCapture *cap;
	for (int i = 0; i < camera_count; i++){
		cap = camera_capture[i];
		if (cap->isOpened()){
			cap->release();
			cout << "Capture " << i << " released." << endl;
		}
	}
}