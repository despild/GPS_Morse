#include <string.h>
#include <Wire.h>
int GPSAddress = 0x42;

int GPSrxPin=0;
int GPStxPin =1;
int tonePin= 8;
int pttPin =11;
int unitTime = 60; 
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

void rec_init(){
  Wire.beginTransmission(GPSAddress);
  Wire.write(0xff);
  Wire.endTransmission();
 
  Wire.beginTransmission(GPSAddress);
  Wire.requestFrom(GPSAddress,50);
} 
  
  
void setup(){
  Wire.begin();
  //pinMode(GPSrxPin,INPUT);
  //pinMode(GPStxPin,OUTPUT);
  pinMode(pttPin,OUTPUT);
  pinMode(tonePin,OUTPUT);
  Serial.begin(9600);
  readLine.reserve(200);
  timeCounter=(int)millis();

  delay(1000);
  /*
  //test
  digitalWrite(pttPin,HIGH);
  delay(1000);
  
  message="test123";

  morseTalk(message);

  digitalWrite(pttPin,LOW);
  timeCounter=millis();
  
  
  delay(500);
  
  */
}



void loop(){
   rec_init();
   while (Wire.available()) {

    // get the new byte:
    char inChar = (char)Wire.read(); 
    // add it to the inputString:
    readLine += inChar;

    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
  //Serial.println("WireExit");
  Wire.endTransmission();
    //Serial.println("0");
  
  if (stringComplete) {
      //Serial.println("1");
    //Serial.println(readLine);
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
      //  Serial.println("2");
      la=String(fLa/(float)floatCount);
      lo=String(fLo/(float)floatCount);
      al=String(fAl/(float)floatCount);
      digitalWrite(pttPin,HIGH);
      delay(1000);
/*

      morseTalk("time:");
      morseTalk(time);
      morseTalk(",latitude:");
      morseTalk(la);
      morseTalk(",");
      morseTalk(ns);
      morseTalk(",longitude:");
      morseTalk(lo);
      morseTalk(",");
      morseTalk(ew);
      morseTalk(",altitude:");
      morseTalk(al);
      morseTalk(",satellites:");
      morseTalk(sat);
*/     
     
           message="time:"+time+"latitude:"+la+","+ns+"longitude:"+lo+","+ew+"altitude:"+al+"satellites:"+sat;
     
     morseTalk(message);

     
     
//     morseTalk(message);

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
   // Serial.println("3");
//  if((millis()-timeCounter)>5*60*1000){
  if((millis()-timeCounter)>100000){
       morseTime=true; 
  }
  

  digitalWrite(pttPin,HIGH);
  // Serial.println("4");
  delay(1000);
  //Serial.println("singleline");
  //morseTalk("singleline");//12chars
//  delay(timeLetter);
  digitalWrite(pttPin,LOW);
  
  delay(100);

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


void morseTalk(String str){
   //Serial.println("!MT");
   char tempStr;
   str.toLowerCase();
   Serial.print(str);
   Serial.println(str.length());
   for(int x =0,strlen =str.length();x<strlen;x++){
     Serial.print(x);
     Serial.print(":");
     tempStr = str.charAt(x);
     Serial.println(tempStr);
     if(tempStr=='a'){
         toneOut(".");
         toneOut("-");
         delay(timeWord);
     }else if(tempStr=='b'){
         toneOut("-");
         toneOut(".");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
      }else if(tempStr=='c'){
         toneOut("-");
         toneOut(".");
         toneOut("-");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=='d'){
         toneOut("-");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=='e'){
         toneOut(".");
         delay(timeWord);
}else if(tempStr=='f'){
         toneOut(".");
         toneOut(".");
         toneOut("-");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=='g'){
         toneOut("-");
         toneOut("-");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=='h'){
         toneOut(".");
         toneOut(".");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=='i'){
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=='j'){
         toneOut(".");
         toneOut("-");
         toneOut("-");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=='k'){
         toneOut("-");
         toneOut(".");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=='l'){
         toneOut(".");
         toneOut("-");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=='m'){
         toneOut("-");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=='n'){
         toneOut("-");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=='o'){
         toneOut("-");
         toneOut("-");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=='p'){
         toneOut(".");
         toneOut("-");
         toneOut("-");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=='q'){
         toneOut("-");
         toneOut("-");
         toneOut(".");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=='r'){
         toneOut(".");
         toneOut("-");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=='s'){
         toneOut(".");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=='t'){
         toneOut("-");
         delay(timeWord);
}else if(tempStr=='u'){
         toneOut(".");
         toneOut(".");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=='v'){
         toneOut(".");
         toneOut(".");
         toneOut(".");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=='w'){
         toneOut(".");
         toneOut("-");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=='x'){
         toneOut("-");
         toneOut(".");
         toneOut(".");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=='y'){
         toneOut("-");
         toneOut(".");
         toneOut("-");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=='z'){
         toneOut("-");
         toneOut("-");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=='0'){
         toneOut("-");
         toneOut("-");
         toneOut("-");
         toneOut("-");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=='1'){
         toneOut(".");
         toneOut("-");
         toneOut("-");
         toneOut("-");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=='2'){
         toneOut(".");
         toneOut(".");
         toneOut("-");
         toneOut("-");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=='3'){
         toneOut(".");
         toneOut(".");
         toneOut(".");
         toneOut("-");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=='4'){
         toneOut(".");
         toneOut(".");
         toneOut(".");
         toneOut(".");
         toneOut("-");
         delay(timeWord);
}else if(tempStr=='5'){
         toneOut(".");
         toneOut(".");
         toneOut(".");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=='6'){
         toneOut("-");
         toneOut(".");
         toneOut(".");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=='7'){
         toneOut("-");
         toneOut("-");
         toneOut(".");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=='8'){
         toneOut("-");
         toneOut("-");
         toneOut("-");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=='9'){
         toneOut("-");
         toneOut("-");
         toneOut("-");
         toneOut("-");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=='.'){
         toneOut(".");
         toneOut("-");
         toneOut(".");
         toneOut("-");
         toneOut(".");
         toneOut("-");
         delay(timeWord);
}else if(tempStr==','){
         toneOut("-");
         toneOut("-");
         toneOut(".");
         toneOut(".");
         toneOut("-");
         toneOut("-");
         delay(timeWord);
}else if(tempStr==':'){
         toneOut("-");
         toneOut("-");
         toneOut("-");
         toneOut("-");
         toneOut(".");
         toneOut(".");
         delay(timeWord);
}else if(tempStr=='-'){
         toneOut("-");
         toneOut(".");
         toneOut(".");
         toneOut(".");
         toneOut(".");
         toneOut("-");
         delay(timeWord);
}else{
       Serial.print("error");
     }
     
   }
  
}
