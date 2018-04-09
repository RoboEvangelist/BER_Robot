/* 
 * rosserial IR Ranger Example  
 * 
 * This example is calibrated for the Sharp GP2D120XJ00F.
 */

#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>

ros::NodeHandle  nh;

/// left IR sensor
sensor_msgs::Range left_range_msg;
ros::Publisher left_pub_range("left_range_data", &left_range_msg);
/// left IR sensor
sensor_msgs::Range center_range_msg;
ros::Publisher center_pub_range("center_range_data", &center_range_msg);
/// right IR sensor
sensor_msgs::Range right_range_msg;
ros::Publisher right_pub_range("right_range_data", &right_range_msg);

const int left_analog_pin = 2;    /**< robot's left IR sensor */
const int center_analog_pin = 1;  /**< center IR sensor */
const int right_analog_pin = 0;   /**< robot's right IR sensor */
unsigned long range_timer;

/*
 * getRange() - samples the analog input from the ranger
 * and converts it into meters.  
 */
float getRange(int pin_num){
    int sample;
    // Get data
    sample = analogRead(pin_num)/4;
    // if the ADC reading is too low, 
    //   then we are really far away from anything
    if(sample < 10)
        return 254;     // max range
    // Magic numbers to get cm
    sample= 1309/(sample-3);
    return (sample - 1)/100; //convert to meters
}

char left_frameid[] = "/ir_left_ranger";
char center_frameid[] = "/ir_center_ranger";
char right_frameid[] = "/ir_right_ranger";

void setup()
{
  nh.initNode();
  nh.advertise(left_pub_range);
  nh.advertise(center_pub_range);
  nh.advertise(right_pub_range);
  
  /// Sharp IR Range Finder 2Y0A21
  left_range_msg.radiation_type = sensor_msgs::Range::INFRARED;
  left_range_msg.header.frame_id = left_frameid;
  left_range_msg.field_of_view = 0.01;
  left_range_msg.min_range = 0.1;
  left_range_msg.max_range = 0.8;

  /// Sharp IR Ranger, Model# GP2D120XJ00F
  center_range_msg.radiation_type = sensor_msgs::Range::INFRARED;
  center_range_msg.header.frame_id = center_frameid;
  center_range_msg.field_of_view = 0.01;
  /// range in meters. See specsheet for values
  center_range_msg.min_range = 0.04;
  center_range_msg.max_range = 0.3;

  /// Sharp IR Range Finder 2Y0A21
  right_range_msg = left_range_msg;
  right_range_msg.header.frame_id = right_frameid;
}

void loop()
{
  // publish the range value every 50 milliseconds
  //   since it takes that long for the sensor to stabilize
  if ( (millis()-range_timer) > 50){
    left_range_msg.range = getRange(left_analog_pin);
    left_range_msg.header.stamp = nh.now();
    left_pub_range.publish(&left_range_msg);

    /// Sharp IR Ranger, Model# GP2D120XJ00F
    center_range_msg.range = getRange(center_analog_pin);
    center_range_msg.header.stamp = nh.now();
    center_pub_range.publish(&center_range_msg);

    right_range_msg.range = getRange(right_analog_pin);
    right_range_msg.header.stamp = nh.now();
    right_pub_range.publish(&right_range_msg);
    
    range_timer =  millis() + 50;
  }
  nh.spinOnce();
}

