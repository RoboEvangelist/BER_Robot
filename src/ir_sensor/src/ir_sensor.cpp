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
  //range_msg_.header.stamp = nh_.now();
  range_msg_.range = get_range();
  pub_range_.publish(range_msg_);
}

