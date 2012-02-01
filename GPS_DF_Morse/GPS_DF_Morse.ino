#include <Wire.h>

#define BUFFER_LENGTH 10//Define the buffer length

int tonePin= 12;
int pttPin =13;
int unitTime = 60; 
int dotTime =unitTime;
int dashTime =unitTime*3;
int timeWord =unitTime*3;
int timeLetter=unitTime*7;
int TONE =700;


char morseTime[20];
char morseLon[20];
char morseLonDir[1];
char morseLat[20];
char morseLatDir[1];
char morseAlt[10];

double dTime=0.0;
double dLon=0.0;
double dLat=0.0;
double dAlt=0.0;

int lonCnt=0;
int latCnt=0;
int altCnt=0;

float systime = (float)millis();

int GPSAddress = 0x42;//GPS I2C Address

double Datatransfer(char *data_buf,char num)//Data type converter：convert char type to float
{                                           //*data_buf:char data array ;num:float length
  double temp=0.0;
  unsigned char i,j;

  if(data_buf[0]=='-')//负数的情况
  {
    i=1;
    //数组中的字符型数据转换成整数并累加
    while(data_buf[i]!='.')
      temp=temp*10+(data_buf[i++]-0x30);
    for(j=0;j<num;j++)
      temp=temp*10+(data_buf[++i]-0x30);
    //将转换后的整数转换成浮点数
    for(j=0;j<num;j++)
      temp=temp/10;
    //转换成负数
    temp=0-temp;
  }
  else//正数情况
  {
    i=0;
    while(data_buf[i]!='.')
      temp=temp*10+(data_buf[i++]-0x30);
    for(j=0;j<num;j++)
      temp=temp*10+(data_buf[++i]-0x30);
    for(j=0;j<num;j++)
      temp=temp/10 ;
  }
  return temp;
}
void rec_init()//initial GPS
{
  Wire.beginTransmission(GPSAddress);
  Wire.write(0xff);//发送数据所在的地址      
  Wire.endTransmission(); 

  Wire.beginTransmission(GPSAddress);
  Wire.requestFrom(GPSAddress,10);//要求从GPS器件读取10个字节
}
char ID()//接收语句的ID
{
  char i = 0;
  char value[7]={
    '$','G','P','G','G','A',','      };//要接收的GPS语句的ID内容
  char buff[7]={
    '0','0','0','0','0','0','0'      };

  while(1)
  {
    rec_init();//接收数据初始化    
    while(Wire.available())   
    { 
      buff[i] = Wire.read();//接收串口的数据  
      if(buff[i]==value[i])//对比是否是正确的ID
      {
        i++;
        if(i==7)
        {
          Wire.endTransmission();//结束接收
          return 1;//接收完毕返回1
        }
      }
      else
        i=0;
    }
    Wire.endTransmission();//结束接收
  }
}


void UTC()//获取时间信息
{
  char i = 0,flag=0;
  char value[7]={
    '$','G','P','G','G','A',','   };
  char buff[7]={
    '0','0','0','0','0','0','0'       };
  char time[9]={
    '0','0','0','0','0','0','0','0','0'    };//存放时间数据
  double t=0.0;

  while(1)
  {
    
    rec_init();    
    while(Wire.available())   
    { 
      if(!flag)
      { 
        buff[i] = Wire.read();

        if(buff[i]==value[i])
        {
          i++;

          if(i==7)
          {
            i=0;
            flag=1;
          }
        }
        else
          i=0;
      }
      else
      {
        time[i] = Wire.read();
        i++;
        if(i==9)
        {
          t=Datatransfer(time,2);//转换成浮点型数据
          t=t+90000.00;//将时间转换成北京时间
          Serial.println(t);//输出时间数据 
          dTime=t;
          if(dTime > 240000){
            dTime -=240000;
          }
          Wire.endTransmission();
          return;
        }
      }
    }
    Wire.endTransmission(); 
  }
}
void rec_data(char *buff,char num1,char num2)//接收数据子函数
{                                            //*buff：存放接收数据的数组；num1：逗号数目；num2：数组长度。
  char i=0,count=0;

  if(ID())
  {
    while(1)
    {
      rec_init();    
      while(Wire.available())   
      { 
        buff[i] = Wire.read();
        if(count!=num1)
        {  
          if(buff[i]==',')
            count++;
        }
        else
        {
          i++;
          if(i==num2)
          {
            Wire.endTransmission();
            return;
          }
        }
      }
      Wire.endTransmission();
    }
  }
}

boolean isFix(){
  char i = 0,flag=0;
  char value[7]={'$','G','P','G','S','A',','};
  char buff[7]={'0','0','0','0','0','0','0'};
  char fix='0';//存放时间数据
  int count=0;
  
  while(1){
    rec_init();    
    while(Wire.available()){ 
      if(flag==0){
        buff[i] = Wire.read();
        if(buff[i]==value[i]){
          i++;
          if(i==7){
            i=0;
            flag=1;
            //check $GPGSA for debug
            /*
            for (int x =0;x<7;x++){
              Serial.print(buff[x]); 
            }
            Serial.println();
            */
          }
        }else{
          i=0;
        }
      }else{
        fix = Wire.read();
        if(count != 1){
          if(fix == ','){
            count++;  
          }
        }else{
          if(fix=='2'||fix=='3'){
            Serial.println("Fixed");
            Serial.print("Fix Status:");
            Serial.println(fix);

            Wire.endTransmission();
            return true; 
          }else{
            Serial.print("Fix Status:");
            Serial.println(fix);
            Wire.endTransmission();
            return false; 
          }
        }
      }
    }
    Wire.endTransmission();
  }
}
   
    
void latitude()//获取纬度信息
{
  double temp;
  char lat[10]={
    '0','0','0','0','0','0','0','0','0','0' };//存放纬度数据
  rec_data(lat,1 ,10);//接收纬度数据
  if(lat[0]=='-'){
    lat[0]='0'; 
  }
  temp = Datatransfer(lat,5);
  dLat += temp;
  latCnt++;
  Serial.println(temp,5);//将纬度数据转换成浮点型数据并输出
}
void lat_dir()//获取纬度方向信息
{
  char dir[1]={'0'};//存放纬度方向数据
  rec_data(dir,2,1);//接收纬度方向数据
  Serial.println(dir[0]);//将纬度方向信息输出
}
void  longitude()//获取经度信息
{
  double temp;
  char lon[11]={
    '0','0','0','0','0','0','0','0','0','0','0' };//存放经度数据
  rec_data(lon,3,11);//接收经度数据
  if(lon[0]=='-'){
    lon[0]='0'; 
  }
  temp =Datatransfer(lon,5);
  dLon+=temp;
  lonCnt++;
  Serial.println(temp,5);//将经度数据转换成浮点型数据并输出
}
void lon_dir()//获取经度方向信息
{
  char dir[1]={'0'};
  rec_data(dir,4,1);
  Serial.println(dir[0]);
}
void altitude()//获取海拔信息
{
  double temp;
  char i=0,count=0;
  char alt[8]={
    '0','0','0','0','0','0','0','0' };

  if(ID())
  {
    while(1)
    {
      rec_init();    
      while(Wire.available())   
      { 
        alt[i] = Wire.read();
        if(count!=8)
        {  
          if(alt[i]==',')
            count++;
        }
        else
        {
          if(alt[i]==',')
          {
            temp = Datatransfer(alt,1);
            dAlt += temp;
            altCnt++;
            Serial.println(temp,1);
            Wire.endTransmission();
            return;
          }
          else
            i++;
        }
      }
      Wire.endTransmission();
    }
  }
}
void setup()
{
  pinMode(pttPin,OUTPUT);
  pinMode(tonePin,OUTPUT);
  Wire.begin();//IIC初始化
  Serial.begin(9600);//设置波特率
  Serial.println("DFRobot DFRduino GPS Shield v1.0");
  Serial.println("$GPGGA statement information: ");
}
void loop()
{
  
//  if(isFix()){
  if(1){


    Serial.println(' ');
    Serial.print("UTC:");
    UTC();
    Serial.print("Lat:");
    latitude();
    lat_dir();
    Serial.print("Lon:");
    longitude();
    lon_dir();
    Serial.print("Alt:");
    altitude();
    Serial.println(' ');
    

    float timeDiff =(float)millis()-systime;
    Serial.println(millis());
    Serial.println(timeDiff);

    if(timeDiff > 60.0*1000){   // need pairing type casting in comparison
      Serial.println("!MORSE");
      int tempInt=0;
      digitalWrite(pttPin,HIGH);
      delay(1000);
      tempInt = double2CharArr(dTime,0,morseTime);
      morseTalk('t');
      morseTalk('i');
      morseTalk('m');
      morseTalk('e');
      morseTalk(':');
      morseTalk(morseTime,tempInt);
      morseTalk(',');    
      tempInt = double2CharArr((double)(dLat/latCnt),4,morseLat);
      Serial.println(dLat);
      Serial.println(dLat/latCnt);
//      for(int i=0;i<tempInt;i++){
//        Serial.println(morseLat[i]);
//      }
      morseTalk('l');
      morseTalk('a');
      morseTalk('t');
      morseTalk(':');
      morseTalk(morseLat,tempInt);
      morseTalk(',');
      morseTalk(morseLatDir,1);
      morseTalk(',');
      tempInt = double2CharArr((double)(dLon/lonCnt),4,morseLon);
      morseTalk('l');
      morseTalk('o');
      morseTalk('n');
      morseTalk(':');
      morseTalk(morseLon,tempInt);
      morseTalk(',');
      morseTalk(morseLonDir,1);
      morseTalk(',');
      tempInt = double2CharArr((double)(dAlt/altCnt),1,morseAlt);
      morseTalk('a');
      morseTalk('l');
      morseTalk('t');
      morseTalk(':');
      morseTalk(morseAlt,tempInt);
      morseTalk('m');
      digitalWrite(pttPin,LOW);
      dLat=0;
      dLon=0;
      dAlt=0;
      latCnt=0;
      lonCnt=0;
      altCnt=0;
       
      systime = (float)millis();  
    }
  
    delay(100);
  }
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




void morseTalk(char c){
  char tempS[1];
 tempS[0]=c;
morseTalk(tempS,1); 
}


void morseTalk(char * str,int n){
   char tempStr;
   for(int x =0;x<n;x++){
     tempStr =str[x];
     Serial.print(tempStr);
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
}else if(tempStr=='\0'){
 Serial.print("NULL"); 
  
}else{
       Serial.print("error");
     }
     
   }
  
}


int double2CharArr(double d, int dot, char *buff){
  if(d>=0){
    long intPart = (long)d;
    long noneIntPart = (long)((d-intPart)*(pow(10,dot)+0.000005)); 
    int intCnt=0;
    long num =intPart;
    while(1){
      num/=10;
      if(num == 0){
        break;
      }else{
        intCnt++; 
      }
    }
    intCnt++;
    Serial.println(intPart);
    Serial.print("!!!");
    for(int i =0;i<intCnt;i++){
//      Serial.print(intPart/(long)(pow(10,intCnt-i-1)+0.5));
       buff[i]=int2Char((intPart/(long)(pow(10,intCnt-i-1)+0.5)));
       intPart =intPart%((long)(pow(10,intCnt-i-1)+0.5));
    }
    buff[intCnt]='.';
    for(int i=0;i<dot;i++){
      buff[i+1+intCnt]=int2Char((noneIntPart/(long)(pow(10,dot-i-1)+0.5)));
      noneIntPart =noneIntPart%((long)(pow(10,dot-i-1)+0.5));
      
    }
    
    return intCnt+dot+1;
  }else if(d<0){
    long intPart = (long)(d*(-1));
    long noneIntPart = (long)(((d*(-1))-intPart)*(pow(10,dot)+0.000005)); 
    int intCnt=0;
    long num =intPart;
    while(1){
      num/=10;
      if(num == 0){
        break;
      }else{
        intCnt++; 
      }
    }
    intCnt++;

    buff[0]='-';
    
    for(int i =0;i<intCnt;i++){
       buff[i+1]=int2Char((intPart/(long)(pow(10,intCnt-i-1)+0.5)));
       //Serial.println(intPart);
       intPart =intPart%((long)(pow(10,intCnt-i-1)+0.5));
    }
    buff[intCnt+1]='.';
    for(int i=0;i<dot;i++){
      buff[i+2+intCnt]=int2Char((noneIntPart/(long)(pow(10,dot-i-1)+0.5)));
      noneIntPart =noneIntPart%((long)(pow(10,dot-i-1)+0.5));
    }
    return intCnt+dot+2; 
  }else{
    double2CharArr(0.0,dot,buff); 
    
  }
}




char int2Char(int n){
  switch(n){
    case 0:
      return '0';
    case 1:
      return '1';
    case 2:
      return '2';
    case 3:
      return '3';
    case 4:
      return '4';
    case 5:
      return '5';
    case 6:
      return '6';
    case 7:
      return '7';
    case 8:
      return '8';
    case 9:
      return '9';    
  } 
  
  return '\0';
}

