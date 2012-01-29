#include <string.h>
#define GPSrxPin 0
#define GPStxPin 1
#define tonePin 8
#define pttPin 11
#define dotTime 1000/4
#define dashTime 1000/4*3
#define TONE_C4 262
String readLine ="";
boolean stringComplete = false;

String time="hhmmss";
String la="ddmm.mmmm";
String ns="a";
String lo="dddmm.mmmm";
String ew="a";
String al="x.x";
String sat="xx";
String message="test";
int codeSize = 39;
int timeWord = 1000/4*3*1.3;
int timeLetter = 1000/4*3*1.3;
String code[] = {
  ".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--..",".----","..---","...--","....-",".....","-....","--...","---..","----.","-----",".-.-.-","--..--","---..."};
String coderef[] = {
  "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","1","2","3","4","5","6","7","8","9","0",".",",",":"};

boolean morseTime=true;
int timeCounter=0;

void setup(){
  pinMode(GPSrxPin,INPUT);
  pinMode(GPStxPin,OUTPUT);
  pinMode(pttPin,OUTPUT);
  pinMode(tonePin,OUTPUT);
  Serial.begin(9600);
  readLine.reserve(200);
  timeCounter=(int)millis();

  delay(1000);
  
  //test
  
  
      digitalWrite(pttPin,HIGH);
      delay(1000);
      
      message="test123";


      Serial.println(message);

      //Serial.println(message.length());
      
      //int strlength=message.length();
      
      morseTalk(message);
      /*
      for (int x = 0; x < strlength; x++){
//        toneOut(".");
        String ch = message.substring(x,x+1);
        String ch2 = message.substring(x+1,x+2);
        flashLetter(ch);
 //       toneOut(".");
        if (ch == ""){
          delay(timeWord);
        }
        else if (ch != "" && ch2 != "") {
          delay(timeLetter);
        }
 //toneOut("-"); 
      }
      
      */

      digitalWrite(pttPin,LOW);
      timeCounter=millis();
  
  
  
  
  
}



void loop(){
  if (stringComplete) {
//    Serial.println(readLine);
    if(splitString(readLine,',',0)=="$GPGGA"){
      time =splitString(readLine,',',1);
      la =splitString(readLine,',',2);
      ns =splitString(readLine,',',3);
      lo =splitString(readLine,',',4);
      ew =splitString(readLine,',',5);
      al =splitString(readLine,',',9);
    }
    if(splitString(readLine,',',0)=="$GPGSV"){
      sat = splitString(readLine,',',3);
    }
    
    
    if(morseTime){
      
      digitalWrite(pttPin,HIGH);
      delay(1000);
      message="time:"+time+"latitude:"+la+","+ns+"longitude:"+lo+","+ew+"altitude:"+al+"satellites:"+sat;
     
     morseTalk(message);
/*
      for (int x = 0,strlength = message.length(); x < strlength; x++){

        String ch = message.substring(x,x+1);
        String ch2 = message.substring(x+1,x+2);
        flashLetter(ch);
        if (ch == ""){
          delay(timeWord);
        }
        else if (ch != "" && ch2 != "") {
          delay(timeLetter);
        }
     
      }
        */
        
        digitalWrite(pttPin,LOW);
        morseTime =false;
        timeCounter=millis();
    }
   
   
   
   
    
    readLine ="";
    stringComplete=false;
    
   
    
  }
  
  if((millis()-timeCounter)>5*60*1000){
       morseTime=true; 
  }
  
  delay(50);

}

void serialEvent(){

  while (Serial.available()) {

    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    readLine += inChar;

    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
  
}


String splitString(String s, char parser,int index){
  String rs='\0';
  int parserIndex = index;
  int parserCnt=0;
  int rFromIndex=0, rToIndex=-1;

  while(index>=parserCnt){
    rFromIndex = rToIndex+1;
    rToIndex = s.indexOf(parser,rFromIndex);

    if(index == parserCnt){
      if(rToIndex == 0 || rToIndex == -1){
        return '\0';
      }
      return s.substring(rFromIndex,rToIndex);
    }
    else{
      parserCnt++;
    }

  }
  return rs;
}


/*


void flashLetter (String character){
  Serial.println("!FL");
  // Flashes the letter
  String morseCode = searchLetter (character);

  int strlength = morseCode.length();
  for (int x = 0; x < strlength; x++){
    String ch = morseCode.substring(x,x+1);
    toneOut(ch);
  }
}

*/
void toneOut (String character){
  Serial.println("!TO");
  // Returns how long a dit or da is in ms.
  if (character == "."){
    tone(tonePin,TONE_C4,dotTime);
    delay(dashTime*1.3);
    noTone(tonePin);
  }
  else if (character == "-"){
    tone(tonePin,TONE_C4,dashTime);
    delay(dashTime*1.3);
    noTone(tonePin);
  }
}
/*

String searchLetter (String character){
Serial.println("!SL");
  // Returns the morse code for a character.
  for (int x = 0; x < codeSize; x++){
    character.toLowerCase();
    if (character == coderef[x]){
      Serial.println(code[x]);
      return code[x];
    }
  }
  return "";
}
*/

void morseTalk(String str){
   Serial.println("!MT");
   String tempStr="";
   str.toLowerCase();
   for(int x =0,strlen =str.length();x<strlen;x++){
     tempStr = str.substring(x,x+1);
     if(tempStr=="a"){
         toneOut(".");
         toneOut("-");
         delay(timeWord);
     }else if(tempStr=="b"){
         toneOut("-");
         toneOut(".");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
      }else if(tempStr=="c"){
         toneOut("-");
         toneOut(".");
         toneOut("-");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=="d"){
         toneOut("-");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=="e"){
         toneOut(".");
         delay(timeWord);
}else if(tempStr=="f"){
         toneOut(".");
         toneOut(".");
         toneOut("-");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=="g"){
         toneOut("-");
         toneOut("-");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=="h"){
         toneOut(".");
         toneOut(".");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=="i"){
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=="j"){
         toneOut(".");
         toneOut("-");
         toneOut("-");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=="k"){
         toneOut("-");
         toneOut(".");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=="l"){
         toneOut(".");
         toneOut("-");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=="m"){
         toneOut("-");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=="n"){
         toneOut("-");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=="o"){
         toneOut("-");
         toneOut("-");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=="p"){
         toneOut(".");
         toneOut("-");
         toneOut("-");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=="q"){
         toneOut("-");
         toneOut("-");
         toneOut(".");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=="r"){
         toneOut(".");
         toneOut("-");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=="s"){
         toneOut(".");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=="t"){
         toneOut("-");
         delay(timeWord);
}else if(tempStr=="u"){
         toneOut(".");
         toneOut(".");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=="v"){
         toneOut(".");
         toneOut(".");
         toneOut(".");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=="w"){
         toneOut(".");
         toneOut("-");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=="x"){
         toneOut("-");
         toneOut(".");
         toneOut(".");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=="y"){
         toneOut("-");
         toneOut(".");
         toneOut("-");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=="z"){
         toneOut("-");
         toneOut("-");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=="0"){
         toneOut("-");
         toneOut("-");
         toneOut("-");
         toneOut("-");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=="1"){
         toneOut(".");
         toneOut("-");
         toneOut("-");
         toneOut("-");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=="2"){
         toneOut(".");
         toneOut(".");
         toneOut("-");
         toneOut("-");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=="3"){
         toneOut(".");
         toneOut(".");
         toneOut(".");
         toneOut("-");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=="4"){
         toneOut(".");
         toneOut(".");
         toneOut(".");
         toneOut(".");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=="5"){
         toneOut(".");
         toneOut(".");
         toneOut(".");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=="6"){
         toneOut("-");
         toneOut(".");
         toneOut(".");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=="7"){
         toneOut("-");
         toneOut("-");
         toneOut(".");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=="8"){
         toneOut("-");
         toneOut("-");
         toneOut("-");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=="9"){
         toneOut("-");
         toneOut("-");
         toneOut("-");
         toneOut("-");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=="."){
         toneOut(".");
         toneOut("-");
         toneOut(".");
         toneOut("-");
         toneOut(".");
         toneOut("-");
         delay(timeWord);
}else if(tempStr==","){
         toneOut("-");
         toneOut("-");
         toneOut(".");
         toneOut(".");
         toneOut("-");
         toneOut("-");
         delay(timeWord);
}else if(tempStr==":"){
         toneOut("-");
         toneOut("-");
         toneOut("-");
         toneOut("-");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else{
       
     }
     
   }
  
}
