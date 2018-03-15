#include <motor_control/motor_control.hpp>

void MotorControl::velocityCallback(
    const geometry_msgs::Twist::ConstPtr& vel_array)
{
    // only angualr z is necessary here?                     
    float omega = vel_array->angular.z;
    // Speed is enough (no direction) since omega will fix heading
    float velocity = vel_array->linear.x;

    /*** convert from m/s to rev/s to VT ***/
    //right wheel                                             
//    cmd_vel[0] = 0;
//    //left wheel
//    cmd_vel[1] = 0;
}

