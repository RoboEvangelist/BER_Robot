// exampleApp.c

#include <iostream>
#include <string>
#include <unistd.h>
#include "jetsonGPIO.h"

int main(int argc, char *argv[]){

    std::cout << "Testing the GPIO Pins" << std::endl;

    /// Thse pins are both, I/O, and PWM pins
    jetsonGPIO redLED = gpio165;       ///< gpio165 from Jetson J3A2 pin 55 pwm2
    jetsonGPIO pushButton = gpio166;   ///< gpio166 from Jetson J3A2 pin 58 pwm3

    // Make the button and led available in user space
    gpioExport(pushButton) ;
    gpioExport(redLED) ;
    gpioSetDirection(pushButton, outputPin) ;
    gpioSetDirection(redLED, outputPin) ;

    unsigned int micro_sec_delay = 1000;
    usleep(3000000);         // off for 200ms
    // Flash the LED 5 times
    for (unsigned char i = 0; i < 200; i++) {
        /// Move car backguards
        //std::cout << "Move Car Forward" << std::endl;
        gpioSetValue(redLED, on);
        usleep(micro_sec_delay);         // on for 200ms
        //std::cout << "Stop Car" << std::endl;
        gpioSetValue(redLED, off);
        usleep(micro_sec_delay);         // off for 200ms
        /// Move car forwards
        //std::cout << "Move Car Backward" << std::endl;
        //gpioSetValue(pushButton, on);
        gpioSetValue(redLED, on);
        usleep(micro_sec_delay);         // on for 200ms
        //std::cout << "Stop Car" << std::endl;
        //gpioSetValue(pushButton, off);
        gpioSetValue(redLED, off);
        usleep(micro_sec_delay);         // off for 200ms
    }

    std::cout << "GPIO example finished." << std::endl;
    gpioUnexport(redLED);     // unexport the LED
    gpioExport(pushButton);      // unexport the push button
    return 0;
}

