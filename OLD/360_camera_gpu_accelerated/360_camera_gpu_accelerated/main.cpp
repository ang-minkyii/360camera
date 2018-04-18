#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Camera.h"
#include <iostream>


class Camera;
using namespace cv;
using namespace std;

#define PIC_WIDTH 1920
#define PIC_HEIGHT 1080

Mat fisheye_distortion_correction(Mat imageInput, double correction, double zoom);
Mat fisheye_correction_old(Mat inputImage);

int main() {   //0 is the id of video device.0 if you have only one camera.
	int zoom1 = 1.2;
	int zoom2 = 1.2;
	int correction1 = 3;
	int correction2 = 3;
	String filename1 = "my_photo-1.jpg";
	String filename2 = "my_photo-2.jpg";
	Camera cam1(PIC_WIDTH, PIC_HEIGHT, 10, -7, 50);
	Camera cam2(PIC_WIDTH, PIC_HEIGHT, 10, -7, 10);
	cam1.setStream(2);
	cam2.setStream(3);


	if (!cam2.getStream().isOpened()) { //check if video device has been initialised
		cout << "cannot open camera 1";
	}
	if (!cam1.getStream().isOpened()) { //check if video device has been initialised
		cout << "cannot open camera 2";
	}

	//cam1.getProperties();
	//cam2.getProperties();

	while (true) {
		Mat cameraFrame1;
		Mat cameraFrame2;
		Mat correctedFrame1;
		Mat correctedFrame2;

		cam1.getStream().read(cameraFrame1);
		cam2.getStream().read(cameraFrame2);
		correctedFrame1 = fisheye_distortion_correction(cameraFrame1, correction1, zoom1);
		correctedFrame2 = fisheye_distortion_correction(cameraFrame2, correction2, zoom2);

		//cout << "Corrected" << endl;

		if (!cameraFrame1.empty()) {
			imshow("window1", correctedFrame1);
		}
		if (!cameraFrame2.empty()) {
			imshow("window2", correctedFrame2);
		}


		//if (!cameraFrame1.empty()) {
		//	imshow("window1", cameraFrame1);
		//}
		//if (!cameraFrame2.empty()) {
		//	imshow("window2", cameraFrame2);
		//}
		int key = waitKey(30);
		if (key == 27) {
			printf("Exit program.\n");
			break;
		}
		if (key == 's') {
			imwrite(filename1, cameraFrame1);
			imwrite(filename2, cameraFrame2);
			break;
		}
	}
	return 0;
}

Mat fisheye_distortion_correction(Mat imageInput, double correction, double zoom) {
	//Sampling algorithm
	int half_width = PIC_WIDTH/2;
	int half_height = PIC_HEIGHT/2;
	double correction_radius = 0.0;
	int new_x = 0;
	int source_x = 0;
	int source_y = 0;
	int new_y = 0;
	double distance = 0.0;
	double r = 0.0;
	double theta = 0.0;

	//CvScalar temp;
	//IplImage* final_image = cvCreateImage(cvSize(PIC_WIDTH, PIC_HEIGHT), IPL_DEPTH_8U, 3);
	Mat finalImage = Mat(Size(PIC_WIDTH,PIC_HEIGHT), CV_8UC3);

	if (correction == 0) {
		correction = 0.00001;
	}
	correction_radius = sqrt(pow(PIC_WIDTH, 2) + pow(PIC_HEIGHT, 2)) / correction;

	for (int i = 0; i < PIC_WIDTH; i++) {
		for (int j = 0; j < PIC_HEIGHT; j++) {
			new_x = i - half_width;
			new_y = j - half_height;
			distance = sqrt((pow(new_x, 2) + pow(new_y, 2)));
			r = distance / correction_radius;

			if (r == 0) {
				theta = 1;
			}
			else {
				theta = atan(r) / r;
			}

			source_x = (int)(half_width + theta*new_x*zoom);
			source_y = (int)(half_height + theta*new_y*zoom);

			//If able to take care of the edges well enough, more information will be available
			if (source_x >= PIC_WIDTH) {
				source_x = (PIC_WIDTH - 1);
			}
			if (source_y >= PIC_HEIGHT) {
				source_y = (PIC_HEIGHT - 1);
			}
			if (source_x <= 0) {
				source_x = 0;
			}
			if (source_y <= 0) {
				source_y = 0;
			}

			//cout << "x: " << source_x << endl;
			//cout << "y: " << source_y << endl;

			Vec3b color = imageInput.at<Vec3b>(Point(source_x, source_y));

			finalImage.at<Vec3b>(Point(i, j)) = color;
		}
	}
	return finalImage;
}

Mat fisheye_correction_old(Mat inputImage) {
	Mat finalImage = Mat(Size(PIC_WIDTH, PIC_HEIGHT), CV_8UC3);
	int source_x = 0;
	int source_y = 0;
	int radius = PIC_HEIGHT / 2;

	for (int i = 0; i < PIC_WIDTH; i++) {
		for (int j = 0; j < PIC_HEIGHT; j++) {



			if (source_x >= PIC_WIDTH) {
				source_x = (PIC_WIDTH - 1);
			}
			if (source_y >= PIC_HEIGHT) {
				source_y = (PIC_HEIGHT - 1);
			}
			if (source_x <= 0) {
				source_x = 0;
			}
			if (source_y <= 0) {
				source_y = 0;
			}
			//temp = cvGet2D(image_array, source_y, source_x);
			Vec3b color = inputImage.at<Vec3b>(Point(source_x, source_y));
			//cvSet2D(final_image, j, i, temp);
			finalImage.at<Vec3b>(Point(i, j)) = color;
		}
	}

	return finalImage;
}



