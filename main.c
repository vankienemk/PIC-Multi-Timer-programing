#include "main.h"
#include "multiTimer.h"

#int_TIMER1

void  TIMER1_isr(void) 
{
   runTimerMng();
   
}

//main 
void main()
{
   //pre setup multi timer
   preSetupTimer();
   
   //set input output
   set_tris_c(0x00);
   set_tris_b(0x00);
   OUTPUT_c(0x00);
   
   //define timer
   int timer1S,timer2S,timer3S,timer4S,timer5S;
   setupTimer(&timer1S,TOGDT,100,1);
   setupTimer(&timer2S,TOGDT,200,1);
   setupTimer(&timer3S,TOGDT,300,1);
   setupTimer(&timer4S,TOGDT,400,1);
   setupTimer(&timer5S,TOGDT,300,1);
   
   //enable timer
   enableTimer(&timer1S);
   enableTimer(&timer2S);
   enableTimer(&timer3S);
   enableTimer(&timer4S);
   enableTimer(&timer5S);
   
   //forever loop
   while(true){
      if(timer1S)output_high(PIN_C0);
      else output_low(PIN_C0);
      if(timer2S) output_high(PIN_C1);
      else output_low(PIN_C1);
      if(timer3S) output_high(pin_C2);
      else output_low(PIN_C2);
      if(timer4S) output_high(pin_C3);
      else output_low(PIN_C3);
      if(timer5S) output_high(pin_C4);
      else output_low(PIN_C4);
   }
}

