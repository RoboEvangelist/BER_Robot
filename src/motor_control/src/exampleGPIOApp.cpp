// exampleApp.c

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

#include <motor_control/jetsonGPIO.h>

#include <ros/ros.h>
#include <tf/transform_listener.h>

int main(int argc, char** argv) {

  ros::init(argc, argv, "motor_control");

  std::cout << "Testing the GPIO Pins" << std::endl;

  /// Thse pins are both, I/O, and PWM pins on Jetson Tk1
  //jetsonGPIO redLED = gpio165;       ///< gpio165 from Jetson J3A2 pin 55 pwm2
  //jetsonGPIO pushButton = gpio166;   ///< gpio166 from Jetson J3A2 pin 58 pwm3
  
  /// Jetson Tx2 pins
  jetsonTX1GPIONumber redLED = gpio219 ;     ///< pin 29 in J21
  jetsonTX1GPIONumber pushButton = gpio187 ;  ///< pin 13 in J21

  // Make the button and led available in user space
  gpioExport(pushButton) ;
  gpioExport(redLED) ;
  gpioSetDirection(pushButton, outputPin) ;
  gpioSetDirection(redLED, outputPin) ;

  unsigned int micro_sec_delay = 15000;
  usleep(3000000);         // off for 200ms
  // Flash the LED 5 times
  for (unsigned char i = 0; i < 200; i++) {
      /// Move car backguards
      gpioSetValue(redLED, on);
      usleep(micro_sec_delay);         // on for 200ms
      gpioSetValue(redLED, off);
      usleep(micro_sec_delay);         // off for 200ms
      /// Move car forwards
      gpioSetValue(redLED, on);
      usleep(micro_sec_delay);         // on for 200ms
      //gpioSetValue(pushButton, off);
      gpioSetValue(redLED, off);
      usleep(micro_sec_delay);         // off for 200ms
  }

  std::cout << "GPIO example finished." << std::endl;
  gpioUnexport(redLED);     // unexport the LED
  gpioExport(pushButton);      // unexport the push button

  ros::spin();
  return 0;
}

