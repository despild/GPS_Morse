#include <string.h>

int GPSrxPin=0;
int GPStxPin =1;
int tonePin= 8;
int pttPin =11;
int unitTime = 150; 
int dotTime =unitTime;
int dashTime =unitTime*3;
int timeWord =unitTime*3;
int timeLetter=unitTime*7;
int TONE =700;
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


float fLa=0;
float fLo=0;
float fAl=0;
int floatCount=0;

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
    


    fLa+=la.toFloat();
    fLo +=lo.toFloat();
    fAl +=al.toFloat();
    floatCount ++;
    
    
    if(morseTime){

      la=String(fLa/(float)floatCount/100);
      lo=String(fLo/(float)floatCount/100);
      al=String(fAl/(float)floatCount);
      digitalWrite(pttPin,HIGH);
      delay(1000);
      message="time:"+time+"latitude:"+la+","+ns+"longitude:"+lo+","+ew+"altitude:"+al+"satellites:"+sat;
     
     morseTalk(message);

        
        digitalWrite(pttPin,LOW);
        morseTime =false;
        timeCounter=millis();
    }
   
   


    
    readLine ="";
    stringComplete=false;

    fLa= 0;
    fLo=0;
    fAl=0;
    floatCount=0;
   
    
  }
  
  if((millis()-timeCounter)>5*60*1000){
       morseTime=true; 
  }
  

  digitalWrite(pttPin,HIGH);
  delay(1000);
  morseTalk("opensat2012");
//  delay(timeLetter);
  digitalWrite(pttPin,LOW);
  
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
  //Serial.println("!TO");
  // Returns how long a dit or da is in ms.
  if (character == "."){
    tone(tonePin,TONE);
    delay(dotTime);
    noTone(tonePin);
    delay(dotTime);
  }
  else if (character == "-"){
    tone(tonePin,TONE);
    delay(dashTime);
    noTone(tonePin);
    delay(dotTime);
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
   //Serial.println("!MT");
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
