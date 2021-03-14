
#include "msp.h"
#include "..\inc\bump.h"
#include "..\inc\Clock.h"
#include "..\inc\SysTick.h"
#include "..\inc\CortexM.h"
#include "..\inc\LaunchPad.h"
#include "..\inc\Motor.h"
#include "..\inc\TimerA1.h"
#include "..\inc\TExaS.h"
#include "../inc/PWM.h"

// Driver test
void TimedPause(uint32_t time){
  Clock_Delay1ms(time);          // run for a while and stop
  Motor_Stop();
  while(LaunchPad_Input()==0);  // wait for touch
  while(LaunchPad_Input());     // wait for release
}


int main(void){

    Clock_Init48MHz();
      LaunchPad_Init(); // built-in switches and LEDs
      Bump_Init();      // bump switches
      Motor_Init();     // your function
      TExaS_Init(LOGICANALYZER_P2);
      PWM_Init34(14999,0,0);

      while(1){
        TimedPause(4000);
        Motor_Forward(7500,7500);  // your function
        TimedPause(2000);
        Motor_Backward(14000,14000); // your function
        TimedPause(3000);
        Motor_Left(5000,5000);     // your function
        TimedPause(3000);
        Motor_Right(5000,5000);    // your function
      }
}
