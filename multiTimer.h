/***********************************PRECAUTION*********************************/
/*          This library was made by KienNV ©Chavaco.ltd, 2020                */
/*          This is a free library, feel free to use                          */
/* Follow folowing step to begin using this library                           */
/* Step1: include this library into your main.c     ex: #include "multiTimer.h*/
/* Step2: define a timer 1 interrupt routine like this                        */
/*                                                                            */
/*                void  TIMER1_isr(void)                                      */
/*                      {                                                     */
/*                         runTimerMng();                                     */
/*                                                                            */
/*                      }                                                     */
/*                                                                            */
/*Step3: insert presetup function into main: preSetupTimer();                 */
/*                                                                            */
/*                                                                            */
/*Step4: setup a timer: setupTimer(unsigned int *_timerNameAdd, unsigned int8 */
/*                      _timerType, unsigned int32 _timerValue,               */
/*                      int _timerPrecision);                                 */
/*                                                                            */
/*Step5: enable timer: enableTimer(unsigned int *_timerName);                 */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*Step6: to suspend timer using: disableTimer(unsigned int *_timerName);      */
/*                                                                            */
/******************************************************************************/

//define max timer be created
#define timerNum      5   //max timer will be created
#define timerOffset  65230 // preset timer value to achive estimated time 10ms
//define desision of timer

#define timer_10ms    1
#define timer_100ms   10
#define timer_1s      100
#define timer_1m      6000
//end define 

//define type of timer
#define ONDT   1 //on delay timer
#define TOGDT  3 //tongle timer
//end define
unsigned int8           i;                // using for couting array's elements at timer1_isr
volatile unsigned int8  nextSlot;         // next slot for new timer
volatile unsigned int32 remainTime[timerNum];   //contain remaining time of timer
int                     timerEnable[timerNum];  //contain timer enable command
int                     timerRunning[timerNum]; //contain timer running state
unsigned int            *timer[timerNum];        //contain timer fire bit
volatile unsigned int32 timerValue[timerNum];   //contain set value of timer
unsigned int8           timerType[timerNum];    //contain type of timer TOFF,TON,TOGL


void runTimerMng(){
      set_timer1(timerOffset); //10ms per interrupt
   //

   for(i = 0; i < timerNum; i++){
      if(timerEnable[i]){//check for timer enable
         //check if timer is running or firered or standby
         if(timerRunning[i]){
         
            if(remainTime[i] > 0){ //timer is running
               remainTime[i] = remainTime[i] - 1;
               
            }
            else//timer firered
            {
               /*check type of timer
               if timer is repeatidly timer enable is not change
               if timer is ONDT or OFFDT then timer enable is set to false
               */
               
               if(timerType[i] == ONDT){
                  //disable timer
                  timerEnable[i] = false;
                  timerRunning = false;
                  //set firered bit
                  *timer[i] = 1;   
               }
               
               else if(timerType[i] == TOGDT){           
                  //reverse firered bit
                  if(*timer[i] == 1) *timer[i] = 0;
                  else *timer[i] = 1;    
                  //recharge timer
                  remainTime[i] = timerValue[i];
               }
               
              }
            }
         else{//timer is enabled but not running yet
            //setting new remaining time equal to setting time
            remainTime[i] = timerValue[i];
            timerRunning[i] = true;
         }
      }
   }
}
void setupTimer(unsigned int *_timerNameAdd, unsigned int8 _timerType, unsigned int32
_timerValue,int _timerPrecision){
   if(nextSlot >= timerNum){}//there is no remaining slot for new timer, do nothing.
   else{
      timer[nextSlot] = _timerNameAdd;//save poiter to array
      timerType[nextSlot] = _timerType;
      int32 calTimerValue;
      switch (_timerPrecision){
         case timer_10ms://10ms equal to timer 1 cycle
            calTimerValue = _timerValue;
            break;
         case timer_100ms:
            calTimerValue = _timerValue * 10;
            break;
         case timer_1s:
            calTimerValue = _timerValue * 100;
            break;
         case timer_1m:
            calTimerValue = _timerValue * 6000;
            break;
         default: 
            break;
      }
      timerValue[nextSlot] = calTimerValue;
      nextSlot++;
   }
}
int findTimerIndex(unsigned int *_timerName){ //find index of timer in declared timer array
   int k;
   for(k=0; k < timerNum; k++){
      if(timer[k] == _timerName){
         
         return k;
      }
   }
   return -1;
}

void enableTimer(unsigned int *_timerName){
   int index = findTimerIndex(_timerName);
   //set timer enable
   if(index != -1){
   
      timerEnable[index] = true;
      if(timerType[index] == ONDT ) *_timerName = 0;
         
      if(timerType[index] == TOGDT ) *_timerName = 0;
   }
}

void disableTimer(unsigned int *_timerName){
   int index = findTimerIndex(_timerName);
   //set timer enable
   if(index != -1) {
      timerEnable[index] = false;
      timerRunning[index] = false;
   }
}
void preSetupTimer(){
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
   set_timer1(timerOffset);
   enable_interrupts(INT_TIMER1);
   enable_interrupts(GlOBAL);
}
