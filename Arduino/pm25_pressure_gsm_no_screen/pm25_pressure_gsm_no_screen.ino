#include <SFE_BMP180.h>
#include <DHT12.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "blinkled123.h"

//define hardware serial port as GSM_A6
#define GSM_A6 Serial

//Software Serial port for PM2.5 sensor
SoftwareSerial mySerial(2, 3);
// You will need to create an SFE_BMP180 object, here called "pressure":
SFE_BMP180 pressure;
DHT12 dht12;
String t1,t2,t3,t4;
String returnStr="";
int sigA6=0;

void(* resetFunc) (void) = 0;

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
  A6Command("AT&F\r\n");
  delay(100);
  A6Command("ATE0\r\n");
  delay(2000);
  doubleblinkled(13);
  do{
    A6Command("AT+CGATT=1\r\n");
  }while(returnStr.indexOf("OK")==-1);
  delay(200);
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
  delay(200);
  do{
    A6Command("AT+CGACT=1,1\r\n");
  }while(returnStr.indexOf("OK")==-1);
  doubleblinkled(13);
  sigA6=0;
}

bool sendData(String pm25,String pm10,String temperature,String pressure,String humidity){
  int i=3;
  do{
    returnStr="";
    A6Command("AT+CIPSTART=TCP,39.108.108.117,8080\r\n");
    if(i--<=0)break;
  }while(returnStr.indexOf("CONNECT OK")==-1);
  A6Command("AT+CIPSEND\r\n");
  delay(50);
  String data_temp="GET /PM2.5_war/insertdata?poiNo=001&pm25="+pm25+"&pm10="+pm10+"&temperature="+temperature+"&pressure="+pressure+"&humidity="+humidity+" HTTP/1.1\r\nHost: 39.108.108.117:8080\r\n\r\n\r\n";
  returnStr="";
  A6Command(data_temp);
  if(returnStr.indexOf("200")!=-1){
    A6Command("AT+CIPCLOSE\r\n");
    tribleblinkled(13);
    return true;
  }
  else{
    sigA6++;
    //Serial.println(sigA6);
    A6Command("AT+CIPCLOSE\r\n"+String(sigA6));
    return false;
  }
}


void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);//PM2.5 Sensor
  if(pressure.begin())
  {  ;}
  else
  {while(1); // Pause forever. 
  }
 
  GSM_A6.begin(115200);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  initA6();
}


void loop() {
  // put your main code here, to run repeatedly:
  if(sigA6>10)resetFunc();//restart A6 module anyway if sigA6 shows connection error
  int num=20;//average no
  int sig=0;
  int i=0;
  int a[10]={0};
  char status;
  double T,P,H,pm25,pm10;
  double T_avg=0,P_avg=0,H_avg=0,pm25_avg=0,pm10_avg=0;
  while(sig<num)
  {
  while(mySerial.available()>0){
    /*********************
    * get PM2.5 from Serial port per second, while data recieved, Arduino reads pressure, temperature and humidity from sensor.
    *
    */
      for(i=0;i<9;i++){
        a[i]=a[i+1];
      }
      a[i]=(int)mySerial.read();
      delay(2);//must do this
      //Serial.println(a[i]);
      if(a[0]==170){
        //get PM2.5
        pm25=(float)(a[3]*256+a[2])/10;
        pm10=(float)(a[5]*256+a[4])/10;
        if(pm25<0||pm10<0)continue;
        //get temperature and pressure
        status = pressure.startTemperature();
        if (status != 0)
        {
          delay(status);
          status = pressure.getTemperature(T);
        }
        status = pressure.startPressure(3);
        if (status != 0)
        {
          delay(status);
          status = pressure.getPressure(P,T);
        }

        H=dht12.readHumidity();//get Humidity

        //sum
        T_avg+=T;
        P_avg+=P;
        H_avg+=H;
        pm25_avg+=pm25;
        pm10_avg+=pm10;
        sig++;
      } 
    }
  }
  //convert double to string
 t1=String(T_avg/num);
 t2=String(P_avg/num);
 t3=String(pm25_avg/num);
 t4=String((double)H_avg/num);

  sendData(String(pm25_avg/num),String(pm10_avg/num),String(T_avg/num),String(P_avg/num),String((double)H_avg/num));
}
