// Lab10_Debugmain.c
// Runs on MSP432
// Student version to Debug lab
// Daniel and Jonathan Valvano
// September 4, 2017
// Interrupt interface for QTRX reflectance sensor array
// Pololu part number 3672.
// Debugging dump, and Flash black box recorder

/* This example accompanies the book
   "Embedded Systems: Introduction to Robotics,
   Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2019
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Simplified BSD License (FreeBSD License)
Copyright (c) 2019, Jonathan Valvano, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are
those of the authors and should not be interpreted as representing official
policies, either expressed or implied, of the FreeBSD Project.
*/
// Negative logic bump sensors
// P4.7 Bump5, left side of robot
// P4.6 Bump4
// P4.5 Bump3
// P4.3 Bump2
// P4.2 Bump1
// P4.0 Bump0, right side of robot

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

#include "msp.h"
#include "..\inc\bump.h"
#include "..\inc\Reflectance.h"
#include "..\inc\Clock.h"
#include "..\inc\SysTickInts.h"
#include "..\inc\CortexM.h"
#include "..\inc\LaunchPad.h"
#include "..\inc\FlashProgram.h"

#define FLASH_BANK1_MIN     0x00020000  // Flash Bank1 minimum address
#define FLASH_BANK1_MAX     0x0003FFFF  // Flash Bank1 maximum address


void Debug_Init(void){
  // write this as part of Lab 10
    //Initialize the bump sensors
    // 0 2 3 5 6 7 all need to be initialized
    P4->SEL0 &= ~0xED;
    P4->SEL1 &= ~0xED;
    P4->DIR &= ~0xED;
    P4->REN |= 0xED;

    //Illuminate the LEDs on P5.3 and P9.2
    //Make P5.3 an output
    P5->SEL0 &= 0xF7; //1111 0111
    P5->SEL1 &= 0xF7;
    P5->DIR |= 0x08; //0000 1000
    P5->OUT &= 0xF7;
    //Make P9.2 an output
    P9->SEL0 &= 0xFB; //1111 1011
    P9->SEL1 &= 0xFB;
    P9->DIR |= 0x04;
    P9->OUT &= 0xFB;
    //Initialize the reflectance sensors
    P7->SEL0 &= 0x00;
    P7->SEL1 &= 0x00;
    P7->DIR &= 0x00;

}
void Debug_Dump(uint8_t x, uint8_t y){
  // write this as part of Lab 10
    static uint8_t bumpSensors;
    static uint8_t LEDSensors;
    uint8_t result;


    //Capture the LED sensors current data
    P5->OUT |= 0x08;      //Set P5.3 and P9.2 high
    P9->OUT |= 0x04;

    P7->DIR |= 0xFF;       // make P7.7-P7.0 out and pulse high for 10 us
    P7->OUT |= 0xFF;
    P7->REN |= 0xFF;      //Enable the pull up resistor
    Clock_Delay1us(10);   // wait 10 us

    P7->REN &= 0x00;
    P7->DIR &= 0x00;       //Make the sensor pins input
    Clock_Delay1us(1000); //Wait t us

    result = P7->IN;     //Read the inputs
    P5->OUT &= 0xF7;      //Set P5.3 and P9.2 low
    P9->OUT &= 0xFB;

    bumpSensors = P4->IN & 0xED;
    LEDSensors = result;
}
void Debug_FlashInit(void){ 
  // write this as part of Lab 10
    Flash_Init(48);
    uint32_t address;
    address = FLASH_BANK1_MIN;

    while(address < FLASH_BANK1_MAX)
    {
        Flash_Erase(address);
        address = address + 64;
    }
}
void Debug_FlashRecord(uint16_t *pt){
  // write this as part of Lab 10
    uint32_t address;
    address = FLASH_BANK1_MIN;
    while(*(uint32_t*)address != 0xFFFFFFFF){ // find first free block
      address = address + 64;
      if(address > FLASH_BANK1_MAX) //Flash is full
      {
          return;
      }
    }
    Flash_FastWrite((uint32_t *)pt, address, 16); //Write 64 bytes
}
void SysTick_Handler(void){ // every 1ms
  // write this as part of Lab 10
}

int Program10_0(void){
  // write this as part of Lab 10
    Bump_Init();
    EnableInterrupts();
  while(1){
  // write this as part of Lab 10
      //WaitForInterrupt();
      Bump_Read();
  }
}

int Program10_1(void){
  uint8_t data=0;
  Clock_Init48MHz();
  Debug_Init();
  LaunchPad_Init();
  while(1){
    P1->OUT |= 0x01;
    Debug_Dump(data,data+1);// linear sequence
    P1->OUT &= ~0x01;
    data=data+2;
  }
}


// Driver test
#define SIZE 256  // feel free to adjust the size
uint16_t Buffer[SIZE];
int main(void){ uint16_t i;
  Clock_Init48MHz();
  LaunchPad_Init(); // built-in switches and LEDs
  for(i=0;i<SIZE;i++){
    Buffer[i] = (i<<8)+(255-i); // test data
  }
  i = 0;
  while(1){
    P1->OUT |= 0x01;
    Debug_FlashInit();
    P1->OUT &= ~0x01;
    P2->OUT |= 0x01;
    Debug_FlashRecord(Buffer); // 114us
    P2->OUT &= ~0x01;
    i++;
  }
}


int Program10_3(void){ uint16_t i;
  Clock_Init48MHz();
  LaunchPad_Init(); // built-in switches and LEDs
  for(i=0;i<SIZE;i++){
    Buffer[i] = (i<<8)+(255-i); // test data
  }
  P1->OUT |= 0x01;
  Debug_FlashInit();
  P1->OUT &= ~0x01;
  i = 0;
  while(1){
    P2->OUT |= 0x01;
    Debug_FlashRecord(Buffer);
    P2->OUT &= ~0x01;
    i++;
  }
}

/*
uint8_t Buffer[1000];
uint32_t I=0;
uint8_t *pt;
void DumpI(uint8_t x){
  if(I<1000){
    Buffer[I]=x;
    I++;
  }
}
void DumpPt(uint8_t x){
  if(pt<&Buffer[1000]){
    *pt=x;
    pt++;
  }
}
void Activity(void){
  DumpI(5);
  DumpI(6);
  pt = Buffer;
  DumpPt(7);
  DumpPt(8);

}
*/
