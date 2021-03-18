// Lab07_FSMmain.c
// Runs on MSP432
// Student version of FSM lab, FSM with 2 inputs and 2 outputs.
// Rather than real sensors and motors, it uses LaunchPad I/O
// Daniel and Jonathan Valvano
// July 11, 2019

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

#include <stdint.h>
#include "msp.h"
#include "../inc/clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/Texas.h"
#include "../inc/CortexM.h"
#include "../inc/Motor.h"
#include "../inc/BumpInt.h"
#include "../inc/TimerA1.h"
#include "../inc/FlashProgram.h"
#include "../inc/PWM.h"


//#include "../inc/BumpInt.c"

//#include "../inc/Motor.c"
//#include "../inc/CortexM.c"


//this has to go when I have Alex's part
/*(Left,Right) Motors, call LaunchPad_Output (positive logic)
3   1,1     both motors, yellow means go straight
2   1,0     left motor,  green  means turns right
1   0,1     right motor, red    means turn left
0   0,0     both off,    dark   means stop
(Left,Right) Sensors, call LaunchPad_Input (positive logic)
3   1,1     both buttons pushed means on line,
2   1,0     SW2 pushed          means off to right
1   0,1     SW1 pushed          means off to left
0   0,0     neither button      means lost
 */

// Linked data structure
struct State {
  uint32_t s;
  uint32_t leftMotorDutyCycle;//duty cycles can range from 0 - 14998 0 is off
  uint32_t rightMotorDutyCycle;
  const struct State *next[4]; // Next if 2-bit input is 0-3
};
typedef const struct State State_t;

#define Center      &fsm[0]
#define Right1      &fsm[1]
#define Right2      &fsm[2]
#define WayRight1   &fsm[3]
#define WayRight2   &fsm[4]
#define Lost        &fsm[5]
#define Left1       &fsm[6]
#define Left2       &fsm[7]
#define WayLeft1    &fsm[8]
#define WayLeft2    &fsm[9]

//// student starter code

State_t fsm[10]={
  {0, 7500, 7500, {Lost , Left1,   Right1,  Center }},  // Center
  {1, 5000, 7500, {WayRight1, Lost, Right2, Center}},   // Right1
  {2, 7500, 7500, {WayRight1, Lost, Right1, Center}},   // Right2
  {3, 5000, 7500, {WayRight2, Lost, Right2, Lost}},     //WayRight1
  {4, 7500, 7500, {Lost, Lost, Right1, Center}},        //WayRight2
  {5,    0,    0, {Lost, Lost, Lost, Center}},          //Lost
  {6, 7500, 5000, {WayLeft1, Left2, Lost, Center}},     //Left1
  {7, 7500, 7500, {WayLeft1, Left1, Lost, Center}},  //Left2
  {8, 7500, 5000, {WayLeft2, Left2, Lost, Lost}},  //WayLeft1
  {9, 7500, 7500, {Lost, Left1, Lost, Center}}    //WayLeft2
};

State_t *Spt;  // pointer to the current state
uint32_t Input;
uint32_t Output;
uint32_t leftMotor;
uint32_t rightMotor;

/*Run FSM continuously
1) Output depends on State (LaunchPad LED)
2) Wait depends on State
3) Input (LaunchPad buttons)
4) Next depends on (Input,State)
 */

uint8_t CollisionData, CollisionFlag;  // mailbox
void HandleCollision(uint8_t bumpSensor){

   Motor_Stop(); // lab 13 Version
   //Motor_StopSimp(); // will modify
   CollisionData = bumpSensor;
   CollisionFlag = 1;
  /* LaunchPad_Output(0x01); //debug light
   //these are going to Modify once Alex R is done w/ his module
   Motor_BackwardSimp(3000, 200);
   Motor_LeftSimp(3000, 100);
   LaunchPad_Output(0x00); //debug light off
   */
}
int main(void){
  Clock_Init48MHz();
  LaunchPad_Init();
  TExaS_Init(LOGICANALYZER);  // optional
  Motor_Init();
  PWM_Init34(14999, 0, 0);
  Spt = Center;
  BumpInt_Init(&HandleCollision);
  EnableInterrupts();
  while(1){

    WaitForInterrupt();
    leftMotor = Spt->leftMotorDutyCycle;            // set output from FSM
    rightMotor = Spt->rightMotorDutyCycle;
    //LaunchPad_Output(Output);     // do output to two motors
    Motor_Forward(leftMotor,rightMotor); //(left wheel, right wheel)

   // TExaS_Set(Input<<2|Output);   // optional, send data to logic analyzer
    //Clock_Delay1ms(Spt->delay);   // wait
    Input = LaunchPad_Input();    // read sensors
    Spt = Spt->next[Input];       // next depends on input and state
  }
}

// Color    LED(s) Port2
// dark     ---    0
// red      R--    0x01
// blue     --B    0x04
// green    -G-    0x02
// yellow   RG-    0x03
// sky blue -GB    0x06
// white    RGB    0x07
// pink     R-B    0x05
