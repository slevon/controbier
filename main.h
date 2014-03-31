#ifndef MAIN_H

#include <Arduino.h>

typedef struct STEP{
  enum TYPE{WAIT,HEAT,END} type;
  byte value; //min or temperatue
};

#define MAX_NUM_OF_STEPS 25



#define MAIN_H
#endif
