// exampleApp.c

#include <ir_sensor/ir_sensor.hpp>

int main(int argc, char** argv) {

  ros::init(argc, argv, "ir_sensor");
  std::cout << "testing IR sensors" << std::endl;

  /// initialize motor control object
  IrSensor ir_sensor(1);

  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    ir_sensor.publish_range();
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}

