#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <iostream>

#include <cstdlib>
//#include <pthread.h>
#include "opencv2/imgproc.hpp"
#include <thread>  
#include <mutex>          // std::mutex

//std::mutex mtx;
//std::mutex mtx1;



using namespace cv;
using namespace std;

/*int main(){
	VideoCapture cap(0);
	VideoCapture cap2(1);

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video file" << endl;
		return -1;
	}

	//    if ( !cap2.isOpened() )  // if not success, exit program
	//    {
	//        cout << "Cannot open the video file" << endl;
	//        return -1;
	//    }

	Mat Webcam;
	Mat Webcam2;

	while (true){
		//Takes x seconds
		//        Mat Webcam;
		cap.read(Webcam);
		//resize(Webcam, Webcam, Size(Webcam.cols / 2, Webcam.rows / 2)); // to half size
		namedWindow("Display frame", CV_WINDOW_AUTOSIZE);
		imshow("Webcam1", Webcam);

		if (waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
}
*/


void test_gst_cam(){
	//mtx.lock();
	std::thread::id this_id = std::this_thread::get_id();
	printf("%s\n", "t1");

	VideoCapture cap1("v4l2src device=/dev/video0 ! video/x-raw, framerate=30/1, width=1920, height=1080, format=RGB ! videorate ! videoconvert ! appsink");
	//I assigned a  new space for each camera to store a frame in one second 
	Mat Webcam1;
	std::cout << "thread " << this_id << "\n";
	while (true){

		cap1.read(Webcam1);
		//namedWindow("Display frame_1", CV_WINDOW_AUTOSIZE);
		imshow("Webcam2", Webcam1);
		if (waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
}
void test(){
	//mtx.lock();
	std::thread::id this_id = std::this_thread::get_id();
	printf("%s\n", "t1");

	VideoCapture cap1(1);
	//I assigned a  new space for each camera to store a frame in one second 
	Mat Webcam1;
	std::cout << "thread " << this_id << "\n";
	while (true){

		cap1.read(Webcam1);
		//namedWindow("Display frame_1", CV_WINDOW_AUTOSIZE);
		imshow("Webcam2", Webcam1);
		if (waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	
	//mtx.unlock();
}

void test1(){
	//mtx.lock();
	std::thread::id this_id = std::this_thread::get_id();
	printf("%s\n", "t1");
	VideoCapture cap(0);
	//I assigned a  new space for each camera to store a frame in one second 
	Mat Webcam;
	std::cout << "thread " << this_id << "\n";
	while (true){

		cap.read(Webcam);
		//namedWindow("Display frame", CV_WINDOW_AUTOSIZE);
		imshow("Webcam1", Webcam);
		if (waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	
	//mtx.unlock();
}

void test2(){
	//VideoCapture::open();
	//mtx.lock();
	printf("%s\n", "t2");
	VideoCapture cap("C:/Users/x/Downloads/final.avi");
	//VideoCapture cap2(1);
	
	Mat Webcam_1;
	while (true){

		bool isframe = cap.read(Webcam_1);
		if (!isframe){
			exit(EXIT_FAILURE);
		}
		//namedWindow("Display frame", CV_WINDOW_AUTOSIZE);
		imshow("Webcam1", Webcam_1);
		if (waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	//mtx.unlock();
}

void test3(){
	printf("%s\n", "t3");
	//mtx.lock();
	//VideoCapture::open();
	VideoCapture cap("C:/Users/x/Downloads/final (1).avi");
	//VideoCapture cap2(1);


	Mat Webcam;
	while (true){

		bool isframe = cap.read(Webcam);
		if (!isframe){
			exit(EXIT_FAILURE);
		}
		//namedWindow("Display frame", CV_WINDOW_AUTOSIZE);
		imshow("Webcam1", Webcam);
		if (waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	//mtx.unlock();
}
int main()
{

	std::thread t1(test);  // spawn new thread that calls foo()
	std::thread t2(test1);
	std::cout << "t1 and t2 now execute concurrently...\n";
	//std::cout << std::thread::get_id();

	// synchronize threads:
	t1.join();                // pauses until first finishes
	t2.join();               // pauses until second finishes

	std::cout << "test completed.\n";

	return 0;
}
