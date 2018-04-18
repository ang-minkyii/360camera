g++ test_cam.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -o test_cam

gst-launch-1.0 v4l2src device="/dev/video0" ! video/x-raw,width=640,height=480 ! autovideosink

v4l2-ctl --list-devices


cmake -DCMAKE_BUILD_TYPE=Release -DWITH_OPENGL=ON -DWITH_NVCUVID=ON -DCMAKE_INSTALL_PREFIX=/usr/local -DBUILD_TIFF=ON -DBUILD_EXAMPLES=ON -DCUDA_GENERATION=Auto -DWITH_JPEG=ON -DBUILD_JPEG=OFF -DJPEG_INCLUDE_DIR=/opt/libjpeg-turbo/include/ -DJPEG_LIBRARY=/opt/libjpeg-turbo/lib/libjpeg.a /media/ubuntu/e0a73a8e-8443-4c8f-97fb-25b57134763b/opencv-2.4.13/ -DWITH_TBB=ON -DWITH_CUDA=ON -DBUILD_NEW_PYTHON_SUPPORT=ON ..


g++ main_1.cpp CameraStreamer.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -o test_cam

-DWITH_OPENGL=ON
-DWITH_NVCUVID=ON


#setup ssh
1. curl ipinfo.io/ip
2. turn on router Port Forwarding / Port Triggering
 
Please select the service type.

Port Forwarding
Port Triggering

3. ssh


gst-launch-1.0 -v v4l2src device=/dev/video0 ! image/jpeg, width=1920,height=1080,framerate=30/1 ! jpegparse ! jpegdec ! videoconvert ! videoscale ! avimux ! filesink location=output0.mp4 sync=false v4l2src device=/dev/video1 ! image/jpeg, width=1920,height=1080,framerate=30/1 ! jpegparse ! jpegdec ! videoconvert ! videoscale ! avimux ! filesink location=output1.mp4 sync=false v4l2src device=/dev/video2 ! image/jpeg, width=1920,height=1080,framerate=30/1 ! jpegparse ! jpegdec ! videoconvert ! videoscale ! avimux ! filesink location=output2.mp4 sync=false v4l2src device=/dev/video3 ! image/jpeg, width=1920,height=1080,framerate=30/1 ! jpegparse ! jpegdec ! videoconvert ! videoscale ! avimux ! filesink location=output3.mp4 sync=false

gst-launch-1.0 -v v4l2src device=/dev/video0 ! image/jpeg, width=1920,height=1080,framerate=30/1 ! jpegparse ! jpegdec ! videoconvert ! videorate ! videoscale ! avimux ! filesink location=output0.mp4 sync=false v4l2src device=/dev/video1 ! image/jpeg, width=1920,height=1080,framerate=30/1 ! jpegparse ! jpegdec ! videoconvert ! videorate ! videoscale ! avimux ! filesink location=output1.mp4 sync=false v4l2src device=/dev/video2 ! image/jpeg, width=1920,height=1080,framerate=30/1 ! jpegparse ! jpegdec ! videoconvert ! videorate ! videoscale ! avimux ! filesink location=output2.mp4 sync=false v4l2src device=/dev/video3 ! image/jpeg, width=1920,height=1080,framerate=30/1 ! jpegparse ! jpegdec ! videoconvert ! videorate ! videoscale ! avimux ! filesink location=output3.mp4 sync=false

gst-launch-1.0 -v v4l2src device=/dev/video0 ! video/x-raw,width=640,height=480 ! autovideosink



gst-inspect-1.0