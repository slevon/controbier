// UTFT_Demo_320x240 (C)2012 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// This program is a demo of how to use most of the functions
// of the library with a supported display modules.
//
// This demo was made for modules with a screen resolution 
// of 320x240 pixels.
//
// This program requires the UTFT library.
//

#include "main.h"
#include <SD.h>
#include <UTFT.h>
#include "RFileIO.h"


#define FG_1 VGA_WHITE
#define BG_1 VGA_BLACK
#define FG_2 VGA_RED
#define BG_2 VGA_BLACK 

#define COLOR_HIGHLIGHT VGA_LIME  //highlight
#define COLOR_ALERT VGA_RED  //alert


//windows:
#define W_MAIN 0
#define W_SETTINGS 1


// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t SevenSegNumFont[];
extern uint8_t BigFont[];

extern unsigned int duff_beer[0x44C];

// Uncomment the next line for Arduino 2009/Uno
//UTFT myGLCD(ITDB32S,19,18,17,16);   // Remember to change the model parameter to suit your display module!

// Uncomment the next line for Arduino Mega
UTFT myGLCD(SSD1963,38,39,40,41);   // Remember to change the model parameter to suit your display module!

RFileIO file;

unsigned long previousMillis = 0;
long interval = 1000; 

STEP steps1[MAX_NUM_OF_STEPS];
STEP steps2[MAX_NUM_OF_STEPS];
String recipe1;//name of the current receipt on ch 1
String recipe2;


void setup()
{
  
  Serial.begin(9600);
  randomSeed(analogRead(0));
  
// Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  // Clear the screen and draw the frame
  myGLCD.clrScr();
  
  myGLCD.drawBitmap(0,0,35,62,duff_beer);
  
  myGLCD.print("Initialisierung....",CENTER,120);
  
  file.begin();
   myGLCD.print("Teste Logging......",CENTER,120);
  //Start main log:
   file.log("--------------------------------"); 
   
  
   myGLCD.print("Oeffene Rezept....",CENTER,120);
   file.openRecipe(1,steps1);
   delay(1000);
   //load default window:
   //we start with ther main window
    initWMain();
}

int currentWindow=W_MAIN;

void loop()
{
   switch (currentWindow){
     case W_MAIN:     updateWMain(); break;
     case W_SETTINGS: updateWSettings(); break;
   } 
}


void initWMain(){
    // Clear the screen and draw the frame
    myGLCD.clrScr();
  
    myGLCD.setColor(255, 0, 0);
    myGLCD.fillRect(0, 0, 319, 13);
    myGLCD.setColor(FG_1);
    myGLCD.setBackColor(255, 0, 0);
    myGLCD.print(" ControBier v0.1 ", CENTER, 1);
    
    //Draw State-Frame:
    myGLCD.setColor(FG_1);
    myGLCD.setBackColor(BG_1);
    myGLCD.drawRoundRect(2, 85, 158, 111);
    myGLCD.drawRoundRect(162, 85, 318, 111);    
}

void updateWMain(){
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > interval) {
      // save the last time you blinked the LED 
      previousMillis = currentMillis;   
      
      myGLCD.setBackColor(0, 0, 0);
      myGLCD.setColor(255, 255, 255);
      myGLCD.setFont(SmallFont);
      myGLCD.print("Zeit:" ,3, 223);
      myGLCD.printNumI((millis()/1000),45,223);
      
      myGLCD.print("RAM:" ,100, 223);
      myGLCD.printNumI(freeRam()/1000,140,223);
      
      //string List
      myGLCD.setColor(FG_2);
      myGLCD.setBackColor(BG_2);
      drawStringList(1,136,50,158,"Das ist eine\nDas ist einen Lange lange Liste mit einem viel zu langen Namen\nListe mit \nHighlight\nund\nnoch\nmehr\nund\nund",2);
      drawStringList(161,136,50,158,file.listRecipes(),millis()%2);
      //state
      drawState(2, 85, 158, 111,(millis()%2)?true:false);
      drawState(162, 85, 318, 111,(millis()%2)?true:false);
      
      //Temperaturen
      drawTemp(1,millis()%100,((millis()%2)?true:false));
      drawTemp(2,millis()%100,((millis()%2)?true:false));
    }
}


void updateWSettings(){

}

//////////////////////////////////////////////////////////////////////////////////////
///UTILIES
//////////////////////////////////////////////////////////////////////////////////////

void drawTemp(byte id, int temp,boolean active){
    
    int x=30;
    int y=15;
    if(id==2){
      x=190;
      y=15;
    }
    myGLCD.setBackColor(BG_1);
    //Temperaturen
    myGLCD.setFont(SevenSegNumFont);
    if(active){
        myGLCD.setColor(COLOR_ALERT);
    }else{
       myGLCD.setColor(COLOR_HIGHLIGHT);
    }
    myGLCD.printNumI(temp,x,y+20);
    myGLCD.setFont(BigFont);
    myGLCD.print("o",x+64,y+20);
   
    myGLCD.setFont(BigFont);
    myGLCD.setColor(FG_1);
    myGLCD.printNumI(id,x+32,y);

}

void drawStringList(int x, int y, int height, int width, String data,int current){ //data ist newline separated,  current will highlight the line

    int curr=0;
    int last=0;
    int to=0;
    myGLCD.setFont(SmallFont);
    String buff;
    int i=0;
    
    
    //draw the frame:
    //Draw List-Frame:
    myGLCD.setColor(FG_1);
    myGLCD.setBackColor(BG_1);
    myGLCD.drawRoundRect(x, y, x+width, y+height);
    
    while(data.length() > to){
      to=data.indexOf('\n',last);
      buff=data.substring(last,to);
      if( i == current){
         //we draw a full width background:
        //myGLCD.setColor(BG_2); //rect color= text background
        //myGLCD.fillRect(x+2,y+2+(i*12),x-2+width,y-2+(i*12)+12);
        //now the text color
        myGLCD.setColor(FG_2);
        myGLCD.setBackColor(BG_2);   
      }else{
        myGLCD.setColor(FG_1);
        myGLCD.setBackColor(BG_1);
         
      }
      // TODO very hackky need to create a for loop that appends the exact number.
      //if(buff.length()*8<width){ //if string is shorter the width, we append spaces so the background is equal for all
      //  buff= buff + "           ";
      //}
      
      //if string is too long:
      if(buff.length()*8-10 > width){ //is string is longer than width+padding
        buff=buff.substring(0,(width/8)-1);
      }
     
      myGLCD.print(buff,x+5,y+1+(i*12));
      last=to+1;
      i++;
      if(height <((i+1)*12)){
        break;  //no mor space for a line;
      }
    }
}


void drawState(int x, int y,int x2,int y2, boolean state){

   myGLCD.setFont(BigFont);
    if(state){
        myGLCD.setColor(COLOR_ALERT);
        myGLCD.setBackColor(COLOR_ALERT);
        myGLCD.fillRoundRect(x, y, x2, y2);
        myGLCD.setColor(FG_1);
        myGLCD.print("Heizen",x+(x2-x)/2-40,y+(y2-y)/2-8);
    }else{
        myGLCD.setColor(VGA_BLUE);
        myGLCD.setBackColor(VGA_BLUE);
        myGLCD.fillRoundRect(x, y, x2, y2);
        myGLCD.setColor(FG_1);
        myGLCD.print("Rasten",x+(x2-x)/2-40,y+(y2-y)/2-8);
    
    }
}


int freeRam(){
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0? (int) &__heap_start : (int) __brkval);
}
