#ifndef MAIN_H

#include <Arduino.h>

typedef struct STEP{
  enum TYPE{INIT,WAIT,HEAT,END} type;  //Init->Targettemp for startup , WAIT -> hold given temp for given minute, HEAT -> heat to temp , END -> recipe is in ended state
  byte temp; //temperatue for PID
  byte minute; // only applicable for WAIT type (holds temp for minute minutes)
};

#define MAX_NUM_OF_STEPS 25



#define MAIN_H
#endif
