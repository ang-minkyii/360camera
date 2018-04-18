#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <concurrent_queue.h>
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\videoio\videoio.hpp"

using namespace std;
using namespace cv;
using namespace concurrency;

class CameraStreamer{

public:
	vector<int> camera_index;
	vector<VideoCapture *> camera_capture;
	vector<concurrent_queue<Mat>*> frame_queue;
	vector<thread *> camera_thread;

	CameraStreamer(vector<int> index);
	~CameraStreamer();

private:
	bool isUSBCamera;
	int camera_count;
	void startMultiCapture();
	void stopMultiCapture();
	void captureFrame(int index);
};