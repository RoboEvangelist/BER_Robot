// exampleApp.c

#include <motor_control/motor_control.hpp>

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "vision_msgs/ObjectHypothesisWithPose.h"
#include "vision_msgs/Detection2D.h"
#include "vision_msgs/Detection2DArray.h"

#include <sstream>
#include <cmath>

uint32_t box_center_x = 0;
uint32_t box_center_y = 0;
uint32_t box_width = 0;
uint32_t box_height = 0;
float box_area= 0;

float image_area = 10000000;
float image_center = 0;

int32_t bbox_center_to_img_center_distance = 0;

void reset_bbox() {
  box_center_x = 0;
  box_center_y = 0;
  box_width = 0;
  box_height = 0;
  box_area= 0;
}

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void object_dection_callback(
  const vision_msgs::Detection2DArray::ConstPtr& msg) {
  if (msg->detections.size()) {
    /// area of the camera image
    image_area = msg->detections[0].source_img.height *
      msg->detections[0].source_img.width;
    image_center = msg->detections[0].source_img.width / 2;

    for (const auto detections: msg->detections) {
      box_center_x = detections.bbox.center.x;
      box_center_y = detections.bbox.center.y;
      box_width = detections.bbox.size_x;
      box_height = detections.bbox.size_y;
      box_area = box_width * box_height;
      /// compute how far in pixels a person is from center of image
      bbox_center_to_img_center_distance = box_center_x - image_center;
    }
  }
  else
    reset_bbox();
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "motor_control");
  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle nh;
  /// publisher to publish motor control commands
  ros::Publisher car_direction_pub =
    nh.advertise<std_msgs::String>("car_direction", 1);
  /// subscriver to the object detection
  /// let's set the queue size to five for now because the python object
  /// detections runs at about 4Hz
  ros::Subscriber sub = nh.subscribe("/objects", 5, object_dection_callback);
  /// send 30 messages per second
  ros::Rate loop_rate(10);
  
  uint32_t distance_from_center_threshold = 0; 
  float kMaxDistanceToImgCenterRatio = 1.0/5;
  float kMaxObjectAreaRatio = 1.0/2;
  float kMinObjectAreaRatio = 1.0/4;
  /**
   * This is a message object. You stuff it with data, and then publish it.
   */
  std_msgs::String msg;
  bool distance_is_big = false;

  while (ros::ok()) {
    std::stringstream ss;

    distance_from_center_threshold =
      (image_center * kMaxDistanceToImgCenterRatio);

    if ((distance_from_center_threshold > 0) && (box_area != 0)) {
      distance_is_big = (std::abs(bbox_center_to_img_center_distance) >
        distance_from_center_threshold);
      if ((box_center_x > image_center) && distance_is_big) {
        ss << "Turn Right";
      }
      else if ((box_center_x < image_center) && distance_is_big) {
        ss << "Trun Left";
      }
      else if ((box_area / image_area) > kMaxObjectAreaRatio) {
        /// if object is too big, move back
        ss << "Move Back";
      }
      else if ((box_area / image_area) < kMinObjectAreaRatio) {
        /// if object is too small, move forward
        ss << "Move Forward";
      }
      else {
        ss << "Stop";
      }
    }
    else {
      ss << "Stop";
    }

    msg.data = ss.str();

    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
    car_direction_pub.publish(msg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}

