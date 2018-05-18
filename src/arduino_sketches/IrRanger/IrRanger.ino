/* 
 * rosserial IR Ranger Example  
 * 
 * This example is calibrated for the Sharp GP2D120XJ00F.
 */

#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>
#include <std_msgs/Int8.h>

char left_frameid[] = "/ir_left_ranger";
char center_frameid[] = "/ir_center_ranger";
char right_frameid[] = "/ir_right_ranger";

int8_t str_back = -1;
int8_t str_forward = 1;
int8_t str_left = 2 ;
int8_t str_right = 3;
std_msgs::Int8 int_msg;

ros::NodeHandle nh;

/// left IR sensor
sensor_msgs::Range left_range_msg;
ros::Publisher left_pub_range("left_range_data", &left_range_msg);
/// left IR sensor
sensor_msgs::Range center_range_msg;
ros::Publisher center_pub_range("center_range_data", &center_range_msg);
/// right IR sensor
sensor_msgs::Range right_range_msg;
ros::Publisher right_pub_range("right_range_data", &right_range_msg);

ros::Publisher chatter("chatter", &int_msg);

const uint8_t left_analog_pin = 2;    /**< robot's left IR sensor */
const uint8_t center_analog_pin = 1;  /**< center IR sensor */
const uint8_t right_analog_pin = 0;   /**< robot's right IR sensor */
const uint16_t time_delta = 1000;
const uint16_t left_right_max_analog = 650; /**< 650 equivalent to 10cm */
const uint16_t left_right_min_analog = 300;  /**< minimum save analog reading */
const uint16_t center_max_analog = 650; /**< 650 equivalent to 4cm */
const uint8_t center_min_analog = 100;  /**< minimum safe analog reading before avoiding an obstacle */

/// pins control the motor controller
const uint8_t motor_conroller_one_pin = 5;
const uint8_t motor_conroller_two_pin = 6;

/// if you do 100 and 100, then the car stops
const uint8_t pin_one_motor_speed = 100;  /**< Keep this one constant to control max speed */
int pin_two_motor_speed = 0;          /**< switch this one between 100 and 200 for slow speed */
const uint8_t kMaxVelForward = 165;
const uint8_t kMinVelBackward = 40;

const uint8_t car_turn_one_pin = 9;
const uint8_t car_turn_two_pin = 10;
uint8_t pin_one_turn_speed = 0;
int pin_two_turn_speed = 0;
unsigned long range_timer;

/// check if there are objects too close before car is ordered to moves
bool object_on_left_state = false;
bool object_on_right_state = false;
bool object_on_center_state = false;

void moveCarForward() {
  analogWrite(motor_conroller_one_pin, pin_one_motor_speed);
  analogWrite(motor_conroller_two_pin, kMaxVelForward);
}

void moveCarBackward() {
  analogWrite(motor_conroller_one_pin, pin_one_motor_speed);
  analogWrite(motor_conroller_two_pin, kMinVelBackward);
}

void turnCarLeft() {
  analogWrite(car_turn_one_pin, 0);
  ///in this pin 100 = complete left turn, 200 = wheels straigth 
  analogWrite(car_turn_two_pin, 110);
}

void turnCarRight() {
  analogWrite(car_turn_one_pin, 110);
  ///in this pin 100 = complete left turn, 200 = wheels straigth 
  analogWrite(car_turn_two_pin, 0);
}

void wheelsStraight() {
  analogWrite(car_turn_one_pin, 255);
  ///in this pin 100 = complete left turn, 255 = wheels straigth 
  analogWrite(car_turn_two_pin, 255);
}

void stopCar() {
  analogWrite(motor_conroller_one_pin, pin_one_motor_speed);
  analogWrite(motor_conroller_two_pin, 100);
  wheelsStraight();
}

void messageCb(const std_msgs::Int8& msg){
  int_msg = msg;
}

ros::Subscriber<std_msgs::Int8> sub("/car_direction", &messageCb);

/*
 * getRange() - samples the analog input from the ranger
 * and converts it into meters.  
 */
float getRange(int pin_num){
    uint16_t sample;
    // Get data
    sample = analogRead(pin_num);
    return sample;
}

void setup()
{
  int_msg.data = 0;
  /// set pwm pins as output
  pinMode(motor_conroller_one_pin, OUTPUT);
  pinMode(motor_conroller_two_pin, OUTPUT);
  stopCar();
  
  pinMode(car_turn_one_pin, OUTPUT);
  pinMode(car_turn_two_pin, OUTPUT);
  //wheelsStraight();

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

  /// subscriber to car move topic
  nh.subscribe(sub);
  nh.advertise(chatter);
}

void loop()
{
  /// lets add sleep to avoid crashing the arduino
  delay(50);
  /*
  left_range_msg.range = getRange(left_analog_pin);
  left_range_msg.header.stamp = nh.now();
  left_pub_range.publish(&left_range_msg);
  right_range_msg.range = getRange(right_analog_pin);
  right_range_msg.header.stamp = nh.now();
  right_pub_range.publish(&right_range_msg);

  if ((right_range_msg.range > 250) && (left_range_msg.range > 250)) {
    /// check if there are objects too close on the left and right
    object_on_left_state = true;
    object_on_right_state = true;
  }
  else if ((left_range_msg.range > 250) && (right_range_msg.range <= 250)) {
    /// check if there are objects too close on the left
    turnCarRight();
    object_on_left_state = true;
  }
  else if ((left_range_msg.range <= 250) && (right_range_msg.range > 250)) {
    /// check if there are objects too close on the left
    turnCarLeft();
    object_on_right_state = true;
  }
  else {
    /// left and right side are clear
    object_on_left_state = false;
    object_on_right_state = false;
    wheelsStraight();
  }

  /// Sharp IR Ranger, Model# GP2D120XJ00F
  center_range_msg.range = getRange(center_analog_pin);
  center_range_msg.header.stamp = nh.now();
  center_pub_range.publish(&center_range_msg);
  if (center_range_msg.range > 300) {
    moveCarBackward();
    object_on_center_state = true;
  }
  else if (center_range_msg.range < 200) {
    /// car is clear to move forward if necessary
    object_on_center_state = false;
  }
  else {
    /// TODO: stop car only if object no longer too close, but don't interrupt the
    /// other functions that need to move the car
    stopCar();
    object_on_center_state = false;
  }
  */

  /// give priority to proximity sensors
  //if ((object_on_left_state == false) &&
  //  (object_on_right_state == false) && (object_on_center_state == false))
  //{
  if (int_msg.data == str_right) {
    turnCarRight();
    moveCarForward();
  }
  else if (int_msg.data == str_left) {
    turnCarLeft();
    moveCarForward();
  }
  else if (int_msg.data == str_back) {
    moveCarBackward();
  }
  else if (int_msg.data == str_forward) {
    moveCarForward();
    wheelsStraight();
  }
  else {
    stopCar();
  }
  chatter.publish(&int_msg);
  //}
    
  //stopCar();
  nh.spinOnce();
}

