#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>    /*1*/
#include <exception>

#include <ir_sensor/jetsonGPIO.h>

#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/Int32MultiArray.h>
#include <sensor_msgs/Range.h>

#ifndef IRSENSOR_H
#define IRSENSOR_H

class IrSensor
{
  ros::NodeHandle nh_;
  /**< subscriber for the camera info of the rectified mono image. */
  //ros::Publisher pub_range("range_data", &range_msg_);
  ros::Publisher pub_range_;

public:
  IrSensor(int32_t user_id)
  {
    std::cout << "\n---------------------------\n";
    std::cout << ros::this_node::getName(); 
    std::cout << " Node Parameters:\n---------------------------\n";
    /** parameters from a YAML File. They are Public since they are not inside a
     *  <node/> tag in the launch file
     */
    /// initialize topic names to a default state
//    ros::NodeHandle param_nh_("~");
//    /// Arguments variables
//    if (!param_nh_.getParam("cam_relative_frame", camera_frame_))
//    {
//      ROS_WARN("cam_relative_frame was not found on Param Server!");
//      ROS_WARN("See your launch file!");
//      ROS_WARN("Shutting down %s parameters node handle",
//        ros::this_node::getName().c_str());
//      param_nh_.shutdown();
//    }
//    if (!param_nh_.getParam("tracker_tf_name", tracker_tf_name_))
//    {
//      ROS_WARN("tracker_tf_name was not found on Param Server!");
//      ROS_WARN("See your launch file!");
//      ROS_WARN("shutting down %s parameters node handle",
//        ros::this_node::getName().c_str());
//      param_nh_.shutdown();
//    }
//    if (!param_nh_.getParam("rgb_camera_info", rgb_camera_info_))
//    {
//      ROS_WARN("rgb_camera_info was not found on Param Server!");
//      ROS_WARN("See your launch file!");
//      ROS_WARN("shutting down speak gesture parameters node handle");
//      param_nh_.shutdown();
//    }
//    param_nh_.param("min_user_distance", min_user_distance_, 0.9);
//    param_nh_.param("max_user_distance", max_user_distance_, 1.8);
    pub_range_ = nh_.advertise<sensor_msgs::Range>("/ir/range_data", 50);
//
//    /** Subscribe to the skeleton people tracking feed, then republish if
//     *  user requested through the TrackPeople service is found
//     */
//    people_list_raw_sub_ = nh_.subscribe("/visual/people_list_raw", 50,
//      &IrSensor::peopleListRawCb, this);
//    /** publishers for detected user ID.
//     *  lets buffer up 1000 messages in a message queue, but lets check to make
//     *  sure that the information
//     *  being published is not too old, or it can cause synchronization problems
//     */
//    track_people_raw_pub_ = nh_.advertise<wiener_msgs::UserInView_Raw>(
//      "/visual/track_people_raw", 50);
//
//    /// publishes whether arms are in motion or not
//    system_to_skeleton_vector_pub_ =
//      nh_.advertise<std_msgs::Int32MultiArray>("/visual/system_to_skeleton_id",
//      30);
//
//    /// service to track a specific user ID (how do we determine who to track??)
//    track_people_server_ = nh_.advertiseService("/visual/track_people",
//      &IrSensor::trackPeopleServiceCb, this);
//
//    system_user_id_ = 0;
//    skeleton_user_id_ = 0;
//    /// dictates wether we track the closest, or a specific one
//    track_closest_system_user_id_ = false;
//
//    /* first Item is the system ID */
//    /* second Item is the skeleton ID */
//    system_and_skeleton_id_msgs_.data.push_back(0);
//    system_and_skeleton_id_msgs_.data.push_back(0);

  range_msg_.radiation_type = sensor_msgs::Range::INFRARED;
  range_msg_.header.frame_id = "/ir_ranger";
  range_msg_.field_of_view = 0.01;
  range_msg_.min_range = 0.03;  // For GP2D120XJ00F only. Adjust for other IR rangers
  range_msg_.max_range = 0.4;   // For GP2D120XJ00F only. Adjust for other IR rangers
  }

  ~IrSensor()
  {
    //cv::destroyWindow(right_hand_window_);
  }

  /** @brief Gets message from /bodyTracker/people topic with information about
   *         all user ID's that are in view
   * 
   *  @param people_msg stors user ID's and their respective body part locations
   *         in meters
   */
  float get_range();
  void publish_range();

private:
  // stores the range of the IR sensor
  sensor_msgs::Range range_msg_;
  uint32_t analog_pin_;
  uint32_t range_timer_;
}; 
   
#endif  //IRSENSOR_H

