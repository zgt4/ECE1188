// reflectance even LED illuminate connected to P5.3
// reflectance odd LED illuminate connected to P9.2
// reflectance sensor 1 connected to P7.0 (robot's right, robot off road to left)
// reflectance sensor 2 connected to P7.1
// reflectance sensor 3 connected to P7.2
// reflectance sensor 4 connected to P7.3 center
// reflectance sensor 5 connected to P7.4 center
// reflectance sensor 6 connected to P7.5
// reflectance sensor 7 connected to P7.6
// reflectance sensor 8 connected to P7.7 (robot's left, robot off road to right)

#include <stdint.h>
#include "msp432.h"
#include "..\inc\Clock.h"


// ------------Reflectance_Init------------
// Initialize the GPIO pins associated with the QTR-8RC
// reflectance sensor.  Infrared illumination LEDs are
// initially off.
// Input: none
// Output: none
void Reflectance_Init(void){
    Clock_Init48MHz();

    //initialize P5.3 as output and set it low
    P5->SEL0 &= 0xF7;
    P5->SEL0 &= 0xF7;
    P5->DIR |= 0x08;
    P5->OUT &= 0xF7;

    //initialize P9.2 as output and set it low
    P9->SEL0 &= 0xFB;
    P9->SEL0 &= 0xFB;
    P9->DIR |= 0x04;
    P9->OUT &= 0xFB;

    //initialize P7.0-P7.7 as inputs
    P7->SEL0 = 0x00;
    P7->SEL0 = 0x00;
    P7->DIR = 0x00;

}

// ------------Reflectance_Read------------
// Read the eight sensors
// Turn on the 8 IR LEDs
// Pulse the 8 sensors high for 10 us
// Make the sensor pins input
// wait t us
// Read sensors
// Turn off the 8 IR LEDs
// Input: time to wait in usec
// Output: sensor readings
// Assumes: Reflectance_Init() has been called
uint8_t Reflectance_Read(uint32_t time){
    uint8_t result;

    P5->OUT |= 0x08; //turn on IR LEDs
    P9->OUT |= 0x04;

    //make P7.0-P7.7 outputs and pulse for 10us then make inputs again
    P7->DIR = 0xFF;
    P7->OUT = 0xFF;
    Clock_Delay1us(10);
    P7->DIR = 0x00;

    //wait so you can differentiate white and black
    Clock_Delay1us(time);

    result = P7->IN;
    P5->OUT &= 0xF7; //turn off LEDs
    P9->OUT &= 0xFB;

    return result;
}

// ------------Reflectance_Center------------
// Read the two center sensors
// Turn on the 8 IR LEDs
// Pulse the 8 sensors high for 10 us
// Make the sensor pins input
// wait t us
// Read sensors
// Turn off the 8 IR LEDs
// Input: time to wait in usec
// Output: 0 (off road), 1 off to left, 2 off to right, 3 on road
// (Left,Right) Sensors
// 1,1          both sensors   on line
// 0,1          just right     off to left
// 1,0          left left      off to right
// 0,0          neither        lost
// Assumes: Reflectance_Init() has been called
uint8_t Reflectance_Center(uint32_t time){
    // write this as part of Lab 6
  return 0; // replace this line
}


// Perform sensor integration
// Input: data is 8-bit result from line sensor
// Output: position in 2 bits...00 is lost...01 is to the left of the line...11 is centered on the line...10 is to the right of the line
uint8_t Reflectance_Position(uint8_t data){

    int8_t position=0;
    if(data >= 0x20 && data != 0x30){
        position = 2; //10 robot is to the right of the line
    }else if(data <= 8 && data > 0){
        position = 1; //01 robot is to the left of the line
    }else if(data == 0){
        position = 0; //00 robot is off the line
    }else{
        position = 3; //11 robot is centered on the line
    }

    return position;
}


/*
// Perform sensor integration
// Input: data is 8-bit result from line sensor
// Output: position in 0.1mm relative to center of line
int W[8] = {334, 238, 142, 48, -48, -142, -238, -334};
int32_t Reflectance_Position(uint8_t data){

    int32_t position=0;
    int n=0;
    for(uint8_t i=0; i<8; i++){
        position += W[i]*(data & 0x01);
        n += (data & 0x01);
        data = data >> 1;
    }

    position = position/n;

    return position;
}
*/


// ------------Reflectance_Start------------
// Begin the process of reading the eight sensors
// Turn on the 8 IR LEDs
// Pulse the 8 sensors high for 10 us
// Make the sensor pins input
// Input: none
// Output: none
// Assumes: Reflectance_Init() has been called
void Reflectance_Start(void){
    // write this as part of Lab 10
}


// ------------Reflectance_End------------
// Finish reading the eight sensors
// Read sensors
// Turn off the 8 IR LEDs
// Input: none
// Output: sensor readings
// Assumes: Reflectance_Init() has been called
// Assumes: Reflectance_Start() was called 1 ms ago
uint8_t Reflectance_End(void){
    // write this as part of Lab 10
 return 0; // replace this line
}


