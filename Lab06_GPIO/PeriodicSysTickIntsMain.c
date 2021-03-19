// built-in LED1 connected to P1.0
// P1.0, P2.0 are an output to profiling scope/logic analyzer

#include <stdint.h>
#include "msp.h"
#include "..\inc\CortexM.h"
#include "..\inc\SysTickInts.h"
#include "..\inc\LaunchPad.h"
#include "..\inc\Clock.h"
#include "..\inc\Reflectance.h"

volatile uint32_t Time,MainCount;
#define LEDOUT (*((volatile uint8_t *)(0x42098040)))

uint8_t z = 0 , pos;



void main(void){
  Clock_Init48MHz();      // running on crystal
  SysTick_Init(48000 , 0);  // set up SysTick for 8 Hz interrupts
  EnableInterrupts();
  Reflectance_Init();
  while(1){
    WaitForInterrupt();
  }
}

//global variables used: pos, z = 0;
void SysTick_Handler(void){
    //every 10 interrupts, Reflectance_Start
    //next interrupt, Reflectance_End and send result to global variable
    //do nothing for 8 interrupts
    if(z % 10 == 0){ //every 10 interrupts, Reflectance_Start
        Reflectance_Start();
    }else if(z % 10 == 1){ //next interrupt, Reflectance_End and send result to global variable
        pos = Reflectance_End();
        //read bump sensors
    }else{ //do nothing for 8 interrupts

    }

    z++;
    if(z == 10)
        z = 0;
}

