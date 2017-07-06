// exampleApp.c

#include <iostream>
#include <string>
#include <unistd.h>
#include "jetsonGPIO.h"

int main(int argc, char *argv[]){

    std::cout << "Testing the GPIO Pins" << std::endl;

    jetsonGPIO redLED = gpio165;       ///< gpio165 from Jetson J3A2 pin 55
    jetsonGPIO pushButton = gpio166;   ///< gpio166 from Jetson J3A2 pin 58
    // Make the button and led available in user space
    gpioExport(pushButton) ;
    gpioExport(redLED) ;
    gpioSetDirection(pushButton, outputPin) ;
    gpioSetDirection(redLED, outputPin) ;
    //gpioSetValue(redLED, on);
    // Reverse the button wiring; this is for when the button is wired
    // with a pull up resistor
    // gpioActiveLow(pushButton, true);


    unsigned int micro_sec_delay = 1000000;
    //gpioSetValue(redLED, on);
    usleep(3000000);         // off for 200ms
    // Flash the LED 5 times
    for (unsigned char i = 0; i < 5; i++) {
        /// Move car backguards
        std::cout << "Move Car Backguard" << std::endl;
        gpioSetValue(redLED, on);
        usleep(micro_sec_delay);         // on for 200ms
        std::cout << "Stop Car" << std::endl;
        gpioSetValue(redLED, off);
        usleep(micro_sec_delay);         // off for 200ms
        /// Move car forwards
        std::cout << "Move Car Forward" << std::endl;
        gpioSetValue(pushButton, on);
        usleep(micro_sec_delay);         // on for 200ms
        std::cout << "Stop Car" << std::endl;
        gpioSetValue(pushButton, off);
        usleep(micro_sec_delay);         // off for 200ms
    }

//    // Wait for the push button to be pressed
//    std::cout << "Please press the button!" << std::endl;
//
//    unsigned int value = low;
//    int ledValue = low ;
//    // Turn off the LED
//    gpioSetValue(redLED,low) ;
//    for (int i = 0 ; i < 10000 ; i++) {
//        gpioGetValue(pushButton, &value) ;
//        // Useful for debugging
//        // std::cout << "Button " << value << std::endl;
//        if (value==high && ledValue != high) {
//            // button is pressed ; turn the LED on
//            ledValue = high ;
//            gpioSetValue(redLED,on) ;
//        } else {
//            // button is *not* pressed ; turn the LED off
//            if (ledValue != low) {
//                ledValue = low ;
//                gpioSetValue(redLED,off) ;
//            }
//
//        }
//        usleep(1000); // sleep for a millisecond
//    }

    std::cout << "GPIO example finished." << std::endl;
    gpioUnexport(redLED);     // unexport the LED
    gpioExport(pushButton);      // unexport the push button
    return 0;
}
