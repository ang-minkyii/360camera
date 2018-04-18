#include "CameraStreamer.hpp"

int main(){
	vector<int> camera_index = {2,3};

	vector<string> label;
	for (int i = 0; i < camera_index.size(); i++){
		string title = "Cam " + to_string(i);
		label.push_back(title);
	}

	CameraStreamer cam(camera_index);

	while (waitKey(1) != 27){
		for (int i = 0; i < camera_index.size(); i++){
			Mat frame;
			if (cam.frame_queue[i]->try_pop(frame)){
				imshow(label[i], frame);
			}
		}
	}
	return 0;
}   