#include <ir_sensor/ir_sensor.hpp>

float IrSensor::get_range() {
  int sample = 0;
  // Get data
  //sample = analogRead(pin_num)/4;
  // if the ADC reading is too low, 
  //   then we are really far away from anything
  if (sample < 10) {
    return 254;     // max range
  }
  // Magic numbers to get cm
  sample = 1309 / (sample - 3);
  return (sample - 1)/100; //convert to meters
}

void IrSensor::publish_range() {
  /// TODO: pass the port number as agument to get_range()
  range_msg_left_.range = get_range();
  range_msg_center_.range = get_range();
  range_msg_right_.range = get_range();

  range_msg_left_.header.stamp = ros::Time::now();
  range_msg_center_.header.stamp = ros::Time::now();
  range_msg_right_.header.stamp = ros::Time::now();

  /// publish the readings from all three IR sesors
  pub_range_left_.publish(range_msg_left_);
  pub_range_center_.publish(range_msg_center_);
  pub_range_right_.publish(range_msg_right_);
}

