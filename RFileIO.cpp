#include "RFileIO.h"

RFileIO::RFileIO(){
}

void RFileIO::begin(){
   Serial.print(F("Initializing SD card..."));
   pinMode(53, OUTPUT);
   
  if (!SD.begin(53)) {
    Serial.println(F("initialization failed!"));
    return;
  }
  Serial.println(F("initialization done."));
}
  
void RFileIO::log(String msg){
   myFile = SD.open("main.log", FILE_WRITE);
    if (myFile) {
       myFile.print(millis());
       myFile.println(":"+msg);
       myFile.close();
    }else{
      Serial.println("error in RFileIO::logFile");
    }
}



String RFileIO::listRecipes(){
  String names;
  File dir=SD.open("/RECIP/");
  do{
     myFile.close();
     myFile=dir.openNextFile();
     //read ther first line where the name is.
     names+=readLine(true).substring(1)+"\n";  // remove the '#'
  }while(myFile);
  
  return names;
}

String RFileIO::readLine(boolean includingComment){
  char ch;
  String string;
  boolean lineIsComment=false;
  while(ch != '\n' && myFile.available()){
    ch=myFile.read();
    if( ch =='#'){
       lineIsComment=true;
    }
    string+=ch;
  }
  string.trim();
  
  if(lineIsComment && !includingComment){
    return readLine(includingComment);  //recursion until uncommentd line
  }
  
  return string;
}

void RFileIO::openRecipe(int numOfFile,STEP* steps){


  Serial.print("Opening Receipt: ");
  Serial.println(numOfFile);
  
  String names;
  File dir=SD.open("/RECIP/");
  for(int i=0; i<=numOfFile;i++){
     myFile.close();
     myFile=dir.openNextFile();
  }
  Serial.print("File:");
  Serial.println(myFile.name());
  
  //now we hav our file open.
  String line;
  int i=0;
  while(myFile.available()){
   line=readLine(false);
   Serial.println(line);
   if(line.startsWith("heat")){
     steps[i].type=STEP::HEAT;
   }else if(line.startsWith("wait")){
     steps[i].type=STEP::WAIT;
   }else{
     steps[i].type=STEP::END;
     break;
   }
   
   steps[i].temp =(byte) line.substring(line.indexOf("\t")+1).toInt();
   Serial.print(steps[i].type);Serial.print("|"); Serial.println(steps[i].temp);
   i++;
   if(i>= MAX_NUM_OF_STEPS){
     break;
   }
  }
  //End
  steps[i].type=STEP::END;
  
  
  myFile.close();
}

