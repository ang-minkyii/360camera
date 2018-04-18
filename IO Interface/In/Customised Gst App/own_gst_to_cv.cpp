#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <stdlib.h>
#include <cstdio>
#include <ctime>

#include "opencv2/opencv.hpp"
using namespace cv;

//synchronized deque
std::deque<Mat> frameQueue;

GstFlowReturn
new_preroll(GstAppSink *appsink, gpointer data) {
  g_print ("Got preroll!\n");
  return GST_FLOW_OK;
}

GstFlowReturn
new_sample(GstAppSink *appsink, gpointer data) {
  static int framecount = 0;
  framecount++;

  GstSample *sample = gst_app_sink_pull_sample(appsink);
  GstCaps *caps = gst_sample_get_caps(sample);
  GstBuffer *buffer = gst_sample_get_buffer(sample);
  const GstStructure *info = gst_sample_get_info(sample);

  // ---- Read frame and convert to opencv format ---------------

  GstMapInfo map;
  gst_buffer_map (buffer, &map, GST_MAP_READ);

  // convert gstreamer data to OpenCV Mat

  Mat frame(Size(1920, 1080), CV_8UC3, (char*)map.data, Mat::AUTO_STEP);
  int frameSize = map.size;

  // TODO: synchronize this....
  frameQueue.push_back(frame);

  gst_buffer_unmap(buffer, &map);

  // ------------------------------------------------------------

  // print dot every 30 frames
  if (framecount%30 == 0) {
    g_print (".");
  }

  // show caps on first frame
  if (framecount == 1) {
    g_print ("%s\n", gst_caps_to_string(caps));
  }


  gst_sample_unref (sample);
  return GST_FLOW_OK;
}

static gboolean
my_bus_callback (GstBus *bus, GstMessage *message, gpointer data) {
  //g_print ("Got %s message\n", GST_MESSAGE_TYPE_NAME (message));
  switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_ERROR: {
      GError *err;
      gchar *debug;

      gst_message_parse_error (message, &err, &debug);
      //g_print ("Error: %s\n", err->message);
      g_error_free (err);
      g_free (debug);    
      break;
    }
    case GST_MESSAGE_EOS:
      /* end-of-stream */
      break;
    default:
      /* unhandled message */
      break;
  }
  /* we want to be notified again the next time there is a message
   * on the bus, so returning TRUE (FALSE means we want to stop watching
   * for messages on the bus and our callback should not be called again)
   */
  return TRUE;
}

void ocv_process(){

  
    namedWindow("edges",1);
  while(1) {

    g_main_iteration(false);

      // TODO: synchronize queue
    if (frameQueue.size() > 0) {

      Mat frame = frameQueue.front();

      imshow("edges", frame);

    //double fps = 1/((end-start)/(double)CLOCKS_PER_SEC);
     //g_print ("FPS: %d\n", fps);

      cv::waitKey(30);
      frameQueue.clear();
    }
  }  
}

int
main (int argc, char *argv[])
{

  GError *error = NULL;

  gst_init (&argc, &argv);
//v4l2src ! videoconvert ! ximagesink
//"v4l2src device=/dev/video0 ! image/jpeg, width=1920,height=1080,framerate=30/1 ! jpegparse ! jpegdec ! videoconvert ! appsink"
  /*gchar *descr = g_strdup(
    "v4l2src ! "
    //"video/x-raw,format=RGB ! "
     "video/x-raw,format=YVYU ! "
    "videoconvert ! "
    "appsink name=sink sync=true"
  );*/
//filesrc location=test.mp4 ! qtdemux ! h264parse ! omxh264dec ! videoconvert ! capsfilter caps=video/x-raw-rgb ! appsink
  gchar *descr = g_strdup(
    "v4l2src device=/dev/video0 ! "
    //"video/x-raw,format=RGB ! "
     "image/jpeg, width=1920,height=1080,framerate=30/1 ! "
     "jpegparse ! "
     "jpegdec ! "
    "videoconvert ! "
    "video/x-raw, format=(string)BGR ! "
    "appsink name=sink "
  );

/*gchar *descr = g_strdup(
    "filesrc location=test.mp4 ! "
    //"video/x-raw,format=RGB ! "
     "qtdemux ! "
     "h264parse ! "
     "omxh264dec ! "
    "videoconvert ! "
    "video/x-raw, format=(string)BGR ! "
    "appsink name=sink "
  );*/
  
  GstElement *pipeline = gst_parse_launch (descr, &error);

  if (error != NULL) {
    g_print ("could not construct pipeline: %s\n", error->message);
    g_error_free (error);
    exit (-1);
  }

  /* get sink */
  GstElement *sink = gst_bin_get_by_name (GST_BIN (pipeline), "sink");

  gst_app_sink_set_emit_signals((GstAppSink*)sink, true);
  gst_app_sink_set_drop((GstAppSink*)sink, true);
  gst_app_sink_set_max_buffers((GstAppSink*)sink, 1);
  GstAppSinkCallbacks callbacks = { NULL, new_preroll, new_sample };
  gst_app_sink_set_callbacks (GST_APP_SINK(sink), &callbacks, NULL, NULL);

  GstBus *bus;
  guint bus_watch_id;
  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  bus_watch_id = gst_bus_add_watch (bus, my_bus_callback, NULL);
  gst_object_unref (bus);

  gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_PLAYING);

  
  ocv_process();

  gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_NULL);
  gst_object_unref (GST_OBJECT (pipeline));

  return 0;  
}