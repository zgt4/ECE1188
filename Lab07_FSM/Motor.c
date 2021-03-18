
#include <stdint.h>
#include "msp.h"
#include "../inc/CortexM.h"
#include "../inc/PWM.h"

// *******Lab 13 solution*******

// ------------Motor_Init------------
// Initialize GPIO pins for output, which will be
// used to control the direction of the motors and
// to enable or disable the drivers.
// The motors are initially stopped, the drivers
// are initially powered down, and the PWM speed
// control is uninitialized.
// Input: none
// Output: none
void Motor_Init(void){
  // write this as part of Lab 13

    P2 -> SEL0 &= 0x3F;
    P2 -> SEL1 &= 0x3F;
    P2 -> DIR  |= 0xC0;

    P3 -> SEL0 &= 0x3F;
    P3 -> SEL1 &= 0x3F;
    P3 -> DIR  |= 0xC0;

    P5 -> SEL0 &= 0xCF;
    P5 -> SEL1 &= 0xCF;
    P5 -> DIR  |= 0x30;
}

// ------------Motor_Stop------------
// Stop the motors, power down the drivers, and
// set the PWM speed control to 0% duty cycle.
// Input: none
// Output: none
void Motor_Stop(void){
    P1->OUT &= ~0xC0;
    P2->OUT &= ~0xC0;   // off
    P3->OUT &= ~0xC0;   // low current sleep mode
}

// ------------Motor_Forward------------
// Drive the robot forward by running left and
// right wheels forward with the given duty
// cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 14,998)
//        rightDuty duty cycle of right wheel (0 to 14,998)
// Output: none
// Assumes: Motor_Init() has been called
void Motor_Forward(uint16_t leftDuty, uint16_t rightDuty){ 
    P3 -> OUT |= 0xC0;
    P5 -> OUT &= 0xCF;
    PWM_Duty3(rightDuty); //For right pwm
    PWM_Duty4(leftDuty); //left pwm
}

// ------------Motor_Right------------
// Turn the robot to the right by running the
// left wheel forward and the right wheel
// backward with the given duty cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 14,998)
//        rightDuty duty cycle of right wheel (0 to 14,998)
// Output: none
// Assumes: Motor_Init() has been called
void Motor_Right(uint16_t leftDuty, uint16_t rightDuty){ 
  // write this as part of Lab 13
    P3 -> OUT |= 0xC0; //3.7=1 and 3.6=1
    P5 -> OUT &= 0xEF; //5.5=1 5.4=0
    P5 -> OUT |= 0x20;
    PWM_Duty3(rightDuty); //For right pwm
    PWM_Duty4(leftDuty); //left pwm
}

// ------------Motor_Left------------
// Turn the robot to the left by running the
// left wheel backward and the right wheel
// forward with the given duty cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 14,998)
//        rightDuty duty cycle of right wheel (0 to 14,998)
// Output: none
// Assumes: Motor_Init() has been called
void Motor_Left(uint16_t leftDuty, uint16_t rightDuty){ 
  // write this as part of Lab 13
    P3 -> OUT |= 0xC0; //3.7=1 and 3.6=1
    P5 -> OUT &= 0xDF; //5.5=0 5.4=1
    P5 -> OUT |= 0x10;
    PWM_Duty3(rightDuty); //For right pwm
    PWM_Duty4(leftDuty); //left pwm

}

// ------------Motor_Backward------------
// Drive the robot backward by running left and
// right wheels backward with the given duty
// cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 14,998)
//        rightDuty duty cycle of right wheel (0 to 14,998)
// Output: none
// Assumes: Motor_Init() has been called
void Motor_Backward(uint16_t leftDuty, uint16_t rightDuty){ 
    P3 -> OUT |= 0xC0;
    P5 -> OUT |= 0x30;
    PWM_Duty3(rightDuty); //For right pwm
    PWM_Duty4(leftDuty); //left pwm
}
