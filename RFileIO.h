
#include <Arduino.h>
#include <SD.h>
#include "main.h"

class RFileIO{
  
  public:
      RFileIO();
      void begin();
      void log(String msg);
      String listRecipes();
      void openRecipe(int numOf,STEP* steps); //this opens the recipe (it takes the number of it in the dir, not its ID
  
  private:
  String readLine(boolean includeingComment=false);  //reads line of the current file;
  
    File myFile;
};
