#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

//#include <tensorflow/cc/ops/const_op.h>
//#include <tensorflow/cc/ops/image_ops.h>
//#include <tensorflow/cc/ops/standard_ops.h>
//#include <tensorflow/core/framework/graph.pb.h>
//#include <tensorflow/core/framework/tensor.h>
//#include <tensorflow/core/graph/default_device.h>
//#include <tensorflow/core/graph/graph_def_builder.h>
//#include <tensorflow/core/lib/core/errors.h>
//#include <tensorflow/core/lib/core/stringpiece.h>
//#include <tensorflow/core/lib/core/threadpool.h>
//#include <tensorflow/core/lib/io/path.h>
//#include <tensorflow/core/lib/strings/stringprintf.h>
//#include <tensorflow/core/platform/env.h>
//#include <tensorflow/core/platform/init_main.h>
//#include <tensorflow/core/platform/logging.h>
//#include <tensorflow/core/platform/types.h>
//#include <tensorflow/core/public/session.h>
//#include <tensorflow/core/util/command_line_flags.h>

std::string get_tegra_pipeline(int width, int height, int fps) {
    return "nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(width) + ", height=(int)" +
           std::to_string(height) + ", format=(string)I420, framerate=(fraction)" + std::to_string(fps) +
           "/1 ! nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
    cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
    cv::waitKey(30);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle nh;

  // Options
  int WIDTH = 1920;
  int HEIGHT = 1080;
  int FPS = 30;
  // Define the gstream pipeline
  std::string pipeline = get_tegra_pipeline(WIDTH, HEIGHT, FPS);
  std::cout << "Using pipeline: \n\t" << pipeline << "\n";
  // Create OpenCV capture object, ensure it works.
  cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
  if (!cap.isOpened()) {
      std::cout << "Connection failed";
      return -1;
  }
  // View video
  cv::Mat frame;
  while (1) {
      cap >> frame;  // Get a new frame from camera
  }

  //cv::namedWindow("view");
  //cv::startWindowThread();
  //image_transport::ImageTransport it(nh);
  //image_transport::Subscriber sub = it.subscribe("/csi_cam/image_raw", 1, imageCallback);
  //ros::spin();
  //cv::destroyWindow("view");
}
