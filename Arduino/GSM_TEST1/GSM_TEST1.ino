//#include<SoftwareSerial.h>
#include "blinkled123.h"

//SoftwareSerial GSM_A6(2,3);
#define GSM_A6 Serial

String returnStr="";
char sigA6=0;

bool A6Command(String Command){
  int i=0;
  GSM_A6.print(Command);
  for(int i=0;i<500;i++){
    if(GSM_A6.available()>0){
      returnStr=GSM_A6.readString();
      //Serial.print(returnStr);
      return true;
   }
   delay(20);
  }
}

bool isConnect(){
  A6Command("AT\r\n");
  if(returnStr.indexOf("OK")!=-1){
    //Serial.println("connect ok");
    returnStr="";
    return true;
  }
  else{
    //Serial.println("error");
    return false;
  }
}

void initA6(){
  while(!isConnect()){ delay(100); }
  A6Command("ATE0\r\n");
  delay(2000);
  doubleblinkled(13);
  do{
    A6Command("AT+CGATT=1\r\n");
  }while(returnStr.indexOf("OK")==-1);
  do{
    A6Command("AT+CGACT=1,1\r\n");
  }while(returnStr.indexOf("OK")==-1);
  doubleblinkled(13);
  sigA6=0;
}

void restartA6(){
  do{
    A6Command("AT+CGATT=1\r\n");
  }while(returnStr.indexOf("OK")==-1);
  
  do{
    A6Command("AT+CGACT=1,1\r\n");
  }while(returnStr.indexOf("OK")==-1);
  doubleblinkled(13);
  sigA6=0;
}

bool sendData(String pm25,String pm10,String temperature,String pressure,String humidity){
  do{
    A6Command("AT+CIPSTART=TCP,39.108.108.117,8080\r\n");
  }while(returnStr.indexOf("CONNECT OK")==1);
  A6Command("AT+CIPSEND\r\n");
  delay(50);
  String data_temp="GET /PM2.5_war/insertdata?poiNo=001&pm25="+pm25+"&pm10="+pm10+"&temperature="+temperature+"&pressure="+pressure+"&humidity="+humidity+" HTTP/1.1\r\nHost: 39.108.108.117:8080\r\n\r\n\r\n";
  A6Command(data_temp);
  if(returnStr.indexOf("200")!=-1){
    A6Command("AT+CIPCLOSE\r\n");
    return true;
  }
  else{
    sigA6++;
    A6Command("AT+CIPCLOSE\r\n");
    return false;
  }
  tribleblinkled(13);
}

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(115200);
  GSM_A6.begin(115200);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  initA6();
}

void loop() {
  if(sigA6>10)restartA6();
  sendData("21.5","35.7","12.43","1013.4","61.4");
  delay(10000);
}
